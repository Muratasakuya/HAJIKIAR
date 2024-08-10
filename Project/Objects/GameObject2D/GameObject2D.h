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
*								GameOject2D Class
////////////////////////////////////////////////////////////////////////////////*/
class GameObject2D :
	public Collider<Vector2> {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	GameObject2D();
	~GameObject2D();

	void Initialize();
	void Update();
	void Draw();

	// 衝突コールバック関数
	void OnCollision() override;

	// setter

	void SetTexture(const std::string name);
	void SetPos(Vector2 pos);
	void SetSize(Vector2 size);

	// getter

	Vector2 GetCenterPos() const override;
	GameObjectType GetType() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	/*--------------------------------------------------------------*/
	// メインオブジェクト

	// トランスフォーム
	Transform2D transform2D_{};

	// 色
	Vector4 color_{};

	/*--------------------------------------------------------------*/
	// サブオブジェクト

	// オブジェクトのタイプ
	const GameObjectType type_ = GameObjectType::Object2D;

	// 使用するテクスチャの名前
	std::string textureName_;

};