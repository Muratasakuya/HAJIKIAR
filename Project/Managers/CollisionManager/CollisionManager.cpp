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
void CollisionManager::CheckAllCollisions() {

	// 判定無し
	if (colliders_.size() <= 1) {
		return;
	}

	// リスト内のすべてのペアをチェック
	for (auto itA = colliders_.begin(); itA != colliders_.end(); ++itA) {
		for (auto itB = std::next(itA); itB != colliders_.end(); ++itB) {

			auto& [variantA, typeA] = *itA;
			auto& [variantB, typeB] = *itB;

			if (typeA == ColliderType::Sphere && typeB == ColliderType::Sphere) {
				if (std::holds_alternative<Collider<Vector3>*>(variantA) && std::holds_alternative<Collider<Vector3>*>(variantB)) {

					Collider<Vector3>* colliderA = std::get<Collider<Vector3>*>(variantA);
					Collider<Vector3>* colliderB = std::get<Collider<Vector3>*>(variantB);

					if (SphereToSphereCheckCollision(colliderA, colliderB)) {

						colliderA->SetIsHit(true);
						colliderB->SetIsHit(true);

						// 衝突相手をセット
						colliderA->SetHitCollider(colliderB);
						colliderB->SetHitCollider(colliderA);
					}
				}
			}

			if (typeA == ColliderType::Circle && typeB == ColliderType::Circle) {
				if (std::holds_alternative<Collider<Vector2>*>(variantA) && std::holds_alternative<Collider<Vector2>*>(variantB)) {

					Collider<Vector2>* colliderA = std::get<Collider<Vector2>*>(variantA);
					Collider<Vector2>* colliderB = std::get<Collider<Vector2>*>(variantB);

					if (CircleToCircleCheckCollision(colliderA, colliderB)) {

						colliderA->SetIsHit(true);
						colliderB->SetIsHit(true);

						// 衝突相手をセット
						colliderA->SetHitCollider(colliderB);
						colliderB->SetHitCollider(colliderA);
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

	Vector3 posA = linePointB->GetCenterPos() - linePointA->GetCenterPos();
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
*								壁との当たり判定X
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::EdgeCheckCollisionX(Collider<Vector3>* collider, float sizeX) {

	// Colliderの中心位置とサイズを取得
	Vector3 pos = collider->GetCenterPos();
	float halfSize = collider->GetHalfSize();

	// 壁とのX方向の当たり判定
	if (pos.x - halfSize < -sizeX) {

		// オブジェクトが左の壁を超えた
		return true;
	} else if (pos.x + halfSize > sizeX) {

		// オブジェクトが右の壁を超えた
		return true;
	}

	// 当たっていない場合
	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*								壁との当たり判定Y
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::EdgeCheckCollisionY(Collider<Vector3>* collider, float sizeY) {

	// Colliderの中心位置とサイズを取得
	Vector3 pos = collider->GetCenterPos();
	float halfSize = collider->GetHalfSize();

	// 壁とのY方向の当たり判定
	if (pos.y - halfSize < -sizeY) {

		// オブジェクトが下の壁を超えた
		return true;
	} else if (pos.y + halfSize > sizeY) {

		// オブジェクトが上の壁を超えた
		return true;
	}

	// 当たっていない場合
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