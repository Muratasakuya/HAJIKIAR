#pragma once

// Base
#include "IScene.h"

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



	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト



};