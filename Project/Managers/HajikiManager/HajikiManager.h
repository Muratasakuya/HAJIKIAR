#pragma once

// 衝突管理
#include "CollisionManager.h"

// ImGui
#include "ImGuiRenderer.h"

// BaseScene
#include "IScene.h"

// 3D
#include "GameObject3D.h"

// c++
#include <vector>
#include <memory>
#include <string>
#include <set>
#include <unordered_map>

// Hajikiのタイプ
enum class HajikiType {

	Player,
	Line,
	Target,
};

// 現実と虚
enum Duality {

	Reality,  // 現実
	Imaginary // 虚
};

// Hajiki情報
struct HajikiData {

	std::unique_ptr<GameObject3D> object;
	Physics<Vector2> physics;
	Friction friction;

	Vector2 pos{};
	Vector2 prePos{};

	uint32_t moveCount = 0;
	bool mouseMove_ = false;

	std::string name_ = "";

	// 魂
	bool isLeave = false;
	// 間を通ったときの強化
	bool isPower_ = false;

	HajikiData(std::unique_ptr<GameObject3D> obj, const Physics<Vector2>& phy, const Friction& fri)
		: object(std::move(obj)), physics(phy), friction(fri) {}
};

/*////////////////////////////////////////////////////////////////////////////////
*								HajikiManager Class
////////////////////////////////////////////////////////////////////////////////*/
class HajikiManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	HajikiManager() = default;
	~HajikiManager() = default;

	void AddHajiki(HajikiType type, std::unique_ptr<GameObject3D> obj);

	void CollisionUpdate();

	void MouseMove(HajikiType type);
	void ARMove();

	void Reset();

	// 全てのHajikiが止まっているか
	bool CheckAllHajikiStop();

	void Initialize();
	void Update();
	void Draw();

	// getter

	const HajikiData& GetHajiki(HajikiType type, size_t index) const;
	size_t GetHajikiCount(HajikiType type) const;
	bool IsClear() const;

	// setter

	void SetBlocks(GameObject3D* block);
	void SetKiraies(GameObject3D* kirai);
	void SetApplicationMode(const ApplicationMode& mode);
	void SetTutorialMode(bool tutorialMode);
	void SetTutorialStepCount(uint32_t tutorialStepCount);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// ARか3Dゲーム
	ApplicationMode mode_;

	// 現実と虚
	static const uint32_t dualityNum = 2;
	// 質量
	const float mass_ = 16.0f;
	// 最低速度
	const Vector2 minVelocity_ = { 0.02f ,0.02f };
	// 最高速度
	const Vector2 maxVelocity_ = { 0.5f,0.5f };
	
	const float deltaTime_ = 1.0f;

	// 閾値
	float thresholdVelocity_ = 0.01f;

	// チュートリアルモード
	bool tutorialMode_;
	uint32_t tutorialStepCount_;
	uint32_t resetCount_;

	// マウス座標
	Vector2 mousePos_;
	// クリックした座標
	Vector2 mousePressPos_;
	// クリックしているか
	bool isPressMouse_;
	// 動かすのに使うカウント
	uint32_t moveCount_;

	// 魂が離れるときの猶予
	bool isLeaveWaitPlayerSoul_;
	// 魂がまたついていく
	bool playerSoulFollow_;
	// 魂無敵時間
	float playerSoulInvincibleTime_;

	// クリア判定フラグ
	bool isClear_;

	// 動いた回数
	int moveCounter_;
	// 終了カウント
	int gameOverCount_;
	// 動いているかどうか
	bool isMove_;
	// ゲームオーバー
	bool gameOver_;

	// ゲームオーバーになったときに使う変数 ↓
	// Z座標
	float zPos_;
	float changeAlpha_;
	float alpha_;

	/*----------------------------------------------------------------------*/

	// ImGui
	ImGuiRenderer imgui_;

	// 衝突管理
	std::array< std::unique_ptr<CollisionManager>, dualityNum> collisionManagers_;

	// 全てのHajiki
	std::unordered_map<HajikiType, std::vector<HajikiData>> hajikies_;
	std::unordered_map<HajikiType, std::vector<Vector3>> hajikiesInitPos_;

	// 衝突相手のブロック
	std::vector<GameObject3D*> blocks_;
	// 衝突相手の機雷
	std::vector<GameObject3D*> kiraies_;

	// 魂が抜けた時の指標オブジェクト
	std::unique_ptr<GameObject3D> soulObject_;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	// LineHajikiの回転角の更新
	void LineHajikiUpdate();

	// 反射速度の計算
	void ReflectVelocity(HajikiData& hajiki1, HajikiData& hajiki2);

	// 速度の適応
	void ApplyVelocityAndFriction(HajikiData& hajiki);

	// 壁の反射処理
	void WallCollision(HajikiData& hajiki);

	// 間を通ったかのチェック
	void CheckPassLineCollision();

	// ブロックとの衝突処理
	void CheckBlockToHajikiCollision();

	// 機雷との衝突処理
	void CheckToHajikiCollision();

	// 魂が離れているときのPlayerの更新
	void LeaveSoulPlayerUpdate();

	// ゴール判定
	void GoalCheck();

};