#pragma once

// Base
#include "IScene.h"

// GameObject
#include "GameObject3D.h"

// 衝突管理
#include "CollisionManager.h"

// ImGui
#include "ImGuiRenderer.h"

// c++
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <string>
#include <numbers>

/*////////////////////////////////////////////////////////////////////////////////
*								GameScene Class
////////////////////////////////////////////////////////////////////////////////*/
class GameScene :
	public IScene {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	GameScene();
	~GameScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::unique_ptr<CollisionManager> collisionManager_;
	std::unique_ptr<ImGuiRenderer> imguiRenderer_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

	std::list<std::unique_ptr<GameObject3D>> spheres;

};