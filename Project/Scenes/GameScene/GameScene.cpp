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
		// モードセット
		soloGame_->SetApplicationMode(applicationMode_);
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

	isClear_ = false;

	/*======================================================*/
	// テクスチャ

	// 波
	const std::string waveTextureName = "wave.png";
	const std::string bgTextureName = "bg.png";
	const std::string gridTextureName = "grid.png";

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

	// 波
	for (uint32_t i = 0; i < waves_.size(); i++) {

		waves_[i] = std::make_unique<GameObject2D>();
		waves_[i]->Initialize();
		waves_[i]->SetAnchor({ 0.5f,0.5f });
		waves_[i]->SetPos({ 960.0f,880.0f + i * 32.0f });
		waves_[i]->SetTexture(waveTextureName);

		waveTextureLeftTops_[i].Initialize();
		waveAlpha_[i] = 1.0f;
	}

	/*======================================================*/
	// 3Dオブジェクト



	/*======================================================*/
	// ImGuiのセット

	imgui_.Set(background_.get());
	imgui_.Set(grid_.get());
	for (const auto& wave : waves_) {
		imgui_.Set(wave.get());
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Update() {

	//　クリア判定更新
	if (gameMode_ == GameMode::SOLO) {

		isClear_ = soloGame_->IsClear();
	} else if (gameMode_ == GameMode::MATCH) {}

	/*======================================================*/
	// ImGui

	ImGui::Begin("GameScene");
	ImGui::Text("SpaceKey: Game -> Result");
	ImGui::Text("EnterKey: Game -> Tutorial");
	ImGui::End();

	NewMoon::InputImGui();

	imgui_.Render();

	/*======================================================*/
	// 2Dオブジェクト

	// 波の更新処理
	WaveUpdate();

	/*======================================================*/
	// ゲームモード

	if (gameMode_ == GameMode::SOLO) {

		soloGame_->Update();
	} else if (gameMode_ == GameMode::MATCH) {

		matchGame_->Update();
	}

	/*======================================================*/
	// シーン遷移処理

	// Game -> Result
	if (NewMoon::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->ChangeScene(RESULT);
	}

	// Game -> Tutorial
	if (NewMoon::TriggerKey(DIK_RETURN)) {

		SceneManager::GetInstance()->ChangeScene(TUTORIAL);
	}

	if (isClear_) {

		SceneManager::GetInstance()->ChangeScene(RESULT);
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Draw() {

	/*======================================================*/
	// 背景

	for (const auto& wave : waves_) {

		wave->Draw();
	}

	grid_->Draw();

	/*======================================================*/
	// ゲームモード

	if (gameMode_ == GameMode::SOLO) {

		soloGame_->Draw();
	} else if (gameMode_ == GameMode::MATCH) {

		matchGame_->Draw();
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*									波の更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::WaveUpdate() {

	for (uint32_t i = 0; i < waves_.size(); i++) {

		float movePos = 1.2f * i + 0.5f;
		waveTextureLeftTops_[i].x += movePos;
		waves_[i]->SetTextureLeftTop(waveTextureLeftTops_[i]);
	}

	// 変数の初期化
	float alphaTransition = 0.01f;
	float alphaValue = 0.0f;
	bool increasing = true;

	// フレームごとにAlpha値を更新する処理
	for (int i = 0; i < waves_.size(); ++i) {
		if (increasing) {

			waveAlpha_[i] += alphaTransition;
			if (waveAlpha_[i] >= 1.0f) {
				waveAlpha_[i] = 1.0f;
				increasing = false;
			}
		} else {
			waveAlpha_[i] -= alphaTransition;
			if (waveAlpha_[i] <= 0.0f) {
				waveAlpha_[i] = 0.0f;
				increasing = true;
			}
		}

		// 色の更新
		waves_[i]->SetColor({ 1.0f, 1.0f, 1.0f, waveAlpha_[i] });
	}
}