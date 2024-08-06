#include "Sphere.h"

#include "TextureManager.h"
#include "ImGuiManager.h"



/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void Sphere::Initialize() {

	// SRT
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	// Material
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = false;
	material_.uvTransform = Matrix4x4::MakeIdentity4x4();

	textureName_ = "monsterBall.png";
	// テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("./Resources/Images/" + textureName_);
}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Sphere::Update() {
#ifdef _DEBUG
	ImGui::Begin("Sphere");

	ImGui::DragFloat3("scale", &transform_.scale.x, 0.05f);
	ImGui::SliderAngle("rotatX", &transform_.rotate.x);
	ImGui::SliderAngle("rotatY", &transform_.rotate.y);
	ImGui::SliderAngle("rotatZ", &transform_.rotate.z);
	ImGui::DragFloat3("translate", &transform_.translate.x, 0.05f);

	ImGui::End();
#endif
}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void Sphere::Draw() {

	// ファイルパスからテクスチャ名を取得
	std::filesystem::path texturePath(textureName_);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifierTexture = texturePath.stem().string();
	std::string useTextureName = identifierTexture;

	Engine::DrawSphere(transform_, material_, light_, useTextureName, Normal, kBlendModeNormal);
}