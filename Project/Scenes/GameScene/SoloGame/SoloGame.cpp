#include "SoloGame.h"

#include "ImGuiManager.h"

// モードセット
void SoloGame::SetApplicationMode(const ApplicationMode& mode) {

	mode_ = mode;
}

// クリア判定
bool SoloGame::IsClear() const {

	return isClear_;
}

/*////////////////////////////////////////////////////////////////////////////////
*								   初期化
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Initialize() {

	// 衝突管理
	// 生成
	collisionManager_ = std::make_unique<CollisionManager>();

	// Hajiki管理
	// 生成
	hajikiManager_ = std::make_unique<HajikiManager>();
	// モードセット
	hajikiManager_->SetApplicationMode(mode_);

	/*======================================================*/
	// テクスチャ モデル

	// debug用White
	const std::string whiteTexName = "white.png";

	// lineHajiki
	const std::string lineHajikiModelName[lineHajikiNum] = { "lineHajiki.gltf" ,"lineHajiki2.gltf" };
	// line
	const std::string lineModelName = "line.gltf";
	// mainHajiki
	const std::string playerHajikiModelName[playerHajikiNum] = { "mainHajiki0.gltf","mainHajiki1.gltf" };
	const std::string targetHajikiModelName[targetHajikiNum] = { "mainHajiki2.gltf","mainHajiki3.gltf" };

	// Cube
	std::string cubeModelName[blockNum];
	cubeModelName[0] = "cube.gltf";
	for (int i = 1; i < blockNum; ++i) {
		std::ostringstream oss;
		oss << "cube" << i + 1 << ".gltf";
		cubeModelName[i] = oss.str();
	}
	// Kirai
	std::string kiraiModelName[kiraiNum];
	kiraiModelName[0] = "kirai.gltf";
	for (int i = 1; i < kiraiNum; ++i) {
		std::ostringstream oss;
		oss << "kirai" << i + 1 << ".gltf";
		kiraiModelName[i] = oss.str();
	}

	/*======================================================*/
	// 3Dオブジェクト

	/*-------------------------------------------------------------------------------------------------------------------*/
	// PlayerHajiki

	// 初期座標
	const Vector3 playerHajikiInitPos[playerHajikiNum] = {
		Vector3(-0.1f,0.0f,1.0f),
		Vector3(-0.1f,0.0f,1.05f)
	};
	// 色
	const Vector4 playerHajikiColor = { 0.16f,0.16f ,0.16f ,1.0f };

	// 2 //
	for (uint32_t i = 0; i < playerHajikiNum; i++) {

		auto playerHajiki = std::make_unique<GameObject3D>(GameObjectType::Model);

		// 初期化
		playerHajiki->Initialize();
		playerHajiki->SetTranslate(playerHajikiInitPos[i]);
		playerHajiki->SetColor(playerHajikiColor);
		playerHajiki->SetObjectName("playerHajiki");
		// 使用するテクスチャとモデル
		playerHajiki->SetTexture(whiteTexName);
		playerHajiki->SetModel(playerHajikiModelName[i]);

		hajikiManager_->AddHajiki(HajikiType::Player, std::move(playerHajiki));
	}

	/*-------------------------------------------------------------------------------------------------------------------*/
	// LineHajiki

	// LineHajikiBodyの色
	const Vector4 lineHajikiBodyColor = { 0.8196f, 0.8784f, 0.8392f, 1.0f };
	// LineHajikiEmmisiveの色 (仮)
	const Vector4 lineHajikiEmmisiveColor = { 0.0745f, 0.8471f, 0.8471f, 1.0f };

	// LineHajikiの初期座標
	const Vector3 lineHajikiInitPos[lineHajikiNum] = {
		Vector3(0.0f,0.143f,1.05f),
		Vector3(0.0f,-0.172f,1.05f)
	};

	// 2 //
	for (uint32_t i = 0; i < lineHajikiNum; i++) {

		auto lineHajiki = std::make_unique<GameObject3D>(GameObjectType::Model);

		// 初期化
		lineHajiki->Initialize();
		lineHajiki->SetTranslate(lineHajikiInitPos[i]);
		lineHajiki->SetMaterialNum(lineHajikiNum);
		lineHajiki->SetColors(lineHajikiBodyColor, lineHajikiEmmisiveColor);
		lineHajiki->SetObjectName("lineHajiki");
		// 使用するモデル
		lineHajiki->SetModel(lineHajikiModelName[i]);

		hajikiManager_->AddHajiki(HajikiType::Line, std::move(lineHajiki));
	}

	/*-------------------------------------------------------------------------------------------------------------------*/
	// TargetHajiki

	//　TargetHajikiの初期座標
	const Vector3 targetHajikiInitPos[targetHajikiNum] = {
	Vector3(0.06f,-0.133f,1.0f),
	Vector3(0.06f,-0.133f,1.05f)
	};
	// 色
	const Vector4 targetHajikiColor = { 0.65f,0.0f ,0.0f ,1.0f };

	// 2 //
	for (uint32_t i = 0; i < targetHajikiNum; i++) {

		auto targetHajiki = std::make_unique<GameObject3D>(GameObjectType::Model);

		// 初期化
		targetHajiki->Initialize();
		targetHajiki->SetTranslate(targetHajikiInitPos[i]);
		targetHajiki->SetColor(targetHajikiColor);
		targetHajiki->SetObjectName("targetHajiki");
		// 使用するテクスチャとモデル
		targetHajiki->SetTexture(whiteTexName);
		targetHajiki->SetModel(targetHajikiModelName[i]);

		hajikiManager_->AddHajiki(HajikiType::Target, std::move(targetHajiki));
	}

	/*-------------------------------------------------------------------------------------------------------------------*/
	// Line

	// Lineの初期スケール
	const Vector3 lineInitScale = { 2.7f,0.5f,0.1f };

	// LineColor
	changeAlpha_ = 0.005f;
	lineColorAlpha_ = 1.0f;

	line_ = std::make_unique<GameObject3D>(GameObjectType::Model);

	// 初期化
	line_->Initialize();
	line_->SetScale(lineInitScale);

	// SRT更新
	LineUpdate();
	line_->SetObjectName("line");
	// 使用するテクスチャとモデル
	line_->SetTexture(whiteTexName);
	line_->SetModel(lineModelName);

	/*-------------------------------------------------------------------------------------------------------------------*/
	// Block

	// 初期座標
	const Vector3 blockInitPos = { -0.044f,0.136f,1.05f };
	// 色
	const Vector4 blockColor = { 0.2f,0.2f,0.2f,1.0f };
	// ブロックモデルのハーフサイズ
	const float kBlockHalfSize = 0.02f;

	for (uint32_t i = 0; i < blockNum; i++) {

		blocks_[i] = std::make_unique<GameObject3D>(GameObjectType::Model);

		// 初期化
		blocks_[i]->Initialize();
		blocks_[i]->SetTranslate(blockInitPos);
		blocks_[i]->SetColor(blockColor);
		blocks_[i]->SetHalfSize(kBlockHalfSize);
		// 使用するテクスチャとモデル
		blocks_[i]->SetObjectName("block");
		blocks_[i]->SetTexture(whiteTexName);
		blocks_[i]->SetModel(cubeModelName[i]);
	}

	/*-------------------------------------------------------------------------------------------------------------------*/
	// Kirai

	// 初期座標
	const Vector3 kiraiInitPos = { 0.043f,0.09f,1.05f };
	// 色
	const Vector4 kiraiColor = { 0.2f,0.2f,0.2f,1.0f };
	// 機雷モデルのハーフサイズ
	const float kKiraiHalfSize = 0.018f;

	for (uint32_t i = 0; i < kiraiNum; i++) {

		kiraies_[i] = std::make_unique<GameObject3D>(GameObjectType::Model);

		// 初期化
		kiraies_[i]->Initialize();
		kiraies_[i]->SetTranslate(kiraiInitPos);
		kiraies_[i]->SetColor(kiraiColor);
		kiraies_[i]->SetHalfSize(kKiraiHalfSize);
		kiraies_[i]->SetObjectName("kirai");
		kiraies_[i]->SetTexture(whiteTexName);
		kiraies_[i]->SetModel(kiraiModelName[i]);
	}

	// 回転
	rotate_.Initialize();
	rotate_.x = std::numbers::pi_v<float> / 6.0f;

	/*-------------------------------------------------------------------------------------------------------------------*/
	// Area

	area_ = std::make_unique<GameObject3D>(GameObjectType::Triangle);

	// 初期化
	area_->Initialize();
	area_->SetObjectName("area");
	area_->SetTexture(whiteTexName);

	/*-------------------------------------------------------------------------------------------------------------------*/
	// HajikiManagerにオブジェクトのセット

	// Block
	for (const auto& block : blocks_) {

		hajikiManager_->SetBlocks(block.get());
	}

	// Kirai
	for (const auto& kirai : kiraies_) {

		hajikiManager_->SetKiraies(kirai.get());
	}

	// 初期化
	hajikiManager_->Initialize();

	/*-------------------------------------------------------------------------------------------------------------------*/
	// ImGuiセット

	imgui_.Set(hajikiManager_->GetHajiki(HajikiType::Player, Imaginary).object.get());
	// 2つ
	for (uint32_t i = 0; i < 2; i++) {
		imgui_.Set(hajikiManager_->GetHajiki(HajikiType::Line, i).object.get());
		imgui_.Set(hajikiManager_->GetHajiki(HajikiType::Target, i).object.get());
	}

	for (const auto& block : blocks_) {

		imgui_.Set(block.get());
	}
	for (const auto& kirai : kiraies_) {

		imgui_.Set(kirai.get());
	}
	imgui_.Set(area_.get());

}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Update() {

	// クリア判定更新
	isClear_ = hajikiManager_->IsClear();

	/*======================================================*/
	// ImGui

	ImGui::Begin("GameScene");
	ImGui::Text("SoloMode");
	ImGui::End();

	imgui_.Render();

	/*======================================================*/
	// 3Dオブジェクト

	if (mode_ == ApplicationMode::GAME3D) {

		// マウス移動
		hajikiManager_->MouseMove(HajikiType::Player);
	} else if (mode_ == ApplicationMode::AR) {

		// AR移動
		hajikiManager_->ARMove();
	}

	// Kirai更新
	KiraiUpdate();

	// Line更新
	LineUpdate();

	// エリア更新
	UpdateArea();

	/*======================================================*/
	// 衝突判定

	hajikiManager_->CollisionUpdate();
	// 更新
	hajikiManager_->Update();
}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Draw() {

	///*======================================================*/
	// 2Dオブジェクト



	///*======================================================*/
	// 3Dオブジェクト

	// Hajiki
	hajikiManager_->Draw();

	// Line
	line_->Draw();

	// Block
	for (const auto& block : blocks_) {

		// 消滅したブロックは描画しない
		if (block->GetIsHit()) {

			continue;
		}

		block->Draw();
	}

	// Kirai
	for (const auto& kirai : kiraies_) {

		kirai->Draw();
	}

	///*======================================================*/
	// エリアの描画

	DrawArea();
}

