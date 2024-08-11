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
	currentScene_ = static_cast<std::unique_ptr<IScene>>(sceneFactory_.CreateScene(currentSceneNo_));
	currentScene_->Initialize();
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

		currentSceneNo_ = currentScene_->GetSceneNo();

		// 更新 描画
		currentScene_->Update();
		currentScene_->Draw();

		// フレームの終了
		NewMoon::EndFrame();
	}

}