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
*							マウスカーソルとの衝突判定
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::MouseToCheckCollision(const Vector2& mousePos, Collider<Vector2>* collider, float anchor) {

	// 左上アンカー
	if (anchor == 0.0f) {

		Vector2 pos = collider->GetCenterPos();
		Vector2 size = collider->GetDiffSize();

		// 左上アンカーの場合の範囲チェック
		if (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
			mousePos.y >= pos.y && mousePos.y <= pos.y + size.y) {

			return true;
		}
	}

	// 中心アンカー
	if (anchor == 0.5f) {

		Vector2 pos = collider->GetCenterPos();
		// 半分
		Vector2 size = { collider->GetDiffSize().x / 2.0f,collider->GetDiffSize().y / 2.0f };

		if (mousePos.x >= pos.x - size.x && mousePos.x <= pos.x + size.x &&
			mousePos.y >= pos.y - size.y && mousePos.y <= pos.y + size.y) {

			return true;
		}
	}

	return false;
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
*								ブロックとの衝突判定
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::SphereToBlockCheckCollision(Collider<Vector3>* sphere, Collider<Vector3>* block) {

	Vector3 spherePos = sphere->GetCenterPos();
	Vector3 blockPos = block->GetCenterPos();
	Vector3 sphereHalfSize = { sphere->GetHalfSize(),sphere->GetHalfSize() ,sphere->GetHalfSize() };
	Vector3 blockHalfSize = { block->GetHalfSize(),block->GetHalfSize() ,block->GetHalfSize() };

	if (CapsuleCheckCollision(spherePos, { blockPos.x - blockHalfSize.x,blockPos.y - blockHalfSize.y },
		{ blockPos.x + blockHalfSize.x,blockPos.y - blockHalfSize.y }, sphereHalfSize.x, 0) ||
		CapsuleCheckCollision(spherePos, { blockPos.x - blockHalfSize.x,blockPos.y + blockHalfSize.y },
			{ blockPos.x + blockHalfSize.x,blockPos.y + blockHalfSize.y }, sphereHalfSize.x, 0) ||
		CapsuleCheckCollision(spherePos, { blockPos.x + blockHalfSize.x,blockPos.y + blockHalfSize.y },
			{ blockPos.x + blockHalfSize.x,blockPos.y - blockHalfSize.y }, sphereHalfSize.x, 0) ||
		CapsuleCheckCollision(spherePos, { blockPos.x - blockHalfSize.x,blockPos.y + blockHalfSize.y },
			{ blockPos.x - blockHalfSize.x,blockPos.y - blockHalfSize.y }, sphereHalfSize.x, 0)) {

		return true;
	}
	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*								   カプセル判定
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::CapsuleCheckCollision(const Vector3& pos0, const Vector3& pos1, const Vector3& pos2, float size0, float size1) {

	Vector3 d = pos0 - pos1;
	Vector3 ab = pos2 - pos1;

	Vector3 e = { MyMath::Normalize(ab.x,MyMath::Length(ab.x,ab.y)),MyMath::Normalize(ab.y,MyMath::Length(ab.x,ab.y)) };

	float t = (d.x * e.x + d.y * e.y) / MyMath::Length(ab.x, ab.y);
	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 f = { (1.0f - t) * pos1.x + t * pos2.x,(1.0f - t) * pos1.y + t * pos2.y };

	// 距離計算
	float distance = Vector3::Length({ pos0.x - f.x, pos0.y - f.y });

	// 衝突判定
	if (distance < size0 + size1) {

		return true;
	}

	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*						球同士の当たり判定 Zを合わせる
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::SphereToSoulSphereCheckCollision(Collider<Vector3>* colliderA, Collider<Vector3>* colliderB) {

	// A座標
	Vector2 posA = { colliderA->GetCenterPos().x,colliderA->GetCenterPos().y };
	// B座標
	Vector2 posB = { colliderB->GetCenterPos().x,colliderB->GetCenterPos().y };

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
*						三角形の中にあるかどうかのチェック
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::PointInTriangle(Collider<Vector3>* vertexA, Collider<Vector3>* vertexB, Collider<Vector3>* vertexC, Collider<Vector3>* point) {

	// 三角形の各頂点の2D座標を取得
	Vector2 posA = { vertexA->GetCenterPos().x, vertexA->GetCenterPos().y };
	Vector2 posB = { vertexB->GetCenterPos().x, vertexB->GetCenterPos().y };
	Vector2 posC = { vertexC->GetCenterPos().x, vertexC->GetCenterPos().y };
	Vector2 posD = { point->GetCenterPos().x, point->GetCenterPos().y };

	// 各辺に対して外積を計算して点が三角形の内部にあるかを確認
	Vector2 AB = { posB.x - posA.x, posB.y - posA.y };
	Vector2 BC = { posC.x - posB.x, posC.y - posB.y };
	Vector2 CA = { posA.x - posC.x, posA.y - posC.y };

	Vector2 AP = { posD.x - posA.x, posD.y - posA.y };
	Vector2 BP = { posD.x - posB.x, posD.y - posB.y };
	Vector2 CP = { posD.x - posC.x, posD.y - posC.y };

	float cross1 = Vector2::Cross(AB, AP);
	float cross2 = Vector2::Cross(BC, BP);
	float cross3 = Vector2::Cross(CA, CP);

	// 全ての外積が同じ符号（すべて正またはすべて負）であれば、点は三角形の内部にある
	if ((cross1 >= 0 && cross2 >= 0 && cross3 >= 0) || (cross1 <= 0 && cross2 <= 0 && cross3 <= 0)) {
		return true;
	}

	return false;
}

/*////////////////////////////////////////////////////////////////////////////////
*						左右のどちらにいるかチェック
////////////////////////////////////////////////////////////////////////////////*/
bool CollisionManager::LeftRightCheck(const Vector2& pos0, const Vector2& pos1, const Vector2& pos2) {

	Vector2 posA = pos1 - pos0;
	Vector2 posB = pos2 - pos0;

	float cross = posA.x * posB.y - posA.y * posB.x;

	if (cross < 0) {

		return true;
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