#pragma once

#include "Vector2.h"
#include "Vector3.h"

#include <string>

// ゲームオブジェクトの種類
enum class GameObjectType {

	Object2D,          // 2Dオブジェクト スプライト
	PrimitiveTriangle, // テクスチャを使用しない三角形
	Triangle,          // テクスチャを使用する三角形
	Sphere,            // 球
	Model              // モデル
};

// コライダーの種類
enum class ColliderType {

	// 3D
	Sphere,  // 球同士の当たり判定
	AABB,    // AABB同士の当たり判定

	// 2D
	Circle,  // 球同士の当たり判定
	Quad     // 矩形同士の当たり判定
};

template<typename VectorType>
struct Physics {
	// 速度
	VectorType velocity;

	// 加速度
	VectorType acceleration;

	// 質量
	float mass;
};

// 摩擦
struct Friction {

	// 重力加速度
	Vector2 kGravity_{};
	// 動摩擦係数
	float miu_{};
	// 動摩擦力の大きさ
	float magnitude_{};
	// 摩擦力の働く向き
	Vector2 dirction_{};
	// 動摩擦力
	Vector2 frictionalForce_{};
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
	void SetIsPass(bool isPass);
	void SetPhysics(Physics<VectorType> physics);
	void SetVelocity(VectorType velocity);
	void SetAcceleration(VectorType acceleration);
	void SetFriction(Friction frction);
	void SetColliderType(ColliderType colliderType);
	void SetHitCollider(Collider* collider);

	// getter

	virtual VectorType GetCenterPos() const = 0;
	float GetHalfSize() const;
	bool GetIsHit() const;
	bool GetIsPass() const;
	Physics<VectorType> GetPhysics() const;
	VectorType GetVelocity() const;
	VectorType SetAcceleration() const;
	Friction GetFriction() const;
	ColliderType GetColliderType() const;
	Collider* GetHitCollider() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	float halhSize_;
	bool isHit_;
	bool isPass_;
	ColliderType colliderType_;

	Collider* hitCollider_;

	Physics<VectorType> physics_;
	Friction friction_;
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

	if (!isHit) {
		// 衝突が解除されたら相手をリセット
		hitCollider_ = nullptr;
	}
}

// isPass_ = isPass setter
template<typename VectorType>
void Collider<VectorType>::SetIsPass(bool isPass) {

	isPass_ = isPass;
}

// physics_ = physics setter
template<typename VectorType>
void Collider<VectorType>::SetPhysics(Physics<VectorType> physics) {

	physics_ = physics;
}

// physics_.velocity_ = velocity setter
template<typename VectorType>
void Collider<VectorType>::SetVelocity(VectorType velocity) {

	physics_.velocity = velocity;
}

// physics_.acceleration = acceleration setter
template<typename VectorType>
void Collider<VectorType>::SetAcceleration(VectorType acceleration) {

	physics_.acceleration = acceleration;
}

// friction_ = friction setter
template<typename VectorType>
void Collider<VectorType>::SetFriction(Friction friction) {

	friction_ = friction;
}

// colliderType_ = colliderType setter
template<typename VectorType>
void Collider<VectorType>::SetColliderType(ColliderType colliderType) {

	colliderType_ = colliderType;
}

// hitCollider_ = collider setter
template<typename VectorType>
void Collider<VectorType>::SetHitCollider(Collider* collider) {

	hitCollider_ = collider;
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

// isPass_ getter
template<typename VectorType>
bool Collider<VectorType>::GetIsPass() const {

	return isPass_;
}

// physics_ getter
template<typename VectorType>
Physics<VectorType> Collider<VectorType>::GetPhysics() const {

	return physics_;
}

// physics_.velocity_ getter
template<typename VectorType>
VectorType Collider<VectorType>::GetVelocity() const {

	return physics_.velocity;
}

//  physics_.acceleration getter
template<typename VectorType>
VectorType Collider<VectorType>::SetAcceleration() const {

	return physics_.acceleration;
}

// friction_ getter
template<typename VectorType>
Friction Collider<VectorType>::GetFriction() const {

	return friction_;
}

// colliderType_ getter
template<typename VectorType>
ColliderType Collider<VectorType>::GetColliderType() const {

	return colliderType_;
}

template<typename VectorType>
Collider<VectorType>* Collider<VectorType>::GetHitCollider() const {

	return hitCollider_;
}
///===============================================================================