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
	std::unique_ptr<GameObject2D> background_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト



};