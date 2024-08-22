#include "HajikiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
HajikiManager::HajikiManager() {

	// 衝突管理
	// 生成
	collisionManager_ = std::make_unique<CollisionManager>();
}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
HajikiManager::~HajikiManager() {}

/*////////////////////////////////////////////////////////////////////////////////
*									Hajiki追加
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::AddHajiki(HajikiType type, std::unique_ptr<GameObject3D> obj) {

	// 初期回転角
	float initRotateX = std::numbers::pi_v<float> / 2.0f;
	// Hajikiモデルのハーフサイズ -> 衝突判定で使用する
	const float kHajikiHalfSize = 0.018f;

	// 共通設定
	obj->SetRotate({ initRotateX ,0.0f,0.0f });
	obj->SetHalfSize(kHajikiHalfSize);
	obj->SetColliderType(ColliderType::Sphere);

	Physics<Vector2> physics{};
	physics.velocity.Initialize();
	physics.acceleration.Initialize();
	physics.mass = mass_;

	Friction friction{};
	// 重力加速度
	friction.kGravity_ = { 0.0f,-9.8f };
	// 動摩擦係数
	friction.miu_ = 0.08f;
	// 動摩擦力の大きさ
	friction.magnitude_ = friction.miu_ * std::sqrtf(-physics.mass * friction.kGravity_.y);
	// 向き
	friction.dirction_ = Vector2::Normalize(physics.velocity);
	// 動摩擦力
	friction.frictionalForce_ =
	{ friction.magnitude_ * -friction.dirction_.x,friction.magnitude_ * -friction.dirction_.y };

	// 追加
	hajikies_[type].emplace_back(std::move(obj), physics, friction);

	for (auto& hajiki : hajikies_[type]) {

		// 座標の適応
		hajiki.pos = { hajiki.object->GetCenterPos().x,hajiki.object->GetCenterPos().y };

		// 名前の設定
		if (type == HajikiType::Player) {

			hajiki.name_ = "HajikiPlayer";
		} else if (type == HajikiType::Line) {

			hajiki.name_ = "HajikiLine";
		} else if (type == HajikiType::Target) {

			hajiki.name_ = "HajikiTarget";
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGui処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::SetImGui() {

	for (auto& pair : hajikies_) {
		auto& hajikiList = pair.second;
		for (auto& hajiki : hajikiList) {

			if (hajiki.object) {

				imgui_.Set(hajiki.object.get());
			}
		}
	}
}
void HajikiManager::ImGui() {

	imgui_.Render();
}

/*////////////////////////////////////////////////////////////////////////////////
*									衝突更新処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::CollisionUpdate() {

	// コライダーリセット
	collisionManager_->Reset();

	// コライダー追加
	for (auto& pair : hajikies_) {
		auto& hajikiList = pair.second;
		for (auto& hajiki : hajikiList) {
			if (hajiki.object) {

				collisionManager_->AddCollider(hajiki.object.get());
			}
		}
	}

	// 全てのオブジェクトの衝突判定
	collisionManager_->CheckAllCollisions();

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
		for (auto& hajiki : hajikiList) {

			// 壁との衝突判定
			WallCollision(hajiki);
			// 速度の適応
			ApplyVelocityAndFriction(hajiki);
		}
	}

	// PlayerHajikiが間を通ったかの判定
	CheckPassLineCollision();

}

