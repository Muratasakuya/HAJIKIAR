#pragma once

// Base
#include "IScene.h"

// 2D
#include "GameObject2D.h"

// Hajiki
#include "HajikiManager.h"

// ImGui
#include "ImGuiRenderer.h"

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

	ImGuiRenderer imgui_;

	// Hajiki
	std::unique_ptr<HajikiManager> hajikiManager_;

	// ステップカウント
	uint32_t stepCount_;
	// 次のステップに進めるかどうか
	bool nextStep_;
	// 次のシーンに遷移
	bool nextScene_;
	// 始めるまでのcoolTime;
	float startCoolTime_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	// 背景画像
	std::unique_ptr<GameObject2D> background_;
	std::unique_ptr<GameObject2D> grid_;

	// チュートリアル文字
	std::array<std::unique_ptr<GameObject2D>, 3> stepChar_;

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
	// Area

	std::unique_ptr<GameObject3D> area_;
	float kAreaTranslateZ = 0.0f;
	bool isShowArea = false;

	/*----------------------------------------------------------------------*/
	// Block

	static const uint32_t blockNum = 2;
	std::array<std::unique_ptr<GameObject3D>, blockNum> blocks_;

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