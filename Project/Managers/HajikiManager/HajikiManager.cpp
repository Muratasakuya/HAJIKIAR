#include "HajikiManager.h"

#include "ImGuiManager.h"
#include "OpenCV.h"

/*////////////////////////////////////////////////////////////////////////////////
*									Hajiki追加
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::AddHajiki(HajikiType type, std::unique_ptr<GameObject3D> obj) {

	// 初期回転角
	float initRotateX = std::numbers::pi_v<float> / 2.0f;
	// Hajikiモデルのハーフサイズ -> 衝突判定で使用する
	const float kHajikiHalfSize = 0.02f;

	// 共通設定
	obj->SetRotate({ initRotateX ,0.0f,0.0f });
	obj->SetHalfSize(kHajikiHalfSize);
	obj->SetColliderType(ColliderType::Sphere);
	obj->SetEnableHalfLambert(true);

	Physics<Vector2> physics{};
	physics.velocity.Initialize();
	physics.acceleration.Initialize();
	physics.mass = mass_;

	Friction friction{};
	// 重力加速度
	friction.kGravity_ = { 0.0f,-9.8f };
	// 動摩擦係数
	friction.miu_ = 0.32f;
	// 動摩擦力の大きさ
	friction.magnitude_ = friction.miu_ * std::sqrtf(-physics.mass * friction.kGravity_.y);
	// 向き
	friction.dirction_ = Vector2::Normalize(physics.velocity);
	// 動摩擦力
	friction.frictionalForce_ =
	{ friction.magnitude_ * -friction.dirction_.x,friction.magnitude_ * -friction.dirction_.y };

	// 追加
	hajikiesInitPos_[type].push_back(obj->GetCenterPos());
	hajikies_[type].emplace_back(std::move(obj), physics, friction);

	for (auto& hajiki : hajikies_[type]) {

		// 座標の適応
		hajiki.pos = { hajiki.object->GetCenterPos().x,hajiki.object->GetCenterPos().y };

		// 名前の設定 DebugName
		if (type == HajikiType::Player) {

			hajiki.name_ = "HajikiPlayer";
		} else if (type == HajikiType::Line) {

			hajiki.name_ = "HajikiLine";
			hajiki.object->SetRotate({ initRotateX,std::numbers::pi_v<float>,0.0f });
		} else if (type == HajikiType::Target) {

			hajiki.name_ = "HajikiTarget";
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									衝突更新処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::CollisionUpdate() {

	// PlayerHajikiが間を通ったかの判定
	CheckPassLineCollision();
	// 虚の衝突判定
	CheckBlockToHajikiCollision();
	// 機雷との衝突判定
	CheckToHajikiCollision();

	// コライダーリセット
	collisionManagers_[Reality]->Reset();

	// コライダー追加
	collisionManagers_[Reality]->AddCollider(hajikies_[HajikiType::Player][Imaginary].object.get());
	collisionManagers_[Reality]->AddCollider(hajikies_[HajikiType::Line][0].object.get());
	collisionManagers_[Reality]->AddCollider(hajikies_[HajikiType::Line][1].object.get());
	collisionManagers_[Reality]->AddCollider(hajikies_[HajikiType::Target][Imaginary].object.get());

	// 全てのオブジェクトの衝突判定
	collisionManagers_[Reality]->CheckAllCollisions();

	// 衝突したペアを記録するためのセット
	std::set<std::pair<HajikiData*, HajikiData*>> collidedPairsSet;

	for (auto& pair : hajikies_) {
		auto& hajikiList = pair.second;
		for (auto& hajiki : hajikiList) {
			// 衝突していたら
			if (hajiki.object->GetIsHit()) {
				for (auto& otherPair : hajikies_) {
					auto& otherHajikiList = otherPair.second;
					for (auto& otherHajiki : otherHajikiList) {
						if (hajiki.object != otherHajiki.object && otherHajiki.object->GetIsHit()) {

							// ペアを保存
							auto pair1 = std::make_pair(&hajiki, &otherHajiki);
							auto pair2 = std::make_pair(&otherHajiki, &hajiki);

							// 既に反射処理されたペアか確認
							if (collidedPairsSet.find(pair1) == collidedPairsSet.end() &&
								collidedPairsSet.find(pair2) == collidedPairsSet.end()) {

								// 新しいペアをセットに追加
								collidedPairsSet.insert(pair1);

								// 衝突したペアの反射速度を計算
								ReflectVelocity(*pair1.first, *pair1.second);
							}
						}
					}
				}
			}
		}
	}

	for (auto& pair : hajikies_) {
		auto& hajikiList = pair.second;

		for (size_t i = 0; i < hajikiList.size(); i++) {

			auto& hajiki = hajikiList[i];

			// ARモードで
			if (mode_ == ApplicationMode::AR) {
				if (pair.first == HajikiType::Player) {
					continue;
				}
				if (pair.first == HajikiType::Target) {
					continue;
				}
			}

			// 壁との衝突判定
			WallCollision(hajiki);
			// 速度の適応
			ApplyVelocityAndFriction(hajiki);
		}
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*									マウス移動
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::MouseMove(HajikiType type) {

	if (hajikies_[type].empty()) {
		return;
	}

	if (gameOver_) {
		return;
	}

	if (CheckAllHajikiStop()) {
		if (isClear_) {
			return;
		}
	}

	// マウス座標
	mousePos_ = NewMoon::GetMousePos();

	// 左クリックしたら && Count0
	if (NewMoon::PushMouseLeft() && moveCount_ == 0) {

		// 左クリックしたときの座標
		mousePressPos_ = NewMoon::GetMousePos();

		// カウントを進める
		moveCount_ = 1;
	}

	hajikies_[type][Reality].mouseMove_ = true;

	// Count1
	if (moveCount_ == 1) {
		// 左クリックを離したとき
		if (!NewMoon::PushMouseLeft()) {

			// Hajikiに足す速度の設定
			hajikies_[type][Reality].physics.velocity = (mousePressPos_ - mousePos_) * 2.0f;

			// 速度ベクトルの大きさを計算
			float velocityLength = Vector2::Length(hajikies_[type][Reality].physics.velocity);

			// 最低限の速度の値を超えていれば
			if (Vector2::fabs(hajikies_[type][Reality].physics.velocity) > minVelocity_) {
				if (velocityLength > maxVelocity_.x) {

					// 速度ベクトルを正規化して最大速度を適応
					hajikies_[type][Reality].physics.velocity = Vector2::Normalize(hajikies_[type][Reality].physics.velocity) * maxVelocity_;
					hajikies_[type][Reality].physics.velocity.y *= -1.0f;
				}

				// カウントを進める
				moveCount_ = 2;
			} else {

				// 最低限の速度の値を超えていなければ動かない
				// カウントを0に戻す
				moveCount_ = 0;
			}
		}
	}

	// Count2
	if (moveCount_ == 2) {

		// 摩擦計算
		hajikies_[type][Reality].friction.magnitude_ =
			hajikies_[type][Reality].friction.miu_ * std::sqrtf(-hajikies_[type][Reality].physics.mass * hajikies_[type][Reality].friction.kGravity_.y);
		hajikies_[type][Reality].friction.dirction_ = Vector2::Normalize(hajikies_[type][Reality].physics.velocity);
		hajikies_[type][Reality].friction.frictionalForce_ =
		{ hajikies_[type][Reality].friction.magnitude_ * -hajikies_[type][Reality].friction.dirction_.x,hajikies_[type][Reality].friction.magnitude_ * -hajikies_[type][Reality].friction.dirction_.y };

		// 加速度計算
		hajikies_[type][Reality].physics.acceleration =
		{ hajikies_[type][Reality].friction.frictionalForce_.x / hajikies_[type][Reality].physics.mass,hajikies_[type][Reality].friction.frictionalForce_.y / hajikies_[type][Reality].physics.mass };

		// 速度計算
		hajikies_[type][Reality].physics.velocity += hajikies_[type][Reality].physics.acceleration / 60.0f;

		// 座標に速度を足す
		hajikies_[type][Reality].pos += hajikies_[type][Reality].physics.velocity / 60.0f;
		hajikies_[type][Reality].object->SetTranslate({ hajikies_[type][Reality].pos.x,hajikies_[type][Reality].pos.y,hajikies_[type][Reality].object->GetCenterPos().z });

		// 下限に行けば止まる
		if (Vector2::fabs({ hajikies_[type][Reality].physics.velocity.x, hajikies_[type][Reality].physics.velocity.y }) < minVelocity_) {

			hajikies_[type][Reality].physics.velocity = { 0.0f,0.0f };
			moveCount_ = 0;

			// 強化状態解除
			hajikies_[type][Reality].isPower_ = false;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									ARモードの移動
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::ARMove() {

	ImGui::Begin("Velocity");

	ImGui::Text("playerVelocity: %f,%f", hajikies_[HajikiType::Player][Reality].physics.velocity.x, hajikies_[HajikiType::Player][Reality].physics.velocity.y);
	ImGui::Text("targetVelocity: %f,%f", hajikies_[HajikiType::Target][Reality].physics.velocity.x, hajikies_[HajikiType::Target][Reality].physics.velocity.y);

	ImGui::End();

	// 壁の端
	const Vector2 edgeSize = { 0.41f, 0.231f };

	OpenCV* openCV = OpenCV::GetInstance();
	openCV->SetGame3DMode(true);
	openCV->SetEdgeSize({ edgeSize.x,edgeSize.y });

	// 追跡する色
	hajikies_[HajikiType::Player][Reality].pos = openCV->GetTrackColorPos();
	hajikies_[HajikiType::Player][Reality].object->SetTranslate(
		{ hajikies_[HajikiType::Player][Reality].pos.x,hajikies_[HajikiType::Player][Reality].pos.y * -1.0f,
		hajikies_[HajikiType::Player][Reality].object->GetCenterPos().z });

	hajikies_[HajikiType::Target][Reality].pos = openCV->GetOtherColorPos();
	hajikies_[HajikiType::Target][Reality].object->SetTranslate(
		{ hajikies_[HajikiType::Target][Reality].pos.x,hajikies_[HajikiType::Target][Reality].pos.y * -1.0f,
		hajikies_[HajikiType::Target][Reality].object->GetCenterPos().z });

	// 速度の計算（60フレームごと）
	static int frameCount = 0;
	static Vector2 playerPreviousPos = hajikies_[HajikiType::Player][Reality].pos;
	static Vector2 targetPreviousPos = hajikies_[HajikiType::Target][Reality].pos;

	frameCount++;

	if (frameCount >= 5) {
		// 速度 = (現在位置 - 前回の位置) / 経過時間
		hajikies_[HajikiType::Player][Reality].physics.velocity.x = (hajikies_[HajikiType::Player][Reality].pos.x - playerPreviousPos.x) * 5.0f;
		hajikies_[HajikiType::Player][Reality].physics.velocity.y = (hajikies_[HajikiType::Player][Reality].pos.y - playerPreviousPos.y) * 5.0f;

		hajikies_[HajikiType::Target][Reality].physics.velocity.x = (hajikies_[HajikiType::Target][Reality].pos.x - targetPreviousPos.x) * 5.0f;
		hajikies_[HajikiType::Target][Reality].physics.velocity.y = (hajikies_[HajikiType::Target][Reality].pos.y - targetPreviousPos.y) * 5.0f;

		// 次の計算のために位置を保存
		playerPreviousPos = hajikies_[HajikiType::Player][Reality].pos;
		targetPreviousPos = hajikies_[HajikiType::Target][Reality].pos;

		// フレームカウントをリセット
		frameCount = 0;
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*								ゲームプレイ中のリセット
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::Reset() {

	if (NewMoon::TriggerKey(DIK_R)) {

		for (auto& pair : hajikies_) {
			auto& hajikiList = pair.second;
			for (uint32_t i = 0; i < hajikiList.size(); i++) {

				// 初期座標で初期化
				hajikiList[i].object->SetTranslate(hajikiesInitPos_[pair.first][i]);
				hajikiList[i].object->SetColor({
					hajikiList[i].object->GetColor().x,hajikiList[i].object->GetColor().y ,hajikiList[i].object->GetColor().z ,1.0f });
				hajikiList[i].pos = { hajikiList[i].object->GetCenterPos().x,hajikiList[i].object->GetCenterPos().y };

				hajikiList[i].physics.velocity.Initialize();
				hajikiList[i].physics.acceleration.Initialize();

				hajikiList[i].isLeave = false;
				hajikiList[i].isPower_ = false;
			}
		}

		for (const auto& block : blocks_) {

			block->SetIsHit(false);
		}

		hajikies_[HajikiType::Player][Reality].isPower_ = false;
		hajikies_[HajikiType::Player][Reality].isLeave = false;

		playerSoulFollow_ = false;
		isLeaveWaitPlayerSoul_ = false;
		// 60f
		playerSoulInvincibleTime_ = 60.0f;

		moveCounter_ = 0;
		gameOverCount_ = -1;
		isMove_ = false;
		gameOver_ = false;

		zPos_ = hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().z;
		changeAlpha_ = 0.01f;
		alpha_ = 1.0f;
	}

	if (gameOver_ && alpha_ <= 0.0f) {

		for (auto& pair : hajikies_) {
			auto& hajikiList = pair.second;
			for (uint32_t i = 0; i < hajikiList.size(); i++) {

				// 初期座標で初期化
				hajikiList[i].object->SetTranslate(hajikiesInitPos_[pair.first][i]);
				hajikiList[i].object->SetColor({
					hajikiList[i].object->GetColor().x,hajikiList[i].object->GetColor().y ,hajikiList[i].object->GetColor().z ,1.0f });
				hajikiList[i].pos = { hajikiList[i].object->GetCenterPos().x,hajikiList[i].object->GetCenterPos().y };

				hajikiList[i].physics.velocity.Initialize();
				hajikiList[i].physics.acceleration.Initialize();

				hajikiList[i].isLeave = false;
				hajikiList[i].isPower_ = false;
			}
		}

		for (const auto& block : blocks_) {

			block->SetIsHit(false);
		}

		hajikies_[HajikiType::Player][Reality].isPower_ = false;
		hajikies_[HajikiType::Player][Reality].isLeave = false;

		playerSoulFollow_ = false;
		isLeaveWaitPlayerSoul_ = false;
		// 60f
		playerSoulInvincibleTime_ = 60.0f;

		moveCounter_ = 0;
		gameOverCount_ = -1;
		isMove_ = false;
		gameOver_ = false;

		zPos_ = hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().z;
		changeAlpha_ = 0.01f;
		alpha_ = 1.0f;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								    初期化
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::Initialize() {

	// 衝突管理
	// 生成
	for (uint32_t i = 0; i < collisionManagers_.size(); i++) {

		collisionManagers_[i] = std::make_unique<CollisionManager>();
	}

	// falseで初期化
	isClear_ = false;
	playerSoulFollow_ = false;
	// 60f
	playerSoulInvincibleTime_ = 60.0f;

	// カウンター0
	moveCounter_ = 0;
	gameOverCount_ = -1;
	isMove_ = false;
	gameOver_ = false;

	zPos_ = hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().z;
	changeAlpha_ = 0.01f;
	alpha_ = 1.0f;

	/*-------------------------------------------------------------------------------------------------------------------*/
	// 魂が抜けた時のオブジェクト

	// 初期回転角
	float initRotateX = std::numbers::pi_v<float> / 2.0f;
	// Hajikiモデルのハーフサイズ -> 衝突判定で使用す
	const float kHajikiHalfSize = 0.018f;
	// 色 半透明くらい
	const Vector4 soulColor = { 1.0f,1.0f,1.0f,0.35f };

	// モデル
	const std::string soulModelName = "mainHajiki4.gltf";
	// テクスチャ
	const std::string soulTextureName = "white.png";

	soulObject_ = std::make_unique<GameObject3D>(GameObjectType::Model);

	// 初期化
	soulObject_->Initialize();
	soulObject_->SetTranslate(hajikies_[HajikiType::Player][Reality].object->GetCenterPos());
	soulObject_->SetRotate({ initRotateX ,0.0f,0.0f });
	soulObject_->SetColor(soulColor);
	// 使用するテクスチャとモデル
	soulObject_->SetTexture(soulTextureName);
	soulObject_->SetModel(soulModelName);

	// コライダー情報
	soulObject_->SetColliderType(ColliderType::Sphere);
	soulObject_->SetHalfSize(kHajikiHalfSize);

}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::Update() {

	ImGui::Begin("DeltaTime");

	ImGui::Text("playerSoulInvincibleTime_ : %4.1f", playerSoulInvincibleTime_);
	ImGui::Text("playerSoulInvincibleTime_ : %4.1f", playerSoulInvincibleTime_);
	ImGui::InputFloat("thresholdVelocity", &thresholdVelocity_);

	std::string invincible = std::format("invincible {}", playerSoulFollow_);
	ImGui::Text(invincible.c_str());

	ImGui::End();

	// リセット処理
	Reset();

	// 魂が離れているときのPlayerの更新
	LeaveSoulPlayerUpdate();

	// LineHajikiの更新処理
	LineHajikiUpdate();

	// ゴール判定
	GoalCheck();

	/*--------------------------------------------------------------------------------------------------------------------------------------------*/
	/// 動いた回数の更新とその処理

	ImGui::Begin("PlayCount");

	ImGui::Text("moveCounter: %d", moveCounter_);
	ImGui::Text("gameOverCount: %d", gameOverCount_);
	ImGui::Text("alpha: %4.1f", alpha_);

	ImGui::End();

	if (moveCount_ == 2 && !isMove_) {

		// 動いたら+1
		moveCounter_++;
		isMove_ = true;
	}
	if (isMove_ && moveCount_ == 0) {

		isMove_ = false;

		// 魂が離れた時のカウント
		if (hajikies_[HajikiType::Player][Reality].isLeave) {

			gameOverCount_++;
		} else {

			gameOverCount_ = -1;
		}
	}

	if (gameOverCount_ == 2 && CheckAllHajikiStop()) {

		// ゲームオーバー
		gameOver_ = true;
	}

	// ゲームオーバーになったとき
	if (gameOver_) {

		zPos_ += 0.01f;

		if (alpha_ > 0.0f) {

			alpha_ -= changeAlpha_;
		}

		// Zを見た目上落として色の透明度も下げる
		hajikies_[HajikiType::Player][Imaginary].object->SetTranslate({
				hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().x,
			hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().y,zPos_ });
		hajikies_[HajikiType::Player][Imaginary].object->SetColor({
				hajikies_[HajikiType::Player][Imaginary].object->GetColor().x,
			hajikies_[HajikiType::Player][Imaginary].object->GetColor().y,
			hajikies_[HajikiType::Player][Imaginary].object->GetColor().z,alpha_ });
	}

	/*--------------------------------------------------------------------------------------------------------------------------------------------*/
	/// クリアしたときの処理



	/*--------------------------------------------------------------------------------------------------------------------------------------------*/
	/// 座標更新

	if (!gameOver_) {

		if (mode_ == ApplicationMode::GAME3D) {

			// TargetHajiki1の座標
			Vector2 targetHajikiPos =
			{ hajikies_[HajikiType::Target][Imaginary].object->GetCenterPos().x,hajikies_[HajikiType::Target][Imaginary].object->GetCenterPos().y };

			// TargetHajiki2のXY座標をTargetHajiki1と合わせる
			hajikies_[HajikiType::Target][Reality].object->SetTranslate(
				{ targetHajikiPos.x,targetHajikiPos.y,hajikies_[HajikiType::Target][Reality].object->GetCenterPos().z });
		} else if (mode_ == ApplicationMode::AR) {

			// TargetHajiki1の座標
			Vector2 targetHajikiPos =
			{ hajikies_[HajikiType::Target][Reality].object->GetCenterPos().x,hajikies_[HajikiType::Target][Reality].object->GetCenterPos().y };

			// TargetHajiki2のXY座標をTargetHajiki1と合わせる
			hajikies_[HajikiType::Target][Imaginary].object->SetTranslate(
				{ targetHajikiPos.x,targetHajikiPos.y,hajikies_[HajikiType::Target][Imaginary].object->GetCenterPos().z });
		}

		if (!hajikies_[HajikiType::Player][Reality].isLeave) {

			// 無敵時間カウント
			if (playerSoulFollow_) {

				playerSoulInvincibleTime_ -= deltaTime_;
				if (playerSoulInvincibleTime_ < 0.0f) {

					// 無敵時間終了
					playerSoulFollow_ = false;
				}
			}

			// PlayerHajiki1の座標
			Vector2 playerHajikiPos =
			{ hajikies_[HajikiType::Player][Reality].object->GetCenterPos().x,hajikies_[HajikiType::Player][Reality].object->GetCenterPos().y };

			// PlayerHajiki2のXY座標をPlayerHajiki1と合わせる
			hajikies_[HajikiType::Player][Imaginary].object->SetTranslate(
				{ playerHajikiPos.x,playerHajikiPos.y,hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().z });

			// PlayerHajiki1の速度をPlayerHajiki2に代入
			hajikies_[HajikiType::Player][Imaginary].physics.velocity = hajikies_[HajikiType::Player][Reality].physics.velocity;
			hajikies_[HajikiType::Player][Imaginary].physics.acceleration = hajikies_[HajikiType::Player][Reality].physics.acceleration;
		} else {

			// 座標はその場に留まる
			hajikies_[HajikiType::Player][Imaginary].object->SetTranslate(
				{ hajikies_[HajikiType::Player][Imaginary].prePos.x,hajikies_[HajikiType::Player][Imaginary].prePos.y,
				hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().z });
			hajikies_[HajikiType::Player][Imaginary].physics.velocity = { 0.0f,0.0f };
			hajikies_[HajikiType::Player][Imaginary].physics.acceleration = { 0.0f,0.0f };
		}

	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::Draw() {

	for (auto& pair : hajikies_) {
		auto& hajikiList = pair.second;
		for (size_t i = 0; i < hajikiList.size(); i++) {

			auto& hajiki = hajikiList[i];

			// ARモード
			if (mode_ == ApplicationMode::AR) {
				if (pair.first == HajikiType::Player && i == static_cast<size_t>(Reality)) {
					continue;
				}
				if (pair.first == HajikiType::Target && i == static_cast<size_t>(Reality)) {
					continue;
				}
			}

			if (hajiki.object) {

				hajiki.object->Draw();
			}
		}
	}

	// 魂が抜けているとき
	if (hajikies_[HajikiType::Player][Reality].isLeave) {

		soulObject_->Draw();
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*								 Hajiki情報の取得
////////////////////////////////////////////////////////////////////////////////*/
const HajikiData& HajikiManager::GetHajiki(HajikiType type, size_t index) const {

	return hajikies_.at(type).at(index);
}

/*////////////////////////////////////////////////////////////////////////////////
*							 Hajikiサイズ(配列)の取得
////////////////////////////////////////////////////////////////////////////////*/
size_t HajikiManager::GetHajikiCount(HajikiType type) const {

	return hajikies_.at(type).size();
}

bool HajikiManager::IsClear() const {

	return isClear_;
}

/*////////////////////////////////////////////////////////////////////////////////
*							衝突相手のブロックのセット
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::SetBlocks(GameObject3D* block) {

	blocks_.push_back(block);
}

/*////////////////////////////////////////////////////////////////////////////////
*							衝突相手の機雷のセット
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::SetKiraies(GameObject3D* kirai) {

	kiraies_.push_back(kirai);
}

void HajikiManager::SetApplicationMode(const ApplicationMode& mode) {

	mode_ = mode;
}

/*////////////////////////////////////////////////////////////////////////////////
*						全てのHajikiが止まっているか
////////////////////////////////////////////////////////////////////////////////*/
bool HajikiManager::CheckAllHajikiStop() {

	if (mode_ == ApplicationMode::GAME3D) {
		if (hajikies_[HajikiType::Player][0].physics.velocity == Vector2(0.0f, 0.0f) &&
			hajikies_[HajikiType::Player][1].physics.velocity == Vector2(0.0f, 0.0f) &&
			hajikies_[HajikiType::Line][0].physics.velocity == Vector2(0.0f, 0.0f) &&
			hajikies_[HajikiType::Line][1].physics.velocity == Vector2(0.0f, 0.0f) &&
			hajikies_[HajikiType::Target][0].physics.velocity == Vector2(0.0f, 0.0f) &&
			hajikies_[HajikiType::Target][1].physics.velocity == Vector2(0.0f, 0.0f)) {

			return true;
		}
	} else if (mode_ == ApplicationMode::AR) {

		float playerVelocity0 = Vector2::Length(hajikies_[HajikiType::Player][0].physics.velocity);
		float playerVelocity1 = Vector2::Length(hajikies_[HajikiType::Player][1].physics.velocity);

		float targetVelocity0 = Vector2::Length(hajikies_[HajikiType::Target][0].physics.velocity);
		float targetVelocity1 = Vector2::Length(hajikies_[HajikiType::Target][1].physics.velocity);

		ImGui::Begin("testVelocity");

		ImGui::Text("playerVelocity0: %f", playerVelocity0);
		ImGui::Text("playerVelocity1: %f", playerVelocity1);
		ImGui::Text("targetVelocity0: %f", targetVelocity0);
		ImGui::Text("targetVelocity1: %f", targetVelocity1);

		ImGui::End();

		if (std::fabs(playerVelocity0) < thresholdVelocity_ &&
			std::fabs(playerVelocity1) < thresholdVelocity_ &&
			hajikies_[HajikiType::Line][0].physics.velocity == Vector2(0.0f, 0.0f) &&
			hajikies_[HajikiType::Line][1].physics.velocity == Vector2(0.0f, 0.0f) &&
			std::fabs(targetVelocity0) < thresholdVelocity_ &&
			std::fabs(targetVelocity1) < thresholdVelocity_) {

			return true;
		}
	}

	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*						LineHajikiの回転角の更新処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::LineHajikiUpdate() {

	// 初期回転角
	float initRotateX = std::numbers::pi_v<float> / 2.0f;

	// 差分ベクトル
	Vector3 directionToA = hajikies_[HajikiType::Line][1].object->GetCenterPos() - hajikies_[HajikiType::Line][0].object->GetCenterPos();
	Vector3 directionToB = hajikies_[HajikiType::Line][0].object->GetCenterPos() - hajikies_[HajikiType::Line][1].object->GetCenterPos();

	// Z回転角の計算
	float zRotateA = std::atan2(directionToA.y, directionToA.x);
	float zRotateB = std::atan2(directionToB.y, directionToB.x);

	hajikies_[HajikiType::Line][0].object->SetRotate({ initRotateX ,std::numbers::pi_v<float>,zRotateA });
	hajikies_[HajikiType::Line][1].object->SetRotate({ initRotateX ,std::numbers::pi_v<float>,zRotateB });
}

/*////////////////////////////////////////////////////////////////////////////////
*								反射速度の計算
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::ReflectVelocity(HajikiData& hajiki1, HajikiData& hajiki2) {

	// 速度の取得
	Vector2 velocity1 = hajiki1.physics.velocity;
	Vector2 velocity2 = hajiki2.physics.velocity;

	// 位置の取得
	Vector2 pos1 = { hajiki1.object->GetCenterPos().x, hajiki1.object->GetCenterPos().y };
	Vector2 pos2 = { hajiki2.object->GetCenterPos().x, hajiki2.object->GetCenterPos().y };

	// 角度計算
	float angle1 = std::atan2(pos2.y - pos1.y, pos2.x - pos1.x);
	float angle2 = std::atan2(pos1.y - pos2.y, pos1.x - pos2.x);

	// 反射速度の計算
	float speed1 = Vector2::Length({ velocity1.x * 0.6f + velocity2.x * 0.4f, velocity1.y * 0.6f + velocity2.y * 0.4f });
	float speed2 = Vector2::Length({ velocity2.x * 0.6f + velocity1.x * 0.4f, velocity2.y * 0.6f + velocity1.y * 0.4f });

	hajiki1.physics.velocity = { -speed1 * std::cosf(angle1), -speed1 * std::sinf(angle1) };
	hajiki2.physics.velocity = { -speed2 * std::cosf(angle2), -speed2 * std::sinf(angle2) };

	hajiki1.object->SetIsHit(false);
	hajiki2.object->SetIsHit(false);
}

/*////////////////////////////////////////////////////////////////////////////////
*								 速度の適応
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::ApplyVelocityAndFriction(HajikiData& hajiki) {

	if (gameOver_) {
		return;
	}

	if (hajiki.name_ == "HajikiPlayer" || hajiki.name_ == "HajikiTarget") {
		return;
	}

	// 速度が0.0f以外ならカウントを進める
	if (Vector2::fabs(hajiki.physics.velocity) != Vector2(0.0f, 0.0f)) {

		hajiki.moveCount = 1;
	}

	// Count1
	if (hajiki.moveCount == 1) {
		hajiki.friction.dirction_ = Vector2::Normalize(hajiki.physics.velocity);
		hajiki.friction.frictionalForce_ = {

			hajiki.friction.magnitude_ * -hajiki.friction.dirction_.x,
			hajiki.friction.magnitude_ * -hajiki.friction.dirction_.y
		};

		hajiki.physics.acceleration = {

			hajiki.friction.frictionalForce_.x / hajiki.physics.mass,
			hajiki.friction.frictionalForce_.y / hajiki.physics.mass
		};

		hajiki.physics.velocity += hajiki.physics.acceleration / 60.0f;

		if (!hajiki.mouseMove_) {

			hajiki.pos += hajiki.physics.velocity / 60.0f;
			hajiki.object->SetTranslate({ hajiki.pos.x, hajiki.pos.y, hajiki.object->GetCenterPos().z });
		}

		// 最低速度を下回ったら
		if (Vector2::fabs({ hajiki.physics.velocity.x, hajiki.physics.velocity.y }) < minVelocity_) {

			// 動きを止める
			hajiki.physics.velocity = { 0.0f, 0.0f };
			hajiki.moveCount = 0;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								壁との衝突判定
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::WallCollision(HajikiData& hajiki) {

	// 壁の端
	const Vector2 edgeSize = { 0.41f, 0.231f };

	// 壁との衝突判定
	if (collisionManagers_[Reality]->EdgeCheckCollisionX(hajiki.object.get(), edgeSize.x)) {

		hajiki.physics.velocity.x = hajiki.physics.velocity.x * -1.0f;
	}
	if (collisionManagers_[Reality]->EdgeCheckCollisionY(hajiki.object.get(), edgeSize.y)) {

		hajiki.physics.velocity.y = hajiki.physics.velocity.y * -1.0f;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								間を通ったかのチェック
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::CheckPassLineCollision() {

	if (collisionManagers_[Reality]->PassLineCheckCollision(
		hajikies_[HajikiType::Line][0].object.get(), hajikies_[HajikiType::Line][1].object.get(),
		hajikies_[HajikiType::Player][Imaginary].object.get())) {

		hajikies_[HajikiType::Player][Imaginary].object->SetIsPass(true);
	} else {

		hajikies_[HajikiType::Player][Imaginary].object->SetIsPass(false);
	}

	// 間を通過したとき
	if (hajikies_[HajikiType::Player][Imaginary].object->GetIsPass()) {

		// 強化する、Imaginaryが離れない、拾える
		hajikies_[HajikiType::Player][Reality].isPower_ = true;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								 ブロックとの衝突処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::CheckBlockToHajikiCollision() {

	for (const auto& block : blocks_) {

		// 消滅したブロックは判定を行わない
		if (block->GetIsHit()) {

			continue;
		}

		if (!hajikies_[HajikiType::Player][Reality].isLeave) {
			// 衝突したらその場に止める
			if (collisionManagers_[Imaginary]->SphereToBlockCheckCollision(
				hajikies_[HajikiType::Player][Imaginary].object.get(), block)) {

				// 強化が入っていなければ 無敵時間外
				if (!hajikies_[HajikiType::Player][Reality].isPower_ && !playerSoulFollow_) {

					// 魂が離れる
					hajikies_[HajikiType::Player][Reality].isLeave = true;

					Vector2 imaginaryPlayerPos =
					{ hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().x,
						hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().y };

					hajikies_[HajikiType::Player][Imaginary].prePos = imaginaryPlayerPos;

					soulObject_->SetTranslate({ imaginaryPlayerPos.x,imaginaryPlayerPos.y,soulObject_->GetCenterPos().z });

				} else {

					// 強化が入っているときにぶつかれば消滅する
					if (hajikies_[HajikiType::Player][Reality].isPower_) {

						block->SetIsHit(true);
					}
				}
			}
		}
	}

	// 魂が離れているとき
	if (hajikies_[HajikiType::Player][Reality].isLeave) {

		// 座標はその場に留まる
		hajikies_[HajikiType::Player][Imaginary].object->SetTranslate(
			{ hajikies_[HajikiType::Player][Imaginary].prePos.x,hajikies_[HajikiType::Player][Imaginary].prePos.y,
			hajikies_[HajikiType::Player][Imaginary].object->GetCenterPos().z });

		// 完全に離れた時
		if (!collisionManagers_[Imaginary]->SphereToSoulSphereCheckCollision(
			hajikies_[HajikiType::Player][Reality].object.get(), soulObject_.get())) {

			isLeaveWaitPlayerSoul_ = true;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								 機雷との衝突処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::CheckToHajikiCollision() {

	// 衝突すれば即死
	for (const auto& kirai : kiraies_) {
		if (collisionManagers_[Imaginary]->SphereToSphereCheckCollision(
			hajikies_[HajikiType::Player][Imaginary].object.get(), kirai)) {

			gameOver_ = true;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							  魂が離れているときのPlayerの更新
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::LeaveSoulPlayerUpdate() {

	// 魂が離れているとき
	if (isLeaveWaitPlayerSoul_) {
		// 衝突すれば
		if (collisionManagers_[Imaginary]->SphereToSoulSphereCheckCollision(
			hajikies_[HajikiType::Player][Reality].object.get(), soulObject_.get())) {

			// 魂がまたついていくようになる
			hajikies_[HajikiType::Player][Reality].isLeave = false;
			isLeaveWaitPlayerSoul_ = false;

			// 魂無敵CoolTimeカウント開始
			playerSoulFollow_ = true;
			playerSoulInvincibleTime_ = 60.0f;
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								ゴール判定とその後の処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::GoalCheck() {

	ImGui::Begin("Clear");

	std::string clear = std::format("isClear {}", isClear_);
	ImGui::Text(clear.c_str());

	ImGui::End();

	// ゴール判定
	if (collisionManagers_[Reality]->PointInTriangle(
		hajikies_[HajikiType::Player][Imaginary].object.get(), hajikies_[HajikiType::Line][0].object.get(),
		hajikies_[HajikiType::Line][1].object.get(), hajikies_[HajikiType::Target][Imaginary].object.get())) {
		// 魂が離れていなくてなおかつ間を通過したあと
		if (hajikies_[HajikiType::Player][Reality].isPower_ && !hajikies_[HajikiType::Player][Reality].isLeave) {

			isClear_ = true;
		}
	} else {

		isClear_ = false;
	}
}