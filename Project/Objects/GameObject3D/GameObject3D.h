#pragma once

// NewMoon
#include "NewMoon.h"

// 構造体
#include "CBufferStructure.h"

// CollisionBase
#include "Collider.h"

// c++
#include <string>
#include <filesystem>

/*////////////////////////////////////////////////////////////////////////////////
*								GameObject3D Class
////////////////////////////////////////////////////////////////////////////////*/
class GameObject3D :
	public Collider<Vector3> {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	GameObject3D(GameObjectType type);
	~GameObject3D();

	void Initialize();
	void Update();
	void Draw();

	void ImGui();

	// 衝突コールバック関数
	void OnCollision() override;

	// setter

	void SetTexture(const std::string name);
	void SetModel(const std::string name);
	void SetPos(Vector3 pos);

	// getter

	Vector3 GetCenterPos() const override;
	GameObjectType GetType() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	/*--------------------------------------------------------------*/
	// メインオブジェクト

	// SRT
	Transform transform_;

	// マテリアル
	Material material_;

	// ライト
	PunctualLight light_;

	// ライティングフラグ
	bool enableLighting_;
	bool enableHalfLambert_;
	bool enablePhongReflection_;
	bool enableBlinnPhongReflection_;

	/*--------------------------------------------------------------*/
	// サブオブジェクト

	// オブジェクトのタイプ
	GameObjectType type_;

	// 使用するテクスチャの名前
	std::string textureName_;
	// 使用するモデルの名前
	std::string modelName_;

};