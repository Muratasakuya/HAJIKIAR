#pragma once

// Base
#include "IScene.h"

/*////////////////////////////////////////////////////////////////////////////////
*							AbstractSceneFactory Class
////////////////////////////////////////////////////////////////////////////////*/
class AbstractSceneFactory {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	virtual ~AbstractSceneFactory() = default;

	virtual IScene* CreateScene(SceneNo sceneNo) = 0;
};