#pragma once

// 2D
#include "GameObject2D.h"
// 3D
#include "GameObject3D.h"

// ImGui
#include "ImGuiRenderer.h"

// BaseScene
#include "IScene.h"

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

	// stter

	void SetApplicationMode(const ApplicationMode& mode);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// ARか3Dゲーム
	ApplicationMode mode_;

	// ImGui
	ImGuiRenderer imgui_;

	// 衝突判定管理
	std::unique_ptr<CollisionManager> collisionManager_;

	// Hajiki管理
	std::unique_ptr<HajikiManager> hajikiManager_;

	// 摩擦
	Friction friction_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト

	/*----------------------------------------------------------------------*/
	// PlayerHajiki

	static const uint32_t playerHajikiNum = 2;

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
	// Block

	static const uint32_t blockNum = 5;
	std::array<std::unique_ptr<GameObject3D>, blockNum> blocks_;

	/*----------------------------------------------------------------------*/
	// Kirai

	std::unique_ptr<GameObject3D> kirai_;
	// Rotate
	Vector3 rotate_;


	//
	// 追加分
	//

	/*----------------------------------------------------------------------*/
	// Area

	std::unique_ptr<GameObject3D> area_;
	float kAreaTranslateZ = 0.0f;
	bool isShowArea = false;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	// Kirai更新
	void KiraiUpdate();

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