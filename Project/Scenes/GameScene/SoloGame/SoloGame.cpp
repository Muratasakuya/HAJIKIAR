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
	const std::string lineHajikiModelName3 = "hajiki3.gltf";
	const std::string lineHajikiModelName4 = "hajiki4.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName2);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName3);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName4);
	// line
	const std::string lineModelName = "line.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineModelName);
	// targetHajiki Test
	const std::string targetHajikiModelName = "targetHajiki.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", targetHajikiModelName);

	/*======================================================*/
	// 3Dオブジェクト

	// LineHajiki
	for (uint32_t i = 0; i < lineHajikies_.size(); i++) {

		lineHajikies_[i] = std::make_unique<GameObject3D>(GameObjectType::Model);
		lineHajikies_[i]->Initialize();
		lineHajikies_[i]->SetRotate({ std::numbers::pi_v<float> / 2.0f,0.0f ,0.0f });
		lineHajikies_[i]->SetTranslate({ 0.0f,0.0f,2.0f });
		lineHajikies_[i]->SetObjectName("lineHajiki");
	}

	// 初期座標セット Zをずらす
	lineHajikies_[0]->SetTranslate({ -0.2f,0.04f,1.0f });
	lineHajikies_[2]->SetTranslate({ -0.1f,-0.04f,1.0f });
	lineHajikies_[1]->SetTranslate({ lineHajikies_[0]->GetCenterPos().x,lineHajikies_[0]->GetCenterPos().y,lineHajikies_[1]->GetCenterPos().z });
	lineHajikies_[3]->SetTranslate({ lineHajikies_[2]->GetCenterPos().x,lineHajikies_[2]->GetCenterPos().y,lineHajikies_[3]->GetCenterPos().z });

	// Line
	line_ = std::make_unique<GameObject3D>(GameObjectType::Model);
	line_->Initialize();
	line_->SetRotate({ std::numbers::pi_v<float> / 2.0f,0.0f,0.0f });
	line_->SetScale({ 1.0f,5.0f,0.1f });
	line_->SetTranslate({ 0.0f,0.0f,1.5f });
	line_->SetObjectName("line");
	// LineColor
	changeAlpha_ = 0.005f;
	lineColorAlpha_ = 1.0f;
	LineUpdate();

	// TargetHajiki
	targetHajiki_ = std::make_unique<GameObject3D>(GameObjectType::Model);
	targetHajiki_->Initialize();
	targetHajiki_->SetRotate({ std::numbers::pi_v<float> / 2.0f,0.0f ,0.0f });
	targetHajiki_->SetTranslate({ 0.1f,0.0f,2.0f });
	targetHajiki_->SetObjectName("targetHajiki");

	/*======================================================*/
	// Texture Modelのセット

	// LineHajiki
	for (const auto& lineHajiki : lineHajikies_) {

		lineHajiki->SetTexture(whiteTexName);
	}
	// ModelのConstBufferが被るので今はこうしている
	lineHajikies_[0]->SetModel(lineHajikiModelName);
	lineHajikies_[1]->SetModel(lineHajikiModelName2);
	lineHajikies_[2]->SetModel(lineHajikiModelName3);
	lineHajikies_[3]->SetModel(lineHajikiModelName4);

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

	// 座標の更新
	lineHajikies_[1]->SetTranslate({ lineHajikies_[0]->GetCenterPos().x,lineHajikies_[0]->GetCenterPos().y,lineHajikies_[1]->GetCenterPos().z });
	lineHajikies_[3]->SetTranslate({ lineHajikies_[2]->GetCenterPos().x,lineHajikies_[2]->GetCenterPos().y,lineHajikies_[3]->GetCenterPos().z });

	// LineHajiki
	for (const auto& lineHajiki : lineHajikies_) {

		lineHajiki->Update();
	}
	// TargetHajiki
	targetHajiki_->Update();

	// Line更新
	LineUpdate();

	/*======================================================*/
	// 衝突判定

	collisionManager_->Reset();

	for (const auto& lineHajiki : lineHajikies_) {

		collisionManager_->AddCollider(lineHajiki.get());
	}
	collisionManager_->AddCollider(targetHajiki_.get());

	collisionManager_->CheckCollision();
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

	// カーテンを貼る対象の座標
	Vector2 posA = { lineHajikies_[0]->GetCenterPos().x,lineHajikies_[0]->GetCenterPos().y };
	Vector2 posB = { lineHajikies_[2]->GetCenterPos().x,lineHajikies_[2]->GetCenterPos().y };

	// 間の中心座標
	Vector2 centerPos = posA + posB;
	centerPos = { centerPos.x / 2.0f,centerPos.y / 2.0f };
	line_->SetTranslate({ centerPos.x, centerPos.y,line_->GetCenterPos().z });

	// scaleの計算
	float distance = Vector2::Length(posB - posA);
	distance = 10.0f * distance;
	Vector3 scale = { distance,line_->GetScale().y,line_->GetScale().z };
	line_->SetScale(scale);

	// rotateの計算
	Vector2 direction = posB - posA;
	direction = Vector2::Normalize(direction);
	float zAngle = std::atan2(direction.y, direction.x);
	Vector3 rotate = { line_->GetRotate().x,line_->GetRotate().y,zAngle };
	line_->SetRotate(rotate);
}