#include "SoloGame.h"

#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								   初期化
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Initialize() {

	// 衝突管理
	// 生成
	collisionManager_ = std::make_unique<CollisionManager>();

	/*======================================================*/
	// 読み込み

	// debug用White
	const std::string whiteTexName = "white.png";
	NewMoon::LoadTexture("./Resources/Images/" + whiteTexName);

	// lineHajiki
	const std::string lineHajikiModelName = "hajiki.gltf";
	const std::string lineHajikiModelName2 = "hajiki2.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName2);
	// line
	const std::string lineModelName = "line.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineModelName);
	// targetHajiki Test
	const std::string targetHajikiModelName = "targetHajiki.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", targetHajikiModelName);

	/*======================================================*/
	// 3Dオブジェクト

	/*-------------------------------------------------------------------------------------------------------------------*/
	// 共通

	// 初期回転角
	float initRotateX = std::numbers::pi_v<float> / 2.0f;

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

	for (uint32_t i = 0; i < lineHajikies_.size(); i++) {

		lineHajikies_[i] = std::make_unique<GameObject3D>(GameObjectType::Model);
		lineHajikies_[i]->Initialize();
		lineHajikies_[i]->SetRotate({ initRotateX,0.0f ,0.0f });
		lineHajikies_[i]->SetTranslate(lineHajikiInitPos[i]);
		lineHajikies_[i]->SetMaterialNum(static_cast<uint32_t>(lineHajikies_.size()));
		lineHajikies_[i]->SetColors(lineHajikiBodyColor, lineHajikiEmmisiveColor);
		lineHajikies_[i]->SetObjectName("lineHajiki");
	}

	/*-------------------------------------------------------------------------------------------------------------------*/
	// Line

	// Lineの初期スケール
	const Vector3 lineInitScale = { 2.7f,0.5f,0.1f };

	line_ = std::make_unique<GameObject3D>(GameObjectType::Model);
	line_->Initialize();
	line_->SetScale(lineInitScale);
	line_->SetRotate({ initRotateX,0.0f,0.0f });
	line_->SetObjectName("line");
	// LineColor
	changeAlpha_ = 0.005f;
	lineColorAlpha_ = 1.0f;
	// SRT更新
	LineUpdate();

	/*-------------------------------------------------------------------------------------------------------------------*/
	// TargetHajiki

	//　TargetHajikiの初期座標
	const Vector3 targetHajikiInitPos = { 0.1f,0.0f,1.0f };

	targetHajiki_ = std::make_unique<GameObject3D>(GameObjectType::Model);
	targetHajiki_->Initialize();
	targetHajiki_->SetRotate({ initRotateX,0.0f ,0.0f });
	targetHajiki_->SetTranslate(targetHajikiInitPos);
	targetHajiki_->SetObjectName("targetHajiki");

	/*======================================================*/
	// Texture Modelのセット
	
	// LineHajiki
	// ModelのConstBufferが被るので今はこうしている
	lineHajikies_[0]->SetModel(lineHajikiModelName);
	lineHajikies_[1]->SetModel(lineHajikiModelName2);

	// Line
	line_->SetTexture(whiteTexName);
	line_->SetModel(lineModelName);

	// Target
	targetHajiki_->SetTexture(whiteTexName);
	targetHajiki_->SetModel(targetHajikiModelName);

	/*======================================================*/
	// Colliderセット

	// Hajikiモデルのハーフサイズ -> 衝突判定で使用する
	const float kHajikiHalfSize = 0.015f;

	// LineHajiki
	for (const auto& lineHajiki : lineHajikies_) {

		lineHajiki->SetColliderType(ColliderType::Sphere);
		lineHajiki->SetHalfSize(kHajikiHalfSize);
	}
	// TargetHajiki
	targetHajiki_->SetColliderType(ColliderType::Sphere);
	targetHajiki_->SetHalfSize(kHajikiHalfSize);

	/*======================================================*/
	// ImGuiセット

	for (const auto& lineHajiki : lineHajikies_) {

		imgui_.Set(lineHajiki.get());
	}
	imgui_.Set(line_.get());
	imgui_.Set(targetHajiki_.get());

}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Update() {

	ImGui::Begin("GameScene");
	ImGui::Text("SoloMode");
	ImGui::End();

	// セットしたオブジェクトのImGui
	imgui_.Render();

	/*======================================================*/
	// 3Dオブジェクト

	// LineHajiki
	for (const auto& lineHajiki : lineHajikies_) {

		lineHajiki->Update();
	}
	// TargetHajiki
	targetHajiki_->Update();
	if (targetHajiki_->GetIsPass()) {

		targetHajiki_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
	} else {

		targetHajiki_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	// Line更新
	LineUpdate();

	/*======================================================*/
	// 衝突判定

	collisionManager_->Reset();

	for (const auto& lineHajiki : lineHajikies_) {

		collisionManager_->AddCollider(lineHajiki.get());
	}
	collisionManager_->AddCollider(targetHajiki_.get());

	// セットしたコライダー全ての接触判定
	collisionManager_->CheckAllHitCollisions();

	// Lineの間を通ったか判定
	if (collisionManager_->PassLineCheckCollision(
		lineHajikies_[0].get(), lineHajikies_[1].get(), targetHajiki_.get())) {

		targetHajiki_->SetIsPass(true);
	} else {

		targetHajiki_->SetIsPass(false);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Draw() {

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	// LineHajiki
	for (const auto& lineHajiki : lineHajikies_) {

		lineHajiki->Draw();
	}
	// Line
	line_->Draw();
	// TargetHajiki
	targetHajiki_->Draw();

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
	line_->SetColor({ 1.0f,1.0f,1.0f,lineColorAlpha_ });

	/*======================================================*/
	// SRTの更新

	// デフォルトサイズ
	const float kPlaneDefaultSize = 10.0f;

	// カーテンを貼る対象の座標
	Vector2 posA = { lineHajikies_[0]->GetCenterPos().x,lineHajikies_[0]->GetCenterPos().y };
	Vector2 posB = { lineHajikies_[1]->GetCenterPos().x,lineHajikies_[1]->GetCenterPos().y };

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
	Vector3 rotate = { line_->GetRotate().x,line_->GetRotate().y,zAngle };
	line_->SetRotate(rotate);
}