/*////////////////////////////////////////////////////////////////////////////////
*								  Kirai更新処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::KiraiUpdate() {

	// Y軸回転
	rotate_.y += std::numbers::pi_v<float> / 45.0f;

	for (const auto& kirai : kiraies_) {

		kirai->SetRotate({ kirai->GetRotate().x,rotate_.y,kirai->GetRotate().z });
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								  Line更新処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::LineUpdate() {

	/*======================================================*/
	// 色の更新

	lineColorAlpha_ += changeAlpha_;

	if (lineColorAlpha_ >= 0.3f) {

		lineColorAlpha_ = 0.3f;
		// 減少に切り替え
		changeAlpha_ = -changeAlpha_;
	} else if (lineColorAlpha_ <= 0.1f) {

		lineColorAlpha_ = 0.1f;
		// 増加に切り替え
		changeAlpha_ = -changeAlpha_;
	}

	// 色のセット
	line_->SetColor({ 0.0f,0.0f,0.0f,lineColorAlpha_ });

	/*======================================================*/
	// SRTの更新

	// デフォルトサイズ
	const float kPlaneDefaultSize = 10.0f;

	// カーテンを貼る対象の座標
	Vector2 posA =
	{ hajikiManager_->GetHajiki(HajikiType::Line,0).object->GetCenterPos().x,
		hajikiManager_->GetHajiki(HajikiType::Line,0).object->GetCenterPos().y };
	Vector2 posB =
	{ hajikiManager_->GetHajiki(HajikiType::Line,1).object->GetCenterPos().x,
		hajikiManager_->GetHajiki(HajikiType::Line,1).object->GetCenterPos().y };

	// 間の中心座標
	Vector2 centerPos = posA + posB;
	centerPos = { centerPos.x / 2.0f,centerPos.y / 2.0f };
	line_->SetTranslate({ centerPos.x, centerPos.y,1.05f });

	// scaleの計算
	float distance = Vector2::Length(posB - posA);
	distance = distance * kPlaneDefaultSize;
	Vector3 scale = { distance,line_->GetScale().y,line_->GetScale().z };
	line_->SetScale(scale);

	// rotateの計算
	Vector2 direction = posB - posA;
	direction = Vector2::Normalize(direction);
	float zAngle = std::atan2(direction.y, direction.x);
	Vector3 rotate = { std::numbers::pi_v<float> / 2.0f,0.0f,zAngle };
	line_->SetRotate(rotate);
}

