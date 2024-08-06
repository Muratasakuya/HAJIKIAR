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