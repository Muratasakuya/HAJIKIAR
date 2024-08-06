#pragma once
#include "Vector3.h"

#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

/// <summary>
/// 4x4行列
/// </summary>
class Matrix4x4 final {
public:

	float m[4][4];

	/*-------------------------------------------------------------*/
	/// 関数
	
	// 積
	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
	// 逆行列
	static Matrix4x4 Inverse(const Matrix4x4& m);
	// 転置行列
	static Matrix4x4 Transpose(const Matrix4x4& m);
	// 単位行列
	static Matrix4x4 MakeIdentity4x4();
	// 拡縮行列
	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	// x軸回転行列
	static Matrix4x4 MakePitchMatrix(float radian);
	// y軸回転行列
	static Matrix4x4 MakeYawMatrix(float radian);
	// z軸回転行列
	static Matrix4x4 MakeRollMatrix(float radian);
	// 回転行列
	static Matrix4x4 MakeRotateMatrix(const Vector3& rotate);
	// 平行移動行列
	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	// アフィン変換
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	// 正射影行列
	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	// 透視投影行列
	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
	// ビューポート変換行列
	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
};
