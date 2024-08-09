#pragma once

#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// 2次元ベクトル
/// </summary>
class Vector2 final {
public:

	float x, y;

	/*-------------------------------------------------------------*/
	/// 算術演算子

	// +
	Vector2 operator+(const Vector2& other) const;
	// -
	Vector2 operator-(const Vector2& other) const;
	// *
	Vector2 operator*(const Vector2& other) const;
	// /
	Vector2 operator/(const Vector2& other) const;

	// +=
	Vector2& operator+=(const Vector2& v);
	// -=
	Vector2& operator-=(const Vector2& v);

	// bool
	// 等価演算子 ==
	bool operator==(const Vector2& other) const;

	// 非等価演算子 !=
	bool operator!=(const Vector2& other) const;

	/*-------------------------------------------------------------*/
	/// 関数

	// ノルム
	static float Length(const Vector2& v);
	// 正規化
	static Vector2 Normalize(const Vector2& v);

};

