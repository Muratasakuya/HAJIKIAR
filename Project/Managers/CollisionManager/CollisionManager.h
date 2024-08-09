#pragma once

// CollisionBase
#include "Collider.h"

// c++
#include <list>
#include <utility>
#include <variant>

/*////////////////////////////////////////////////////////////////////////////////
*							CollisionManager Class
////////////////////////////////////////////////////////////////////////////////*/
class CollisionManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	CollisionManager() = default;
	~CollisionManager() = default;

	void Reset();
	template<typename VectorType>
	void AddCollider(Collider<VectorType>* collider);

	void CheckCollision();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	using ColliderVariant = std::variant<Collider<Vector2>*, Collider<Vector3>*>;
	std::list<std::pair<ColliderVariant, ColliderType>> colliders_;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	// 3D
	bool SphereToSphereCheckCollision(Collider<Vector3>* colliderA, Collider<Vector3>* colliderB);
	bool AABBToAABBCheckCollision(Collider<Vector3>* colliderA, Collider<Vector3>* colliderB);

	// 2D
	bool CircleToCircleCheckCollision(Collider<Vector2>* colliderA, Collider<Vector2>* colliderB);
	bool QuadToQuadCheckCollision(Collider<Vector2>* colliderA, Collider<Vector2>* colliderB);

};

///===============================================================================
/// template関数定義

/*////////////////////////////////////////////////////////////////////////////////
*								コライダー追加
////////////////////////////////////////////////////////////////////////////////*/
template<typename VectorType>
void CollisionManager::AddCollider(Collider<VectorType>* collider) {

	colliders_.emplace_back(collider, collider->GetColliderType());
}
///===============================================================================