#pragma once

// Base
#include "IScene.h"

// 2D
#include "GameObject2D.h"

// ImGui
#include "ImGuiRenderer.h"

// c++
#include <memory>

/*////////////////////////////////////////////////////////////////////////////////
*								TitleScene Class
////////////////////////////////////////////////////////////////////////////////*/
class TitleScene :
	public IScene {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	TitleScene();
	~TitleScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// ImGui
	ImGuiRenderer imgui_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト
	// 
	// タイトルロゴ
	std::unique_ptr<GameObject2D> titleLogo_;

	// スタートの仕方表記文字
	std::unique_ptr<GameObject2D> startNotation_;
	// Color
	float startNotionChangeAlpha_;
	float startNotionColorAlpha_;

	// クレジット
	std::unique_ptr<GameObject2D> credit_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	// スタートの仕方表記文字の色更新
	void StartNotionUpdate();

};