/*////////////////////////////////////////////////////////////////////////////////
*									マウス移動
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::MouseMove(HajikiType type) {

	if (hajikies_[type].empty()) {
		return;
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

	auto& hajikies = hajikies_[type];

	for (auto& hajiki : hajikies) {

		hajiki.mouseMove_ = true;

		// Count1
		if (moveCount_ == 1) {
			// 左クリックを離したとき
			if (!NewMoon::PushMouseLeft()) {

				// Hajikiに足す速度の設定
				hajiki.physics.velocity = (mousePressPos_ - mousePos_) * 2.0f;

				// 速度ベクトルの大きさを計算
				float velocityLength = Vector2::Length(hajiki.physics.velocity);

				// 最低限の速度の値を超えていれば
				if (Vector2::fabs(hajiki.physics.velocity) > minVelocity_) {
					if (velocityLength > maxVelocity_.x) {

						// 速度ベクトルを正規化して最大速度を適応
						hajiki.physics.velocity = Vector2::Normalize(hajiki.physics.velocity) * maxVelocity_;
						hajiki.physics.velocity.y *= -1.0f;
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
			hajiki.friction.magnitude_ = hajiki.friction.miu_ * std::sqrtf(-hajiki.physics.mass * hajiki.friction.kGravity_.y);
			hajiki.friction.dirction_ = Vector2::Normalize(hajiki.physics.velocity);
			hajiki.friction.frictionalForce_ =
			{ hajiki.friction.magnitude_ * -hajiki.friction.dirction_.x,hajiki.friction.magnitude_ * -hajiki.friction.dirction_.y };

			// 加速度計算
			hajiki.physics.acceleration =
			{ hajiki.friction.frictionalForce_.x / hajiki.physics.mass,hajiki.friction.frictionalForce_.y / hajiki.physics.mass };

			// 速度計算
			hajiki.physics.velocity += hajiki.physics.acceleration / 60.0f;

			// 座標に速度を足す
			hajiki.pos += hajiki.physics.velocity / 60.0f;
			hajiki.object->SetTranslate({ hajiki.pos.x,hajiki.pos.y,hajiki.object->GetCenterPos().z });

			// 下限に行けば止まる
			if (Vector2::fabs({ hajiki.physics.velocity.x, hajiki.physics.velocity.y }) < minVelocity_) {

				hajiki.physics.velocity = { 0.0f,0.0f };
				moveCount_ = 0;
			}
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::Update() {

	// TargetHajiki2のXY座標をTargetHajiki1と合わせる
	

}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::Draw() {

	for (auto& pair : hajikies_) {
		auto& hajikiList = pair.second;
		for (auto& hajiki : hajikiList) {

			if (hajiki.object) {

				hajiki.object->Draw();
			}
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								 Hajiki情報の取得
////////////////////////////////////////////////////////////////////////////////*/
HajikiData& HajikiManager::GetHajiki(HajikiType type, size_t index) {

	return hajikies_[type][index];
}

/*////////////////////////////////////////////////////////////////////////////////
*							 Hajikiサイズ(配列)の取得
////////////////////////////////////////////////////////////////////////////////*/
size_t HajikiManager::GetHajikiCount(HajikiType type) const {

	return hajikies_.at(type).size();
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
	float speed1 = Vector2::Length({ velocity1.x * 0.9f + velocity2.x * 0.6f, velocity1.y * 0.9f + velocity2.y * 0.6f });
	float speed2 = Vector2::Length({ velocity2.x * 0.9f + velocity1.x * 0.6f, velocity2.y * 0.9f + velocity1.y * 0.6f });

	hajiki1.physics.velocity = { -speed1 * std::cosf(angle1), -speed1 * std::sinf(angle1) };
	hajiki2.physics.velocity = { -speed2 * std::cosf(angle2), -speed2 * std::sinf(angle2) };

	hajiki1.object->SetIsHit(false);
	hajiki2.object->SetIsHit(false);
}

/*////////////////////////////////////////////////////////////////////////////////
*								 速度の適応
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::ApplyVelocityAndFriction(HajikiData& hajiki) {

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
	const Vector2 edgeSize = { 0.361f, 0.204f };

	// 壁との衝突判定
	if (collisionManager_->EdgeCheckCollisionX(hajiki.object.get(), edgeSize.x)) {

		hajiki.physics.velocity.x = hajiki.physics.velocity.x * -1.0f;
	}
	if (collisionManager_->EdgeCheckCollisionY(hajiki.object.get(), edgeSize.y)) {

		hajiki.physics.velocity.y = hajiki.physics.velocity.y * -1.0f;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								間を通ったかのチェック
////////////////////////////////////////////////////////////////////////////////*/
void HajikiManager::CheckPassLineCollision() {

	if (collisionManager_->PassLineCheckCollision(
		hajikies_[HajikiType::Line][0].object.get(), hajikies_[HajikiType::Line][1].object.get(),
		hajikies_[HajikiType::Player][0].object.get())) {

		hajikies_[HajikiType::Player][0].object->SetIsPass(true);
	} else {

		hajikies_[HajikiType::Player][0].object->SetIsPass(false);
	}

	// 通っている時に色を変える
	if (hajikies_[HajikiType::Player][0].object->GetIsPass()) {

		// 赤
		hajikies_[HajikiType::Player][0].object->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	} else {

		// 黒
		hajikies_[HajikiType::Player][0].object->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	}
}
