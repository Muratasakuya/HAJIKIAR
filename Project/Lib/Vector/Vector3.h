#pragma once

#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Vector4.h"

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 final {
public:

	float x, y, z;

	/*-------------------------------------------------------------*/
	/// 算術演算子

	// +
	Vector3 operator+(const Vector3& other) const;
	// -
	Vector3 operator-(const Vector3& other) const;
	// *
	Vector3 operator*(const Vector3& other) const;
	// /
	Vector3 operator/(const Vector3& other) const;

	// +=
	Vector3& operator+=(const Vector3& v);
	// -=
	Vector3& operator-=(const Vector3& v);

	// bool
	// 等価演算子 ==
	bool operator==(const Vector3& other) const;

	// 非等価演算子 !=
	bool operator!=(const Vector3& other) const;

	/*-------------------------------------------------------------*/
	/// 関数

	// 三角形の頂点から法線の取得
	static Vector3 CalculateTriangleNormal(const Vector4& v0, const Vector4& v1, const Vector4& v2);

	// ノルム
	static float Length(const Vector3& v);
	// 正規化
	static Vector3 Normalize(const Vector3& v);

};