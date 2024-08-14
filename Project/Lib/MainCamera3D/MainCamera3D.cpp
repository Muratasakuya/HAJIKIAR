#include "MainCamera3D.h"

#include "ImGuiManager.h"
#include "NewMoon.h"

// transform_.translate getter
Vector3 MainCamera3D::GetWorldPos() const {

	return transform_.translate;
}

// cameraMatrix_ getter
Matrix4x4 MainCamera3D::GetCameraMatrix() const {

	return cameraMatrix_;
}

// viewMatrix_ getter
Matrix4x4 MainCamera3D::GetViewMatrix() const {

	return viewMatrix_;
}

// projectionMatrix_ getter
Matrix4x4 MainCamera3D::GetProjectionMatrix() const {

	return projectionMatrix_;
}

/*////////////////////////////////////////////////////////////////////////////////
*								singleton
////////////////////////////////////////////////////////////////////////////////*/
MainCamera3D* MainCamera3D::GetInstance() {

	static MainCamera3D instance;

	return &instance;
}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void MainCamera3D::Initialize() {

	// アフィン
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	projectionMatrix_ =
		Matrix4x4::MakePerspectiveFovMatrix(0.45f * 2.0f, NewMoon::kWindowWidthf / NewMoon::kWindowHeightf, 0.1f * 2.0f, 100.0f * 2.0f);
}

/*////////////////////////////////////////////////////////////////////////////////
*								  更新処理
////////////////////////////////////////////////////////////////////////////////*/
void MainCamera3D::Update() {

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);
}

/*///////////////////////////////////////////////////////////////////////////////
*								 ImGui描画処理
////////////////////////////////////////////////////////////////////////////////*/
void MainCamera3D::ImGuiDraw() {
#ifdef _DEBUG
	ImGui::Begin("3DCamera");

	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.01f);

	ImGui::End();
#endif
}