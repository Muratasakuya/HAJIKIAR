#pragma once

// Base
#include "IScene.h"

// 2D
#include "GameObject2D.h"

#include "ImGuiRenderer.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								TutorialScene Class
////////////////////////////////////////////////////////////////////////////////*/
class TutorialScene :
	public IScene {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	TutorialScene();
	~TutorialScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	ImGuiRenderer imgui_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	// 背景画像
	std::unique_ptr<GameObject2D> background_;
	std::unique_ptr<GameObject2D> grid_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト



};