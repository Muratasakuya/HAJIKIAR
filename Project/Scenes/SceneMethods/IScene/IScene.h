#pragma once

// シーンの種類
enum SceneNo {

	TITLE,
	TUTORIAL,
	SELECT,
	GAME,
	RESULT
};

// ソロ マルチ選択
enum class GameMode {

	SOLO,
	MATCH
};

/*////////////////////////////////////////////////////////////////////////////////
*								IScene Class
////////////////////////////////////////////////////////////////////////////////*/
class IScene {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	IScene() {}
	virtual ~IScene() {}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// getter

	SceneNo GetSceneNo() const;

protected:
	/*-----------------------------*/
	///			アクセス修飾子
	/*-----------------------------*/

	// シーン
	static SceneNo sceneNo_;
	// ゲームモード
	static GameMode gameMode_;

};