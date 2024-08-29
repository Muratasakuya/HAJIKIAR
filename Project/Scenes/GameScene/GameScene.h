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

	ImGuiRenderer imgui_;

	// クリア判定
	bool isClear_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	// 背景画像
	std::unique_ptr<GameObject2D> background_;
	std::unique_ptr<GameObject2D> grid_;

	// 波
	static const uint32_t waveNum = 3;
	std::array<std::unique_ptr<GameObject2D>, waveNum> waves_;
	// テクスチャ左上座標
	std::array<Vector2, waveNum> waveTextureLeftTops_;
	std::array<float, waveNum> waveAlpha_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト



private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	void WaveUpdate();

};