#include "Vector4.h"

/*-------------------------------------------------------------*/
/// 算術演算子

// +
Vector4 Vector4::operator+(const Vector4& other) const {
	return { x + other.x, y + other.y, z + other.z,w + other.w };
}
// -
Vector4 Vector4::operator-(const Vector4& other) const {
	return { x - other.x, y - other.y, z - other.z,w - other.w };
}
// *
Vector4 Vector4::operator*(const Vector4& other) const {
	return { x * other.x, y * other.y, z * other.z,w * other.w };
}
// /
Vector4 Vector4::operator/(const Vector4& other) const {
	return { x / other.x, y / other.y, z / other.z ,w / other.w };
}

// +=
Vector4& Vector4::operator+=(const Vector4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

// -=
Vector4& Vector4::operator-=(const Vector4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

// bool
// 等価演算子 ==
bool Vector4::operator==(const Vector4& other) const {

	return x == other.x && y == other.y && z == other.z && w == other.w;
}

// 非等価演算子 !=
bool Vector4::operator!=(const Vector4& other) const {

	return !(*this == other);
}