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
SelectScene::~SelectScene() {}

/*////////////////////////////////////////////////////////////////////////////////
*									 初期化
////////////////////////////////////////////////////////////////////////////////*/
void SelectScene::Initialize() {

	// debug用White
	const std::string playerHajikiTextureName = "playerHajiki.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + playerHajikiTextureName);
	const std::string targetHAJIKIActiveTextureName = "targetHAJIKIActive.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + targetHAJIKIActiveTextureName);

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


	/*======================================================*/
	// 3Dオブジェクト



}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void SelectScene::Draw() {

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	player_->Draw();
	target_->Draw();

}