#pragma once

// DirectX
#include "CBufferStructure.h"

// c++
#include <numbers>

/*////////////////////////////////////////////////////////////////////////////////
*
*								Camera3D Class
*
////////////////////////////////////////////////////////////////////////////////*/
class MainCamera3D {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void Initialize();
	void Update();
	void ImGuiDraw();

	// singleton
	static MainCamera3D* GetInstance();

	// getter

	Vector3 GetWorldPos() const;
	Matrix4x4 GetCameraMatrix() const;
	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetProjectionMatrix() const;

public:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	Matrix4x4 cameraMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;

	Transform transform_;
};
