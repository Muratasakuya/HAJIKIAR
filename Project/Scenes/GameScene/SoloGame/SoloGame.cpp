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
	const std::string lineHajikiModelName[lineHajikiNum] = { "hajiki.gltf" ,"hajiki2.gltf" };
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName[0]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName[1]);
	// line
	const std::string lineModelName = "line.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineModelName);
	// mainHajiki
	const std::string mainHajikiModelName = "mainHajiki.gltf";
	const std::string targetHajikiModelName[targetHajikiNum] = { "mainHajiki2.gltf","mainHajiki3.gltf" };
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", mainHajikiModelName);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", targetHajikiModelName[0]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", targetHajikiModelName[1]);

	// Cube
	const std::string cubeModelName = "cube.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Objects/", cubeModelName);

	// Kirai
	const std::string kiraiModelName = "kirai.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Objects/", kiraiModelName);

	/*======================================================*/
	// 3Dオブジェクト

	/*-------------------------------------------------------------------------------------------------------------------*/
	// PlayerHajiki

	// 初期座標
	const Vector3 playerHajikiInitPos = { 0.0f,0.0f,1.0f };

	auto playerHajiki = std::make_unique<GameObject3D>(GameObjectType::Model);

	// 初期化
	playerHajiki->Initialize();
	playerHajiki->SetTranslate(playerHajikiInitPos);
	playerHajiki->SetObjectName("playerHajiki");
	// 使用するテクスチャとモデル
	playerHajiki->SetTexture(whiteTexName);
	playerHajiki->SetModel(mainHajikiModelName);

	hajikiManager_->AddHajiki(HajikiType::Player, std::move(playerHajiki));

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
	Vector3(0.1f,0.0f,2.0f)
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
	// Cube

	cube_ = std::make_unique<GameObject3D>(GameObjectType::Model);

	// 初期化
	cube_->Initialize();
	cube_->SetObjectName("cube");
	cube_->SetTexture(whiteTexName);
	cube_->SetModel(cubeModelName);

	/*-------------------------------------------------------------------------------------------------------------------*/
	// Kirai

	kirai_ = std::make_unique<GameObject3D>(GameObjectType::Model);

	// 初期化
	kirai_->Initialize();
	kirai_->SetObjectName("kirai");
	kirai_->SetTexture(whiteTexName);
	kirai_->SetModel(kiraiModelName);

	// ImGuiセット
	hajikiManager_->SetImGui();
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

	hajikiManager_->ImGui();

	/*======================================================*/
	// 3Dオブジェクト

	// マウス移動
	hajikiManager_->MouseMove(HajikiType::Player);

	// Line更新
	LineUpdate();

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

	// Cube
	cube_->Draw();

	// Kirai
	kirai_->Draw();
}

/*////////////////////////////////////////////////////////////////////////////////
*							全てのオブジェクトの移動処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::AllObjectsMove() {}

/*////////////////////////////////////////////////////////////////////////////////
*							Player移動処理 マウス
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::PlayerMove() {}

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
*									衝突判定
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::CheckCollision() {}