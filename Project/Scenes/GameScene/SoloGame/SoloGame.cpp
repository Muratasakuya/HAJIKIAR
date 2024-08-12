#include "SoloGame.h"

#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								   初期化
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Initialize() {

	/*======================================================*/
	// 読み込み

	// Player
	const std::string playerTexName = "playerHajiki.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + playerTexName);
	// Line
	const std::string lineHajikiTexName = "lineHAJIKIActive.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + lineHajikiTexName);
	const std::string lineHajkiModelName = "lineHajiki.obj";
	NewMoon::LoadModel("./Resources/Obj", lineHajkiModelName);
	// Target
	const std::string targetTexName = "targetHAJIKIActive.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + targetTexName);

	/*======================================================*/
	// 2Dオブジェクト

	// Player
	playerHajiki_ = std::make_unique<GameObject2D>();
	playerHajiki_->Initialize();
	playerHajiki_->SetPos({ 256.0f,256.0f });
	playerHajiki_->SetTexture(playerTexName);

	// Line
	for (uint32_t i = 0; i < lineHajikies_.size(); i++) {

		lineHajikies_[i] = std::make_unique<GameObject3D>(GameObjectType::Model);
		lineHajikies_[i]->Initialize();
		lineHajikies_[i]->SetPos({ 0.0f,0.0f });
		lineHajikies_[i]->SetTexture(lineHajikiTexName);
		lineHajikies_[i]->SetModel(lineHajkiModelName);
	}

	// target
	targetHajiki_ = std::make_unique<GameObject2D>();
	targetHajiki_->Initialize();
	targetHajiki_->SetPos({ 720.0f,720.0f });
	targetHajiki_->SetTexture(targetTexName);
}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Update() {

	ImGui::Begin("GameScene");
	ImGui::Text("SoloMode");
	ImGui::End();

	for (const auto& lineHajiki : lineHajikies_) {

		lineHajiki->ImGui();
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Draw() {

	/*======================================================*/
	// 2Dオブジェクト

	// Player
	playerHajiki_->Draw();
	// Line
	for (const auto& lineHajiki : lineHajikies_) {

		lineHajiki->Draw();
	}
	// Target
	targetHajiki_->Draw();
}