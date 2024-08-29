#include "Vector2.h"

/*-------------------------------------------------------------*/
/// 算術演算子

// +
Vector2 Vector2::operator+(const Vector2& other) const {
	return { x + other.x, y + other.y };
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

// float*
Vector2 Vector2::operator*(float scalar) const {

	return Vector2(x * scalar, y * scalar);
}
Vector2 operator*(float scalar, const Vector2& v) {

	return Vector2(v.x * scalar, v.y * scalar);
}
// float/
Vector2 Vector2::operator/(float scalar) const {

	return Vector2(x / scalar, y / scalar);
}
Vector2 operator/(float scalar, const Vector2& v) {

	return Vector2(v.x / scalar, v.y / scalar);
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

bool Vector2::operator>(const Vector2& other) const {

	return x > other.x && y > other.y;
}

bool Vector2::operator<(const Vector2& other) const {

	return x < other.x && y < other.y;
}

/*-------------------------------------------------------------*/
/// 関数

// 0.0f初期化
void Vector2::Initialize() {

	this->x = 0.0f;
	this->y = 0.0f;
}

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

// 絶対値
Vector2 Vector2::fabs(const Vector2& v) {

	return Vector2(std::fabs(v.x), std::fabs(v.y));
}

float Vector2::Cross(const Vector2& v1, const Vector2& v2) {

	return v1.x * v2.y - v1.y * v2.x;;
}
