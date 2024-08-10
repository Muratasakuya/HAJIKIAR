#pragma once

// Engine
#include "NewMoon.h"

// Base
#include "IScene.h"
// SceneFactory
#include "SceneFactory.h"

// Lib
#include "OpenCV.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								SceneManager Class
////////////////////////////////////////////////////////////////////////////////*/
class SceneManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	SceneManager();
	~SceneManager();

	void Run();

	void ChangeScene(SceneNo sceneNo);

	// singleton
	static SceneManager* GetInstance();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	OpenCV* openCV_ = nullptr;

	std::unique_ptr<IScene> currentScene_;
	SceneFactory sceneFactory_;

	SceneNo currentSceneNo_{};

};