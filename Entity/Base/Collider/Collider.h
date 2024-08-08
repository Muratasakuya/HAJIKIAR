#pragma once

#include "Vector2.h"
#include "Vector3.h"

// ゲームオブジェクトの種類
enum class GameObjectType {

	Object2D,          // 2Dオブジェクト スプライト
	PrimitiveTriangle, // テクスチャを使用しない三角形
	Triangle,          // テクスチャを使用する三角形
	Sphere,            // 球
	Model              // モデル
};

// コライダーの種類
enum class ColliderType{

	// 3D
	Sphere,  // 球同士の当たり判定
	AABB,    // AABB同士の当たり判定

	// 2D
	Circle,  // 球同士の当たり判定
	Quad     // 矩形同士の当たり判定
};

/*////////////////////////////////////////////////////////////////////////////////
*								Collider Class
////////////////////////////////////////////////////////////////////////////////*/
template<typename VectorType>
class Collider {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	Collider() = default;
	virtual ~Collider() = default;

	// 衝突コールバック関数
	virtual void OnCollision() = 0;

	// setter

	void SetHalfSize(float halfSize);
	void SetIsHit(bool isHit);
	void SetColliderType(ColliderType colliderType);

	// getter

	virtual VectorType GetCenterPos() const = 0;
	float GetHalfSize() const;
	bool GetIsHit() const;
	ColliderType GetColliderType() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	float halhSize_;
	bool isHit_;
	ColliderType colliderType_;

};

///===============================================================================
/// template関数定義

// radius_ = radius setter
template<typename VectorType>
void Collider<VectorType>::SetHalfSize(float halfSize) {

	halhSize_ = halfSize;
}

// isHit_ = isHit setter
template<typename VectorType>
void Collider<VectorType>::SetIsHit(bool isHit) {

	isHit_ = isHit;
}

// colliderType_ = colliderType setter
template<typename VectorType>
void Collider<VectorType>::SetColliderType(ColliderType colliderType) {

	colliderType_ = colliderType;
}

// halhSize_ getter
template<typename VectorType>
float Collider<VectorType>::GetHalfSize() const {

	return halhSize_;
}

// isHit_ getter
template<typename VectorType>
bool Collider<VectorType>::GetIsHit() const {

	return isHit_;
}

// colliderType_ getter
template<typename VectorType>
ColliderType Collider<VectorType>::GetColliderType() const {

	return colliderType_;
}
///===============================================================================