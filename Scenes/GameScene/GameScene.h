#pragma once

// Base
#include "IScene.h"

// GameObject
#include "GameObject2D.h"
#include "GameObject3D.h"

// 衝突管理
#include "CollisionManager.h"

// ImGui
#include "ImGuiRenderer.h"

// Lib
#include "OpenCV.h"

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

	static const uint32_t spriteNum_ = 2;
	std::array<std::unique_ptr<GameObject2D>, spriteNum_> sprites_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

	
	/*----------------------------------------------------------------------*/
	// OpenCV

	// ゲーム開始
	bool gameStart_;

	std::unique_ptr<OpenCV> openCV_;
	bool isCameraOpened_;

};