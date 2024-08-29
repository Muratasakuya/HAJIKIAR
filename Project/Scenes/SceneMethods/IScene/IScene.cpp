#include "IScene.h"

// タイトルシーンで初期化
SceneNo IScene::sceneNo_ = SceneNo::TITLE;

// ゲームモード とりあえずソロ
GameMode IScene::gameMode_ = GameMode::SOLO;

// アプリケーションモード
ApplicationMode IScene::applicationMode_ = ApplicationMode::GAME3D;

ApplicationMode IScene::GetApplicationMode() const{

	return applicationMode_;
}
