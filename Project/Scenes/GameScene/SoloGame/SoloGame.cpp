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

	// debug
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
		lineHajikies_[i]->SetPos({ 0.0f,0.0f,2.0f });
		lineHajikies_[i]->SetTexture(whiteTexName);
		lineHajikies_[i]->SetObjectName("lineHajiki");
		lineHajikies_[i]->SetColliderType(ColliderType::Sphere);
		lineHajikies_[i]->SetHalfSize(0.015f);
	}

	lineHajikies_[0]->SetPos({ -0.2f,0.04f,1.0f });
	lineHajikies_[2]->SetPos({ -0.1f,-0.04f,1.0f });

	lineHajikies_[0]->SetModel(lineHajikiModelName);
	lineHajikies_[1]->SetModel(lineHajikiModelName2);
	lineHajikies_[2]->SetModel(lineHajikiModelName3);
	lineHajikies_[3]->SetModel(lineHajikiModelName4);

	// Line
	line_ = std::make_unique<GameObject3D>(GameObjectType::Model);
	line_->Initialize();
	line_->SetRotate({ std::numbers::pi_v<float> / 2.0f,0.0f,0.0f });
	line_->SetPos({ 0.0f,0.0f,1.0f });
	line_->SetTexture(whiteTexName);
	line_->SetModel(lineModelName);
	line_->SetObjectName("line");

	// TargetHajiki
	targetHajiki_ = std::make_unique<GameObject3D>(GameObjectType::Model);
	targetHajiki_->Initialize();
	targetHajiki_->SetRotate({ std::numbers::pi_v<float> / 2.0f,0.0f ,0.0f });
	targetHajiki_->SetPos({ 0.1f,0.0f,2.0f });
	targetHajiki_->SetTexture(whiteTexName);
	targetHajiki_->SetModel(targetHajikiModelName);
	targetHajiki_->SetColliderType(ColliderType::Sphere);
	targetHajiki_->SetHalfSize(0.015f);

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
	lineHajikies_[1]->SetPos({ lineHajikies_[0]->GetCenterPos().x,lineHajikies_[0]->GetCenterPos().y,lineHajikies_[1]->GetCenterPos().z });
	lineHajikies_[3]->SetPos({ lineHajikies_[2]->GetCenterPos().x,lineHajikies_[2]->GetCenterPos().y,lineHajikies_[3]->GetCenterPos().z });

	// LineHajiki
	for (const auto& lineHajiki : lineHajikies_) {

		lineHajiki->Update();
	}
	// TargetHajiki
	targetHajiki_->Update();

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