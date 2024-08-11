#pragma once

// Base
#include "IScene.h"

// 2D
#include "GameObject2D.h"

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

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	// 背景画像
	const std::string backgroundTextureName_ = "background.jpg";
	std::unique_ptr<GameObject2D> background_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト



};