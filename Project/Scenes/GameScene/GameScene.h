#pragma once

// Base
#include "IScene.h"
// GameMode
#include "SoloGame.h"
#include "MatchGame.h"

// GameObject
#include "GameObject2D.h"
#include "GameObject3D.h"

// 衝突管理
#include "CollisionManager.h"

// ImGui
#include "ImGuiRenderer.h"

// c++
#include <memory>

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

	// ゲームモード
	std::unique_ptr<SoloGame> soloGame_;
	std::unique_ptr<MatchGame> matchGame_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	// 背景画像
	std::unique_ptr<GameObject2D> background_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト



};