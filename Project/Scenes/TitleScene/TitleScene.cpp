#include "TitleScene.h"

#include "ImGuiManager.h"
#include "SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
TitleScene::TitleScene() {}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
TitleScene::~TitleScene() {}


/*////////////////////////////////////////////////////////////////////////////////
*									 初期化
////////////////////////////////////////////////////////////////////////////////*/
void TitleScene::Initialize() {

	/*======================================================*/
	// テクスチャ読み込み

	// 背景画像
	const std::string bgTextureName = "background.jpg";
	NewMoon::LoadTexture("./Resources/Images/Common/Backgrounds/" + bgTextureName);

	// タイトルロゴ
	const std::string titleLogoTextureName = "titleSceneLogoType.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + titleLogoTextureName);

	// スタートの仕方表記文字
	const std::string startNotationTextureName = "titleSceneSetBitToStart.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + startNotationTextureName);

	// クレジット 今はActiveのみ
	const std::string creditTextureName = "titleSceneCredit.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + creditTextureName);

	/*======================================================*/
	// 2Dオブジェクト

	/*-------------------------------------------------------------------------------------------------------------------*/
	// 共通

	// テクスチャアンカーポイント
	const Vector2 anchorPoint = { 0.5f,0.5f };

	/*-------------------------------------------------------------------------------------------------------------------*/
	// 背景画像

	// 背景色
	const Vector4 backgroundColor = { 0.16f,0.16f,0.16f,1.0f };

	background_ = std::make_unique<GameObject2D>();
	background_->Initialize();
	background_->SetTexture(bgTextureName);
	background_->SetColor(backgroundColor);

	/*-------------------------------------------------------------------------------------------------------------------*/
	// タイトルロゴ

	// ロゴ座標 ど真ん中
	const Vector2 titleLogoPos = { 960.0f,440.0f };

	titleLogo_ = std::make_unique<GameObject2D>();
	titleLogo_->Initialize();
	titleLogo_->SetPos(titleLogoPos);
	titleLogo_->SetTexture(titleLogoTextureName);
	titleLogo_->SetAnchor(anchorPoint);
	titleLogo_->SetObjectName("titleLogo");

	/*-------------------------------------------------------------------------------------------------------------------*/
	// スタートの仕方表記文字

	// 文字座標
	const Vector2 startNotationPos = { 960.0f,660.0f };

	startNotation_ = std::make_unique<GameObject2D>();
	startNotation_->Initialize();
	startNotation_->SetPos(startNotationPos);
	startNotation_->SetTexture(startNotationTextureName);
	startNotation_->SetAnchor(anchorPoint);
	startNotation_->SetObjectName("startNotation");

	// Color
	startNotionChangeAlpha_ = 0.005f;
	startNotionColorAlpha_ = 1.0f;

	/*-------------------------------------------------------------------------------------------------------------------*/
	// クレジット

	const Vector2 creditPos = { 960.0f,900.0f };

	credit_ = std::make_unique<GameObject2D>();
	credit_->Initialize();
	credit_->SetPos(creditPos);
	credit_->SetTexture(creditTextureName);
	credit_->SetAnchor(anchorPoint);
	credit_->SetObjectName("credit");

	/*======================================================*/
	// 3Dオブジェクト



	/*======================================================*/
	// ImGuiセット

	imgui_.Set(titleLogo_.get());
	imgui_.Set(startNotation_.get());

}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void TitleScene::Update() {

	/*======================================================*/
	// ImGui

	ImGui::Begin("TitleScene");
	ImGui::Text("SpaceKey: Title -> Tutorial");
	ImGui::End();

	// セットしたオブジェクトのImGui
	imgui_.Render();

	/*======================================================*/
	// 2Dオブジェクト

	// スタートの仕方表記文字の色更新
	StartNotionUpdate();

	/*======================================================*/
	// 3Dオブジェクト

	/*======================================================*/
	// シーン遷移処理

	// Title -> Tutorial
	if (NewMoon::TriggerKey(DIK_SPACE)) {

		SceneManager::GetInstance()->ChangeScene(TUTORIAL);
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void TitleScene::Draw() {

	/*======================================================*/
	// 2Dオブジェクト

	// 背景画像
	background_->Draw();

	// タイトルロゴ
	titleLogo_->Draw();

	// スタート文字表記
	startNotation_->Draw();

	// クレジット
	credit_->Draw();

	/*======================================================*/
	// 3Dオブジェクト



}

/*////////////////////////////////////////////////////////////////////////////////
*							スタートの仕方表記文字の色更新
////////////////////////////////////////////////////////////////////////////////*/
void TitleScene::StartNotionUpdate() {

	startNotionColorAlpha_ += startNotionChangeAlpha_;

	if (startNotionColorAlpha_ >= 0.4f) {

		startNotionColorAlpha_ = 0.4f;
		// 減少に切り替え
		startNotionChangeAlpha_ = -startNotionChangeAlpha_;
	} else if (startNotionColorAlpha_ <= 0.1f) {

		startNotionColorAlpha_ = 0.1f;
		// 増加に切り替え
		startNotionChangeAlpha_ = -startNotionChangeAlpha_;
	}

	// 色のセット
	startNotation_->SetColor({ 1.0f,1.0f,1.0f,startNotionColorAlpha_ });
}