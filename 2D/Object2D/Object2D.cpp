#include "Object2D.h"

// Base
#include "Engine.h"
#include "ImGuiManager.h"

// transform2D_.pos = pos setter
void Object2D::SetPos(const Vector2& pos) {

	transform2D_.pos = pos;
}

// transform2D_.size = size setter
void Object2D::SetSize(const Vector2& size) {

	transform2D_.size = size;
	transform2D_.textureSize = size;
}

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Object2D::Object2D() {}

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
Object2D::~Object2D() {}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void Object2D::Initialize() {

	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };

	// 二次元座標
	transform2D_.pos = { 0.0f,0.0f };
	// 回転
	transform2D_.rotate = 0.0f;
	//　描画サイズ
	transform2D_.size = { 100.0f,100.0f };
	// アンカーポイント
	transform2D_.anchorPoint = { 0.0f,0.0f };
	// テクスチャ左上座標
	transform2D_.textureLeftTop = { 0.0f,0.0f };
	// テクスチャ切り出しサイズ
	transform2D_.textureSize = { 100.0f,100.0f };
	// フリップ設定
	transform2D_.isFlipX = false;
	transform2D_.isFlipY = false;

}

/*////////////////////////////////////////////////////////////////////////////////
*								ImGuiの更新
////////////////////////////////////////////////////////////////////////////////*/
void Object2D::UpdateImGui(const std::string& name) {

	ImGui::Begin(name.c_str());

	ImGui::ColorEdit4("color", &color_.x);
	ImGui::DragFloat2("translate", &transform2D_.pos.x, 1.0f);
	ImGui::DragFloat("rotate", &transform2D_.rotate, 0.01f);
	ImGui::DragFloat2("size", &transform2D_.size.x, 1.0f);
	ImGui::DragFloat2("anchorPoint", &transform2D_.anchorPoint.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat2("textureLeftTop", &transform2D_.textureLeftTop.x, 1.0f, 0.0f, 1000.0f);
	ImGui::DragFloat2("textureSize", &transform2D_.textureSize.x, 1.0f, 0.0f,1000.0f);
	ImGui::Checkbox("isFlipX", &transform2D_.isFlipX);
	ImGui::Checkbox("isFlipY", &transform2D_.isFlipY);

	ImGui::End();

}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Object2D::Update() {


}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void Object2D::Draw(const std::string textureName, BlendMode blendMode) {

	// ファイルパスからテクスチャ名を取得
	std::filesystem::path texturePath(textureName);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifierTexture = texturePath.stem().string();
	std::string useTextureName = identifierTexture;

	Engine::DrawSprite(transform2D_, color_, useTextureName, blendMode);
}