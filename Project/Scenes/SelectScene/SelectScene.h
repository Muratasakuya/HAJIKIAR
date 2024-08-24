#pragma once

// 3D
#include "GameObject2D.h"

// ImGui
#include "ImGuiRenderer.h"

// OpenCV
#include "OpenCV.h"

// Base
#include "IScene.h"

/*////////////////////////////////////////////////////////////////////////////////
*								SelectScene Class
////////////////////////////////////////////////////////////////////////////////*/
class SelectScene :
	public IScene {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	SelectScene();
	~SelectScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// OpenCV
	OpenCV* openCV_ = nullptr;

	// ImGui
	ImGuiRenderer imgui_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト



	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

	std::unique_ptr<GameObject2D> player_;
	std::unique_ptr<GameObject2D> target_;

};