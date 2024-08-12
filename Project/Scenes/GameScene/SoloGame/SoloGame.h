#pragma once

// 2D
#include "GameObject2D.h"
// 3D
#include "GameObject3D.h"

// ImGui
#include "ImGuiRenderer.h"

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

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	// Player
	std::unique_ptr<GameObject2D> playerHajiki_;
	// Line
	static const uint32_t lineHajikiNum = 2;
	std::array<std::unique_ptr<GameObject2D>, lineHajikiNum> lineHajikies_;
	// Target
	std::unique_ptr<GameObject2D> targetHajiki_;

};