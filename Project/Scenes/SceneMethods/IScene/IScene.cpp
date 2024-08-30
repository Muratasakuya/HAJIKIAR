#include "IScene.h"

// タイトルシーンで初期化
SceneNo IScene::sceneNo_ = SceneNo::GAME;

// ゲームモード とりあえずソロ
GameMode IScene::gameMode_ = GameMode::SOLO;

// アプリケーションモード
ApplicationMode IScene::applicationMode_ = ApplicationMode::AR;

ApplicationMode IScene::GetApplicationMode() const{

	return applicationMode_;
}
