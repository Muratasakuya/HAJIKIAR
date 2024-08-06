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

	currentSceneNo_ = TITLE;
	currentScene_ = static_cast<std::unique_ptr<IScene>>(sceneFactory_.CreateScene(currentSceneNo_));
	currentScene_->Initialize();
}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
SceneManager::~SceneManager() {

	currentScene_.reset();
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

	inputManager_ = InputManager::GetInstance();

	// ウィンドウのxボタンが押されるまでループ
	while (Engine::ProcessMessage() == 0) {
		// フレームの開始
		Engine::BeginFrame();

		// キーの入力状態取得
		std::memcpy(inputManager_->GetPreKeys(), inputManager_->GetKeys(), inputKeyMaxNum);
		Engine::GetHitKeyStateAll(inputManager_->GetKeys());

		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = currentScene_->GetSceneNo();

		// 更新 描画
		currentScene_->Update();
		currentScene_->Draw();

		// フレームの終了
		Engine::EndFrame();
	}

}