/*////////////////////////////////////////////////////////////////////////////////
*									エリア用関数
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::UpdateArea() {

	if (hajikiManager_->CheckAllHajikiStop()) {
		isShowArea = false;
	} else if (hajikiManager_->GetHajiki(HajikiType::Player, Imaginary).object->GetIsPass()) {
		isShowArea = true;
	}

	if (hajikiManager_->IsClear()) {
		isShowArea = true;
	}

	// フラグが立っていなければ早期リターン
	if (!isShowArea) {
		return;
	}

	// カメラの方向ベクトル（例として前方方向を示す）
	Vector3 cameraDirection = { 0.0f, 0.0f, -1.0f };

	// ターゲットハジキの座標を取得
	Vector3 posA = { hajikiManager_->GetHajiki(HajikiType::Line,0).object->GetCenterPos().x,
					 hajikiManager_->GetHajiki(HajikiType::Line,0).object->GetCenterPos().y,
					 hajikiManager_->GetHajiki(HajikiType::Line,0).object->GetCenterPos().z };
	Vector3 posB = { hajikiManager_->GetHajiki(HajikiType::Line,1).object->GetCenterPos().x,
					 hajikiManager_->GetHajiki(HajikiType::Line,1).object->GetCenterPos().y,
					 hajikiManager_->GetHajiki(HajikiType::Line,1).object->GetCenterPos().z };

	// プレイヤーハジキの座標を取得
	Vector3 posC = { hajikiManager_->GetHajiki(HajikiType::Player,Imaginary).object->GetCenterPos().x,
					 hajikiManager_->GetHajiki(HajikiType::Player,Imaginary).object->GetCenterPos().y,
					 hajikiManager_->GetHajiki(HajikiType::Player,Imaginary).object->GetCenterPos().z };

	// 三角形の法線ベクトルを計算
	Vector3 edge1 = posB - posA;
	Vector3 edge2 = posC - posA;
	Vector3 normal = Vector3::Normalize(Vector3::Cross(edge1, edge2));

	// 法線ベクトルがカメラの方向を向くように調整
	if (normal.x * cameraDirection.x + normal.y * cameraDirection.y + normal.z * cameraDirection.z < 0) {
		// 法線ベクトルがカメラの方向を向いていない場合、三角形の頂点を反転
		std::swap(posA, posB);
	}

	// 配列に座標を挿入
	std::array<Vector3, 3> areaTriangleVertices = { posA, posB, posC };

	// 重心を求める
	Vector3 CenterPos = (posA + posB + posC) / 3.0f;

	// エリアに座標をセット
	area_->SetTriangleVertices(areaTriangleVertices);
	area_->SetTranslate(Vector3(0.0f, 0.0f, kAreaTranslateZ));
	area_->SetColor(Vector4(0.0f, 0.8f, 1.0f, 0.4f));
}

void SoloGame::DrawArea() {

	// フラグが立っていなければ早期リターン
	if (!isShowArea) {
		return;
	}
	area_->Draw();
}