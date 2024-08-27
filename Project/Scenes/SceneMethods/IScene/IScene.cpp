#include "IScene.h"

// sceneNo_ getter
SceneNo IScene::GetSceneNo() const {

	return sceneNo_;
}

// タイトルシーンで初期化
SceneNo IScene::sceneNo_ = SceneNo::TITLE;

// ゲームモード とりあえずソロ
GameMode IScene::gameMode_ = GameMode::SOLO;

// アプリケーションモード
ApplicationMode IScene::applicationMode_ = ApplicationMode::AR;