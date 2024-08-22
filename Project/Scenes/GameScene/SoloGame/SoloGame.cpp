#include "SoloGame.h"

#include "ImGuiManager.h"

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

	// クリックしているか
	isPressMouse_ = false;
	// Playerを動かすのに使うカウント
	playerMoveCount_ = 0;

	/*======================================================*/
	// 読み込み

	// debug用White
	const std::string whiteTexName = "white.png";
	NewMoon::LoadTexture("./Resources/Images/" + whiteTexName);

	// lineHajiki
	const std::string lineHajikiModelName[lineHajikiNum] = { "lineHajiki.gltf" ,"lineHajiki2.gltf" };
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName[0]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName[1]);
	// line
	const std::string lineModelName = "line.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineModelName);
	// mainHajiki
	const std::string playerHajikiModelName[playerHajikiNum] = { "mainHajiki0.gltf","mainHajiki1.gltf" };
	const std::string targetHajikiModelName[targetHajikiNum] = { "mainHajiki2.gltf","mainHajiki3.gltf" };
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", playerHajikiModelName[0]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", playerHajikiModelName[1]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", targetHajikiModelName[0]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", targetHajikiModelName[1]);

	// Cube
	std::string cubeModelName[blockNum];
	cubeModelName[0] = "cube.gltf";
	for (int i = 1; i < blockNum; ++i) {
		std::ostringstream oss;
		oss << "cube" << i + 1 << ".gltf";
		cubeModelName[i] = oss.str();
	}
	for (uint32_t i = 0; i < blockNum; i++) {

		NewMoon::LoadGltfModel("./Resources/Gltf/Objects/", cubeModelName[i]);
	}

	// Kirai
	const std::string kiraiModelName = "kirai.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Objects/", kiraiModelName);

	/*======================================================*/
	// 3Dオブジェクト

	/*-------------------------------------------------------------------------------------------------------------------*/
	// PlayerHajiki

	// 初期座標
	const Vector3 playerHajikiInitPos[playerHajikiNum] = {
		Vector3(0.0f,0.0f,1.0f),
		Vector3(0.0f,0.0f,1.5f)
	};
	// 色
	const Vector4 playerHajikiColor = { 0.0f,0.0f ,0.0f ,1.0f };

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
	const Vector4 lineHajikiBodyColor = { 1.0f,1.0f,1.0f,1.0f };
	// LineHajikiEmmisiveの色 (仮)
	const Vector4 lineHajikiEmmisiveColor = { 0.698f, 0.992f, 1.0f, 1.0f };

	// LineHajikiの初期座標
	const Vector3 lineHajikiInitPos[lineHajikiNum] = {
		Vector3(-0.215f,0.087f,1.0f),
		Vector3(-0.008f,-0.093f,1.0f)
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
	Vector3(0.1f,0.0f,1.0f),
	Vector3(0.1f,0.0f,1.5f)
	};
	// 色
	const Vector4 targetHajikiColor = { 0.16f,0.16f ,0.16f ,1.0f };

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
	const Vector3 blockInitPos = { -0.044f,0.136f,1.5f };
	// 色
	const Vector4 blockColor = { 0.0f,0.0f,0.0f,1.0f };
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
	const Vector3 kiraiInitPos = { 0.043f,0.09f,1.5f };
	// 色
	const Vector4 kiraiColor = { 0.0f,0.0f,0.0f,1.0f };

	kirai_ = std::make_unique<GameObject3D>(GameObjectType::Model);

	// 初期化
	kirai_->Initialize();
	kirai_->SetTranslate(kiraiInitPos);
	kirai_->SetColor(kiraiColor);
	kirai_->SetObjectName("kirai");
	kirai_->SetTexture(whiteTexName);
	kirai_->SetModel(kiraiModelName);

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

	/*-------------------------------------------------------------------------------------------------------------------*/
	// ImGuiセット
	imgui_.Set(hajikiManager_->GetHajiki(HajikiType::Player, 0).object.get());
	// 2つ
	for (uint32_t i = 0; i < 2; i++) {
		imgui_.Set(hajikiManager_->GetHajiki(HajikiType::Line, i).object.get());
		imgui_.Set(hajikiManager_->GetHajiki(HajikiType::Target, i).object.get());
	}

	for (const auto& block : blocks_) {

		imgui_.Set(block.get());
	}
	imgui_.Set(kirai_.get());
	imgui_.Set(area_.get());

}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Update() {

	/*======================================================*/
	// ImGui

	ImGui::Begin("GameScene");
	ImGui::Text("SoloMode");
	ImGui::End();

	imgui_.Render();

	/*======================================================*/
	// 3Dオブジェクト

	// マウス移動
	hajikiManager_->MouseMove(HajikiType::Player);
	// 更新
	hajikiManager_->Update();

	// Kirai更新
	KiraiUpdate();

	// Line更新
	LineUpdate();

	// エリア更新
	UpdateArea();

	/*======================================================*/
	// 衝突判定

	hajikiManager_->CollisionUpdate();
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

		block->Draw();
	}

	// Kirai
	kirai_->Draw();

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

	kirai_->SetRotate({ kirai_->GetRotate().x,rotate_.y,kirai_->GetRotate().z });
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
	line_->SetTranslate({ centerPos.x, centerPos.y,1.0f });

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
	Vector3 posC = { hajikiManager_->GetHajiki(HajikiType::Player,0).object->GetCenterPos().x,
					 hajikiManager_->GetHajiki(HajikiType::Player,0).object->GetCenterPos().y,
					 hajikiManager_->GetHajiki(HajikiType::Player,0).object->GetCenterPos().z };

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
	area_->Draw();
}