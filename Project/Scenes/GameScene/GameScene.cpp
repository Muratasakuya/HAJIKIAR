#include "GameScene.h"

#include "SceneManager.h"
#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::GameScene() {

	// ゲームモード
	// 生成と初期化
	if (gameMode_ == GameMode::SOLO) {

		soloGame_ = std::make_unique<SoloGame>();
		soloGame_->Initialize();
	} else if (gameMode_ == GameMode::MATCH) {

		matchGame_ = std::make_unique<MatchGame>();
		matchGame_->Initialize();
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::~GameScene() {}

/*////////////////////////////////////////////////////////////////////////////////
*								   初期化
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Initialize() {

	/*======================================================*/
	// 2Dオブジェクト

	// 背景画像
	const std::string bgTextureName = "bg.png";
	background_ = std::make_unique<GameObject2D>();
	background_->Initialize();
	background_->SetTexture(bgTextureName);

	/*======================================================*/
	// 3Dオブジェクト



}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Update() {

	/*======================================================*/
	// ImGui

	ImGui::Begin("GameScene");
	ImGui::Text("SpaceKey: Game -> Result");
	ImGui::End();

	/*======================================================*/
	// シーン遷移処理

	// Game -> Result
	if (NewMoon::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->ChangeScene(RESULT);
	}

	/*======================================================*/
	// ゲームモード

	if (gameMode_ == GameMode::SOLO) {

		soloGame_->Update();
	} else if (gameMode_ == GameMode::MATCH) {

		matchGame_->Update();
	}
	
}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Draw() {

	/*======================================================*/
	// 背景

	// 背景画像
	background_->Draw();

	/*======================================================*/
	// ゲームモード

	if (gameMode_ == GameMode::SOLO) {

		soloGame_->Draw();
	} else if (gameMode_ == GameMode::MATCH) {

		matchGame_->Draw();
	}

}