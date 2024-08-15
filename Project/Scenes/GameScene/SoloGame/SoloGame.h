#pragma once

// 2D
#include "GameObject2D.h"
// 3D
#include "GameObject3D.h"

// ImGui
#include "ImGuiRenderer.h"

// 衝突判定
#include "CollisionManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								SoloGame Class
////////////////////////////////////////////////////////////////////////////////*/
class SoloGame {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	SoloGame() = default;
	~SoloGame() = default;

	void Initialize();
	void Update();
	void Draw();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// ImGui
	ImGuiRenderer imgui_;

	// 衝突判定管理
	std::unique_ptr<CollisionManager> collisionManager_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

	// LineHajiki
	static const uint32_t lineHajikiNum = 4;
	std::array<std::unique_ptr<GameObject3D>, lineHajikiNum> lineHajikies_;
	std::unique_ptr<GameObject3D> line_;

	// TargetHajiki (虚 imaginary)
	std::unique_ptr<GameObject3D> targetHajiki_;

};