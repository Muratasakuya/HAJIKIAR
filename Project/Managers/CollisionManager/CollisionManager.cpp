#include "CollisionManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								 コライダーリセット
////////////////////////////////////////////////////////////////////////////////*/
void CollisionManager::Reset() {

	colliders_.clear();
}

/*////////////////////////////////////////////////////////////////////////////////
*								コライダーの当たり判定
////////////////////////////////////////////////////////////////////////////////*/
void CollisionManager::CheckAllHitCollisions() {

	// 判定無し
	if (colliders_.size() <= 1) {
		return;
	}

	// リスト内のすべてのペアをチェック
	for (auto itA = colliders_.begin(); itA != colliders_.end(); ++itA) {
		for (auto itB = std::next(itA); itB != colliders_.end(); ++itB) {

			auto& [variantA, typeA] = *itA;
			auto& [variantB, typeB] = *itB;

			// 判定対象が両方Sphereのとき
			if (typeA == ColliderType::Sphere && typeB == ColliderType::Sphere) {
				if (std::holds_alternative<Collider<Vector3>*>(variantA) && std::holds_alternative<Collider<Vector3>*>(variantB)) {

					Collider<Vector3>* colliderA = std::get<Collider<Vector3>*>(variantA);
					Collider<Vector3>* colliderB = std::get<Collider<Vector3>*>(variantB);

					if (SphereToSphereCheckCollision(colliderA, colliderB)) {

						// 衝突コールバック
						colliderA->SetIsHit(true);
						colliderB->SetIsHit(true);
					} else {

						colliderA->SetIsHit(false);
						colliderB->SetIsHit(false);
					}
				}
			}

			// 判定対象が両方Circleのとき
			if (typeA == ColliderType::Circle && typeB == ColliderType::Circle) {
				if (std::holds_alternative<Collider<Vector2>*>(variantA) && std::holds_alternative<Collider<Vector2>*>(variantB)) {

					Collider<Vector2>* colliderA = std::get<Collider<Vector2>*>(variantA);
					Collider<Vector2>* colliderB = std::get<Collider<Vector2>*>(variantB);

					if (CircleToCircleCheckCollision(colliderA, colliderB)) {

						// 衝突コールバック
						colliderA->SetIsHit(true);
						colliderB->SetIsHit(true);
					} else {

						colliderA->SetIsHit(false);
						colliderB->SetIsHit(false);
					}
				}
			}
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							 線の上を通過したかの判定
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::PassLineCheckCollision(Collider<Vector3>* linePointA, Collider<Vector3>* linePointB, Collider<Vector3>* collider) {

	Vector3 posA= linePointB->GetCenterPos() - linePointA->GetCenterPos();
	Vector3 posB = collider->GetCenterPos() - linePointA->GetCenterPos();

	float cross = posA.x * posB.y - posA.y * posB.x;

	// 線分を越えているかどうかチェック
	float tolerance = 0.001f;
	if (std::fabs(cross) < tolerance) {
		
		float dot1 = posA.x * posB.x + posA.y * posB.y;
		float dot2 = posA.x * posA.x + posA.y * posA.y;

		if (dot1 >= 0 && dot1 <= dot2) {

			return true;
		}
	}

	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*								球同士の当たり判定
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::SphereToSphereCheckCollision(Collider<Vector3>* colliderA, Collider<Vector3>* colliderB) {

	// A座標
	Vector3 posA = colliderA->GetCenterPos();
	// B座標
	Vector3 posB = colliderB->GetCenterPos();

	// 差分ベクトル
	Vector3 sub = posB - posA;
	// 距離
	float distance = Vector3::Length(sub);

	if (distance <= colliderA->GetHalfSize() + colliderB->GetHalfSize()) {

		// 衝突
		return true;
	}

	// 当たっていない
	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*								AABB同士の当たり判定
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::AABBToAABBCheckCollision(Collider<Vector3>* colliderA, Collider<Vector3>* colliderB) {

	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*								  円同士の当たり判定
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::CircleToCircleCheckCollision(Collider<Vector2>* colliderA, Collider<Vector2>* colliderB) {

	// A座標
	Vector2 posA = colliderA->GetCenterPos();
	// B座標
	Vector2 posB = colliderB->GetCenterPos();

	// 差分ベクトル
	Vector2 sub = posB - posA;
	// 距離
	float distance = Vector2::Length(sub);

	if (distance <= colliderA->GetHalfSize() + colliderB->GetHalfSize()) {

		// 衝突
		return true;
	}

	// 当たっていない
	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*								 矩形同士の当たり判定
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::QuadToQuadCheckCollision(Collider<Vector2>* colliderA, Collider<Vector2>* colliderB) {

	return false;
}