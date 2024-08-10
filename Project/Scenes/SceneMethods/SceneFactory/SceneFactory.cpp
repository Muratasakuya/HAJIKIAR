#include "SceneFactory.h"

// Scene
#include "TitleScene.h"
#include "TutorialScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ResultScene.h"

/*////////////////////////////////////////////////////////////////////////////////
*								シーン生成
////////////////////////////////////////////////////////////////////////////////*/
IScene* SceneFactory::CreateScene(SceneNo sceneNo) {

	// シーン遷移
	switch (sceneNo) {

	case TITLE:

		return new TitleScene();
	case TUTORIAL:

		return new TutorialScene();
	case SELECT:

		return new SelectScene();
	case GAME:

		return new GameScene();
	case RESULT:

		return new ResultScene();
	}

	return nullptr;
}