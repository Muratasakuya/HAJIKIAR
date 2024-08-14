#include "SoloGame.h"

#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								   初期化
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Initialize() {

	/*======================================================*/
	// 読み込み

	// Target
	const std::string targetTexName = "white.png";
	NewMoon::LoadTexture("./Resources/Images/" + targetTexName);

	// 共通モデル
	const std::string testHajikiModelName = "hajiki.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", testHajikiModelName);

	/*======================================================*/
	// 2Dオブジェクト

	// Player
	playerHajiki_ = std::make_unique<GameObject3D>(GameObjectType::Model);
	playerHajiki_->Initialize();
	playerHajiki_->SetScale({ 0.05f,0.05f ,0.05f });
	playerHajiki_->SetRotate({ std::numbers::pi_v<float> / 2.0f,0.0f ,0.0f });
	playerHajiki_->SetPos({ 0.0f,0.0f,0.025f });
	playerHajiki_->SetModel(testHajikiModelName);
	playerHajiki_->SetTexture(targetTexName);
	playerHajiki_->SetObjectName("playerHajiki");

	// ImGuiセット
	imgui_.Set(playerHajiki_.get());
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

}