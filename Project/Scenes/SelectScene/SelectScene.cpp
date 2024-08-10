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

	/*======================================================*/
	// 2Dオブジェクト



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

		SceneManager::GetInstance()->ChangeScene(GAME);
		gameMode_ = GameMode::SOLO;
	}
	// Select -> Game(Match)
	if (NewMoon::TriggerKey(DIK_2)) {

		SceneManager::GetInstance()->ChangeScene(GAME);
		gameMode_ = GameMode::MATCH;
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



}