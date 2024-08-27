#pragma once

// CollisionBase
#include "Collider.h"

// c++
#include <vector>
#include <algorithm>
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

	void CheckAllCollisions();

	// 凡庸
	bool EdgeCheckCollisionX(Collider<Vector3>* collider, float sizeX);
	bool EdgeCheckCollisionY(Collider<Vector3>* collider, float sizeY);

	// 3D
	bool SphereToSphereCheckCollision(Collider<Vector3>* colliderA, Collider<Vector3>* colliderB);
	bool AABBToAABBCheckCollision(Collider<Vector3>* colliderA, Collider<Vector3>* colliderB);

	// 2D
	bool CircleToCircleCheckCollision(Collider<Vector2>* colliderA, Collider<Vector2>* colliderB);
	bool QuadToQuadCheckCollision(Collider<Vector2>* colliderA, Collider<Vector2>* colliderB);

	// MouseTo
	// 2D 
	bool MouseToCheckCollision(const Vector2& mousePos, Collider<Vector2>* collider, float anchor);

	// HAJIKI
	bool PassLineCheckCollision(Collider<Vector3>* linePointA, Collider<Vector3>* linePointB, Collider<Vector3>* collider);
	bool SphereToBlockCheckCollision(Collider<Vector3>* sphere, Collider<Vector3>* block);
	bool CapsuleCheckCollision(const Vector3& pos0, const Vector3& pos1, const Vector3& pos2, float size0, float size1);
	bool SphereToSoulSphereCheckCollision(Collider<Vector3>* colliderA, Collider<Vector3>* colliderB);
	bool PointInTriangle(Collider<Vector3>* vertexA, Collider<Vector3>* vertexB, Collider<Vector3>* vertexC, Collider<Vector3>* point);

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

	// 左右判定
	bool LeftRightCheck(const Vector2& pos0, const Vector2& pos1, const Vector2& pos2);

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