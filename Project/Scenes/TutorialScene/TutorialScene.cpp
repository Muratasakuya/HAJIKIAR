#include "TutorialScene.h"

#include "SceneManager.h"
#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
TutorialScene::TutorialScene() {}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
TutorialScene::~TutorialScene() {}


/*////////////////////////////////////////////////////////////////////////////////
*									 初期化
////////////////////////////////////////////////////////////////////////////////*/
void TutorialScene::Initialize() {

	/*======================================================*/
	// テクスチャ読み込み

	// 背景画像
	const std::string bgTextureName = "bg.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Backgrounds/" + bgTextureName);

	const std::string gridTextureName = "grid.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Backgrounds/" + gridTextureName);

	/*======================================================*/
	// 2Dオブジェクト

	// 背景画像
	background_ = std::make_unique<GameObject2D>();
	background_->Initialize();
	background_->SetTexture(bgTextureName);

	// 背景グリッド
	grid_ = std::make_unique<GameObject2D>();
	grid_->Initialize();
	grid_->SetTexture(gridTextureName);

	/*======================================================*/
	// 3Dオブジェクト


	imgui_.Set(grid_.get());

}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void TutorialScene::Update() {

	/*======================================================*/
	// ImGui

	ImGui::Begin("TutorialScene");
	ImGui::Text("SpaceKey: Tutorial -> Select");
	ImGui::End();

	imgui_.Render();

	/*======================================================*/
	// シーン遷移処理

	// Tutorial -> Select
	if (NewMoon::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->ChangeScene(SELECT);
	}

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト



}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void TutorialScene::Draw() {

	/*======================================================*/
	// 2Dオブジェクト

	// 背景画像
	background_->Draw();
	grid_->Draw();

	/*======================================================*/
	// 3Dオブジェクト



}