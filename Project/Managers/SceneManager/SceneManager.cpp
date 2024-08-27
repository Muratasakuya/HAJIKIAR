#include "SceneManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								singleton
////////////////////////////////////////////////////////////////////////////////*/
SceneManager* SceneManager::GetInstance() {

	static SceneManager instance;

	return &instance;
}

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
SceneManager::SceneManager() {

	// デフォルトでフルスクリーン設定
	NewMoon::SetFullScreenMode(true);

	// インスタンスの代入
	openCV_ = OpenCV::GetInstance();

	// シーン遷移
	transitionScene_ = std::make_unique<TransitionScene>();

	currentSceneNo_ = TITLE;
	currentScene_ = static_cast<std::unique_ptr<IScene>>(sceneFactory_.CreateScene(currentSceneNo_));
	currentScene_->Initialize();
}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
SceneManager::~SceneManager() {

	currentScene_.reset();
	openCV_->Finalize();
}

/*////////////////////////////////////////////////////////////////////////////////
*								   シーン遷移
////////////////////////////////////////////////////////////////////////////////*/
void SceneManager::ChangeScene(SceneNo sceneNo) {

	currentSceneNo_ = sceneNo;

	// 0.1f == waitTimeInit 遷移中に更新させないため
	if (transitionScene_->GetWaitTime() == 0.1f) {

		isTransition_ = true;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							各シーンの初期化 更新　描画
////////////////////////////////////////////////////////////////////////////////*/
void SceneManager::Run() {

	// ウィンドウのxボタンが押されるまでループ
	while (NewMoon::ProcessMessage() == 0) {
		// フレームの開始
		NewMoon::BeginFrame();

		// フルスクリーン設定 F11切り替え
		if (NewMoon::IsFullScreenMode()) {
			if (NewMoon::TriggerKey(DIK_F11)) {

				NewMoon::SetFullScreenMode(false);
			}
		} else {
			if (NewMoon::TriggerKey(DIK_F11)) {

				NewMoon::SetFullScreenMode(true);
			}
		}

		if (isTransition_) {

			transitionScene_->Fade();
			if (transitionScene_->FadeOutFinished()) {

				currentScene_ = static_cast<std::unique_ptr<IScene>>(sceneFactory_.CreateScene(currentSceneNo_));
				currentScene_->Initialize();

				isTransition_ = false;
			}
		} else {
			if (transitionScene_->FadeOutFinished() && !transitionScene_->FadeInFinished()) {

				transitionScene_->Fade();
			}
		}

		// 更新
		currentScene_->Update();

		// 描画
		currentScene_->Draw();

		// シーン遷移
		transitionScene_->Draw();

		// フレームの終了
		NewMoon::EndFrame();
	}

}