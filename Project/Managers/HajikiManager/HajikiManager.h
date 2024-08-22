#pragma once

// 衝突管理
#include "CollisionManager.h"

// ImGui
#include "ImGuiRenderer.h"

// 3D
#include "GameObject3D.h"

// c++
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

// Hajikiのタイプ
enum class HajikiType {

	Player,
	Line,
	Target,
};

// Hajiki情報
struct HajikiData {

	std::unique_ptr<GameObject3D> object;
	Physics<Vector2> physics;
	Friction friction;

	Vector2 pos{};

	uint32_t moveCount = 0;
	bool mouseMove_ = false;

	std::string name_ = "";

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

	HajikiManager();
	~HajikiManager();

	void AddHajiki(HajikiType type, std::unique_ptr<GameObject3D> obj);
	void SetImGui();
	void ImGui();

	void CollisionUpdate();

	void MouseMove(HajikiType type);

	void Draw();

	// getter

	HajikiData& GetHajiki(HajikiType type, size_t index);
	size_t GetHajikiCount(HajikiType type) const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// ImGui
	ImGuiRenderer imgui_;

	// 衝突管理
	std::unique_ptr<CollisionManager> collisionManager_;

	// 質量
	const float mass_ = 16.0f;
	// 最低速度
	const Vector2 minVelocity_ = { 0.02f ,0.02f };
	// 最高速度
	const Vector2 maxVelocity_ = { 0.5f,0.5f };

	// 全てのHajiki
	std::unordered_map<HajikiType, std::vector<HajikiData>> hajikies_;

	// マウス座標
	Vector2 mousePos_;
	// クリックした座標
	Vector2 mousePressPos_;
	// クリックしているか
	bool isPressMouse_;
	// 動かすのに使うカウント
	uint32_t moveCount_;

};