#pragma once

// 3D
#include "Object3D.h"

// Lib
#include "Matrix4x4.h"

/*////////////////////////////////////////////////////////////////////////////////
*								Camera2D Class
////////////////////////////////////////////////////////////////////////////////*/
class MainCamera2D {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void Initialize();
	void Update();
	void ImGuiDraw();

	// singleton
	static MainCamera2D* GetInstance();

	// getter

	Matrix4x4 GetViewMatrix() const;
	Matrix4x4 GetOrthoMatrix() const;

public:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	Matrix4x4 cameraMatrix_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 orthoMatrix_;

	Transform transform_;
};
