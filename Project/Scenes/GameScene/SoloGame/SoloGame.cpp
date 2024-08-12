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
	// Target
	const std::string targetTexName = "targetHAJIKIActive.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + targetTexName);

	/*======================================================*/
	// 2Dオブジェクト

	// Player
	playerHajiki_ = std::make_unique<GameObject2D>();
	playerHajiki_->Initialize();
	playerHajiki_->SetPos({ 512.0f,320.0f });
	playerHajiki_->SetTexture(playerTexName);

	// Line
	for (uint32_t i = 0; i < lineHajikies_.size(); i++) {

		lineHajikies_[i] = std::make_unique<GameObject2D>();
		lineHajikies_[i]->Initialize();
		lineHajikies_[i]->SetPos({ 568.0f,568.0f + i * 256.0f });
		lineHajikies_[i]->SetTexture(lineHajikiTexName);
	}

	// target
	targetHajiki_ = std::make_unique<GameObject2D>();
	targetHajiki_->Initialize();
	targetHajiki_->SetPos({ 720.0f,720.0f });
	targetHajiki_->SetTexture(targetTexName);

	// ImGuiセット
	imgui_.Set(playerHajiki_.get());
	imgui_.Set(targetHajiki_.get());
	for (const auto& lineHajiki : lineHajikies_) {
		imgui_.Set(lineHajiki.get());
	}
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