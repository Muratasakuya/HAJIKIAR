#include "SoloGame.h"

#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								   初期化
////////////////////////////////////////////////////////////////////////////////*/
void SoloGame::Initialize() {

	/*======================================================*/
	// 読み込み

	//// Player
	//const std::string playerTexName = "playerHajiki.png";
	//NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + playerTexName);
	//// Line
	//const std::string lineHajikiTexName = "lineHAJIKIActive.png";
	//NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + lineHajikiTexName);
	//// Target
	//const std::string targetTexName = "targetHAJIKIActive.png";
	//NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + targetTexName);

	// 共通モデル
	const std::string testHajikiModelName = "hajiki.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", testHajikiModelName);

	/*======================================================*/
	// 2Dオブジェクト

	// Player
	playerHajiki_ = std::make_unique<GameObject3D>(GameObjectType::Model);
	playerHajiki_->Initialize();
	playerHajiki_->SetScale({ 0.01f,0.01f,0.01f });
	playerHajiki_->SetModel(testHajikiModelName);
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
	//playerHajiki_->Draw();

}