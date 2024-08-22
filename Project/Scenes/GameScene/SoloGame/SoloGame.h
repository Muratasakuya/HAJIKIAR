#pragma once

// 2D
#include "GameObject2D.h"
// 3D
#include "GameObject3D.h"

// ImGui
#include "ImGuiRenderer.h"

// 衝突判定
#include "CollisionManager.h"

// Hajiki管理
#include "HajikiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								SoloGame Class
////////////////////////////////////////////////////////////////////////////////*/
class SoloGame {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	SoloGame() = default;
	~SoloGame() = default;

	void Initialize();
	void Update();
	void Draw();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// 衝突判定管理
	std::unique_ptr<CollisionManager> collisionManager_;

	// Hajiki管理
	std::unique_ptr<HajikiManager> hajikiManager_;

	// マウス座標
	Vector2 mousePos_;
	// クリックした座標
	Vector2 mousePressPos_;
	// クリックしているか
	bool isPressMouse_;
	// Playerを動かすのに使うカウント
	uint32_t playerMoveCount_;

	// 摩擦
	Friction friction_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

	/*----------------------------------------------------------------------*/
	// LineHajiki

	static const uint32_t lineHajikiNum = 2;

	/*----------------------------------------------------------------------*/
	// Line

	std::unique_ptr<GameObject3D> line_;
	// Color
	float changeAlpha_;
	float lineColorAlpha_;

	/*----------------------------------------------------------------------*/
	// TargetHajiki

	static const uint32_t targetHajikiNum = 2;

	/*----------------------------------------------------------------------*/
	// Cube

	std::unique_ptr<GameObject3D> cube_;

	/*----------------------------------------------------------------------*/
	// Kirai

	std::unique_ptr<GameObject3D> kirai_;


	//
	// 追加分
	//

	/*----------------------------------------------------------------------*/
	// Area

	std::unique_ptr<GameObject3D> area_;
	float kAreaTranslateZ = 0.0f;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	// Line更新
	void LineUpdate();

	/*-----------------------------*/
	///			Area用関数
	/*-----------------------------*/

	// エリア更新
	void UpdateArea();

	// エリア描画
	void DrawArea();


};