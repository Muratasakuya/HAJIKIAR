#include "TitleScene.h"

#include "ImGuiManager.h"
#include "SceneManager.h"
#include "OpenCV.h"

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

	// 衝突判定
	collisionManager_ = std::make_unique<CollisionManager>();

	//  AR スタート待ち時間
	waitStartTime_ = 60.0f;

	/*======================================================*/
	// テクスチャ読み込み

	// タイトルロゴ
	const std::string titleLogoTextureName = "titleSceneLogoType.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + titleLogoTextureName);

	// クレジット 今はActiveのみ
	const std::string creditTextureName = "titleSceneCredit.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + creditTextureName);

	// スタートの仕方表記文字
	std::string startNotationTextureName;

	// ARMode
	if (applicationMode_ == ApplicationMode::AR) {

		startNotationTextureName = "titleSceneSetBitToStart.png";
		NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + startNotationTextureName);
	}
	// 3DMode
	if (applicationMode_ == ApplicationMode::GAME3D) {

		startNotationTextureName = "titleSceneSetBitToStart.png";
		NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + startNotationTextureName);
	}

	/*======================================================*/
	// 2Dオブジェクト

	/*-------------------------------------------------------------------------------------------------------------------*/
	// 共通

	// テクスチャアンカーポイント
	const Vector2 anchorPoint = { 0.5f,0.5f };
	// 色
	const Vector4 textureColor = { 0.0f,0.0f,0.0f,1.0f };

	/*-------------------------------------------------------------------------------------------------------------------*/
	// タイトルロゴ

	// ロゴ座標 ど真ん中
	const Vector2 titleLogoPos = { 960.0f,440.0f };

	titleLogo_ = std::make_unique<GameObject2D>();
	titleLogo_->Initialize();
	titleLogo_->SetPos(titleLogoPos);
	titleLogo_->SetTexture(titleLogoTextureName);
	titleLogo_->SetAnchor(anchorPoint);
	titleLogo_->SetColor(textureColor);
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
	startNotation_->SetColor(textureColor);
	startNotation_->SetObjectName("startNotation");

	// Color
	startNotionChangeAlpha_ = 0.008f;
	startNotionColorAlpha_ = 1.0f;

	/*-------------------------------------------------------------------------------------------------------------------*/
	// クレジット

	const Vector2 creditPos = { 960.0f,900.0f };

	credit_ = std::make_unique<GameObject2D>();
	credit_->Initialize();
	credit_->SetPos(creditPos);
	credit_->SetTexture(creditTextureName);
	credit_->SetAnchor(anchorPoint);
	credit_->SetColor(textureColor);
	credit_->SetDiffSize({ 327.0f,69.0f });
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

	// ARMode
	if (applicationMode_ == ApplicationMode::AR) {

		OpenCV* openCV = OpenCV::GetInstance();
		
		// 一定時間置かれていればスタート
		if (openCV->FoundTrackColor()) {

			waitStartTime_--;

			// 1秒以上置いてあれば次のシーンに遷移
			if (waitStartTime_ < 0.0f) {

				SceneManager::GetInstance()->ChangeScene(TUTORIAL);
			}
		} else {

			waitStartTime_ = 60.0f;
		}

	}
	// 3DMode
	if (applicationMode_ == ApplicationMode::GAME3D) {

		Vector2 mousePos = NewMoon::GetMousePos();

		if (collisionManager_->MouseToCheckCollision(mousePos, credit_.get(), credit_->GetAnchor())) {

			// 本来であればクレジットシーンに行く
			return;
		} else {

			// 左クリックスタート 基本どこでも、ただしクレジットの文字の場所以外
			if (NewMoon::PushMouseLeft()) {

				SceneManager::GetInstance()->ChangeScene(TUTORIAL);
			}
		}
	}

}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void TitleScene::Draw() {

	/*======================================================*/
	// 2Dオブジェクト

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

	if (startNotionColorAlpha_ >= 1.0f) {

		startNotionColorAlpha_ = 1.0f;
		// 減少に切り替え
		startNotionChangeAlpha_ = -startNotionChangeAlpha_;
	} else if (startNotionColorAlpha_ <= 0.25f) {

		startNotionColorAlpha_ = 0.25f;
		// 増加に切り替え
		startNotionChangeAlpha_ = -startNotionChangeAlpha_;
	}

	// 色のセット
	startNotation_->SetColor({ 0.0f,0.0f,0.0f,startNotionColorAlpha_ });
}