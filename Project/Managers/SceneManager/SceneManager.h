#pragma once

// Engine
#include "NewMoon.h"

// Base
#include "IScene.h"
// SceneMethod
#include "SceneFactory.h"
#include "TransitionScene.h"

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
	std::unique_ptr<TransitionScene> transitionScene_;
	SceneFactory sceneFactory_;

	SceneNo currentSceneNo_{};

	bool isTransition_ = false;

};