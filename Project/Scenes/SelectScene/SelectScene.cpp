#include "SelectScene.h"

#include "SceneManager.h"
#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
SelectScene::SelectScene() {}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
SelectScene::~SelectScene() {
	//openCV_->Finalize();
}

/*////////////////////////////////////////////////////////////////////////////////
*									 初期化
////////////////////////////////////////////////////////////////////////////////*/
void SelectScene::Initialize() {

	/*======================================================*/
	// テクスチャ

	// debug用White
	const std::string playerHajikiTextureName = "playerHajiki.png";
	const std::string targetHAJIKIActiveTextureName = "targetHAJIKIActive.png";

	/*======================================================*/
	// 2Dオブジェクト

	player_ = std::make_unique<GameObject2D>();
	player_->Initialize();
	player_->SetTexture(playerHajikiTextureName);

	target_ = std::make_unique<GameObject2D>();
	target_->Initialize();
	target_->SetTexture(targetHAJIKIActiveTextureName);

	/*======================================================*/
	// 3Dオブジェクト

	/*======================================================*/
	// ImGui

	imgui_.Set(player_.get());
	imgui_.Set(target_.get());

}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void SelectScene::Update() {

	/*======================================================*/
	// ImGui

	ImGui::Begin("SelectScene");
	ImGui::Text("SpaceKey: Select -> Title");
	ImGui::Text("1Key: Select -> Game(Solo)");
	ImGui::Text("2Key: Select -> Game(Match)");
	ImGui::End();

	imgui_.Render();

	/*======================================================*/
	// シーン遷移処理

	// Select -> Title
	if (NewMoon::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->ChangeScene(TITLE);
	}

	// Select -> Game(Solo)
	if (NewMoon::TriggerKey(DIK_1)) {

		gameMode_ = GameMode::SOLO;
		SceneManager::GetInstance()->ChangeScene(GAME);
	}
	// Select -> Game(Match)
	if (NewMoon::TriggerKey(DIK_2)) {

		gameMode_ = GameMode::MATCH;
		SceneManager::GetInstance()->ChangeScene(GAME);
	}

	/*======================================================*/
	// 2Dオブジェクト

	//openCV_->Update();

	//// カラー追従
	//player_->SetPos(openCV_->GetBlueCenterPos());
	//target_->SetPos(openCV_->GetGreenCenterPos());

	/*======================================================*/
	// 3Dオブジェクト



}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void SelectScene::Draw() {

	//openCV_->Draw();

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	player_->Draw();
	target_->Draw();

}