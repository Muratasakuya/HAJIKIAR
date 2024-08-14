#pragma once

/// <summary>
/// 4次元ベクトル
/// </summary>
class Vector4 final {
public:

	float x, y, z, w;

	/*-------------------------------------------------------------*/
	/// 算術演算子

	// +
	Vector4 operator+(const Vector4& other) const;
	// -
	Vector4 operator-(const Vector4& other) const;
	// *
	Vector4 operator*(const Vector4& other) const;
	// /
	Vector4 operator/(const Vector4& other) const;

	// +=
	Vector4& operator+=(const Vector4& v);
	// -=
	Vector4& operator-=(const Vector4& v);

	// bool
	// 等価演算子 ==
	bool operator==(const Vector4& other) const;

	// 非等価演算子 !=
	bool operator!=(const Vector4& other) const;

	/*-------------------------------------------------------------*/
	/// 関数

};