#pragma once

// Base
#include "IScene.h"

// 2D
#include "GameObject2D.h"

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

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	// 背景画像
	const std::string bgTextureName_ = "bg.png";
	const std::string bgGridTextureName_ = "grid.png";
	static const uint32_t bgNum_ = 2;
	std::array<std::unique_ptr<GameObject2D>, bgNum_> backgrounds_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト



};