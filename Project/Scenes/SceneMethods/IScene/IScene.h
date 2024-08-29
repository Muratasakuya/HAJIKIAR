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

// AR 3Dゲーム選択
enum class ApplicationMode {

	AR,
	GAME3D
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

	ApplicationMode GetApplicationMode() const;

protected:
	/*-----------------------------*/
	///			アクセス修飾子
	/*-----------------------------*/

	// シーン
	static SceneNo sceneNo_;
	// ゲームモード
	static GameMode gameMode_;
	// アプリケーションモード
	static ApplicationMode applicationMode_;

};