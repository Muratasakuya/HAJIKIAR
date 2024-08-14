#include "MainCamera2D.h"

#include "ImGuiManager.h"
#include "NewMoon.h"

// viewMatrix_ getter
Matrix4x4 MainCamera2D::GetViewMatrix() const {

	return viewMatrix_;
}

// orthoMatrix_ getter
Matrix4x4 MainCamera2D::GetOrthoMatrix() const {

	return orthoMatrix_;
}

/*////////////////////////////////////////////////////////////////////////////////
*								singleton
////////////////////////////////////////////////////////////////////////////////*/
MainCamera2D* MainCamera2D::GetInstance() {

	static MainCamera2D instance;

	return &instance;
}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void MainCamera2D::Initialize() {

	// アフィン
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);

	orthoMatrix_ =
		Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, NewMoon::kWindowWidthf, NewMoon::kWindowHeightf, 0.0f, 100.0f * 2.0f);
}

/*////////////////////////////////////////////////////////////////////////////////
*								  更新処理
////////////////////////////////////////////////////////////////////////////////*/
void MainCamera2D::Update() {

	cameraMatrix_ =
		Matrix4x4::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Matrix4x4::Inverse(cameraMatrix_);
}

/*////////////////////////////////////////////////////////////////////////////////
*								 ImGui描画処理
////////////////////////////////////////////////////////////////////////////////*/
void MainCamera2D::ImGuiDraw() {

	ImGui::Begin("Camera2D");

	ImGui::SliderAngle("rotateX", &transform_.rotate.x);
	ImGui::SliderAngle("rotateY", &transform_.rotate.y);
	ImGui::SliderAngle("rotateZ", &transform_.rotate.z);
	ImGui::SliderFloat3("translate", &transform_.translate.x, -20.0f, 50.0f);

	ImGui::End();
}