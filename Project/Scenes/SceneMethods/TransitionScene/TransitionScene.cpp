#include "TransitionScene.h"

#include "NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
TransitionScene::TransitionScene() {

	/*======================================================*/
	// テクスチャ読み込み

	const std::string whiteTextureName = "white.png";
	NewMoon::LoadTexture("./Resources/Images/" + whiteTextureName);

	/*======================================================*/
	// フェード

	fade_ = std::make_unique<GameObject2D>();

	fade_->Initialize();
	fade_->SetAjustSize(false);
	// 最初は透明
	fade_->SetColor({ 0.0f,0.0f,0.0f,0.0f });
	fade_->SetSize({ NewMoon::kWindowWidthf ,NewMoon::kWindowHeightf });
	fade_->SetTexture(whiteTextureName);

	// 待ち時間
	fadeWaitTime_ = 0.1f;

}

/*////////////////////////////////////////////////////////////////////////////////
*									フェード
////////////////////////////////////////////////////////////////////////////////*/
void TransitionScene::Fade() {

	const float changeAlpha = 0.01f;

	// 透明 -> 不透明
	if (!fadeOutFinished_) {

		fadeAlpha_ += changeAlpha;
		if (fadeAlpha_ > 1.0f) {

			fadeAlpha_ = 1.0f;
			fadeOutFinished_ = true;
		}
	}

	// 待ち時間
	if (fadeOutFinished_) {

		fadeWaitTime_ -= NewMoon::deltaTime;

		if (fadeWaitTime_ < 0.0f) {
			// 不透明 -> 透明
			if (!fadeInFinished_) {

				fadeAlpha_ -= changeAlpha;
				if (fadeAlpha_ < 0.0f) {

					fadeAlpha_ = 0.0f;
					fadeInFinished_ = true;
				}
			}

		}
	}

	// 全てのフラグをリセット
	if (fadeInFinished_) {

		fadeWaitTime_ = 0.1f;
		fadeOutFinished_ = false;
		fadeInFinished_ = false;
	}

	fade_->SetColor({ 0.0f,0.0f,0.0f,fadeAlpha_ });

}

void TransitionScene::Draw() {

	switch (type_) {
	case TransitionType::Fade:
		/*======================================================*/
		// フェード

		fade_->Draw();

		break;
	case TransitionType::Other:
		break;
	}

}

void TransitionScene::SetType(const TransitionType& type) {

	type_ = type;
}

bool TransitionScene::FadeOutFinished() const {

	return fadeOutFinished_;
}

bool TransitionScene::FadeInFinished() const {

	return fadeInFinished_;
}

float TransitionScene::GetWaitTime() const {

	return fadeWaitTime_;
}
