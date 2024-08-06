#pragma once

// Engine
#include "Engine.h"

// Base
#include "IScene.h"

// SceneFactory
#include "SceneFactory.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*
*								SceneManager Class
*
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

	std::unique_ptr<IScene> currentScene_;
	SceneFactory sceneFactory_;

	SceneNo currentSceneNo_{};
	SceneNo prevSceneNo_{};

};