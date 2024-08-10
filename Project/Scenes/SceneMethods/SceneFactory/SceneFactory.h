#pragma once

// Base
#include "AbstractSceneFactory.h"

/*////////////////////////////////////////////////////////////////////////////////
*								IScene Class
////////////////////////////////////////////////////////////////////////////////*/
class SceneFactory :
	public AbstractSceneFactory {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	IScene* CreateScene(SceneNo sceneNo) override;

};