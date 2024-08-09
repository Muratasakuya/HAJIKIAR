#include "Vector2.h"

/*-------------------------------------------------------------*/
/// 算術演算子

// +
Vector2 Vector2::operator+(const Vector2& other) const {
	return { x + other.x, y + other.y};
}
// -
Vector2 Vector2::operator-(const Vector2& other) const {
	return { x - other.x, y - other.y };
}
// *
Vector2 Vector2::operator*(const Vector2& other) const {
	return { x * other.x, y * other.y };
}
// /
Vector2 Vector2::operator/(const Vector2& other) const {
	return { x / other.x, y / other.y };
}

// +=
Vector2& Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

// -=
Vector2& Vector2::operator-=(const Vector2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

// bool
// 等価演算子 ==
bool Vector2::operator==(const Vector2& other) const {

	return x == other.x && y == other.y;
}

// 非等価演算子 !=
bool Vector2::operator!=(const Vector2& other) const {

	return !(*this == other);
}

/*-------------------------------------------------------------*/
/// 関数

// ノルム
float Vector2::Length(const Vector2& v) {

	return sqrtf(v.x * v.x + v.y * v.y);
}

// 正規化
Vector2 Vector2::Normalize(const Vector2& v) {

	float length = Length(v);
	if (length != 0) {
		return Vector2(v.x / length, v.y / length);
	} else {

		// 値が入ってなければnullで返す
		return Vector2(0.0f, 0.0f);
	}
}