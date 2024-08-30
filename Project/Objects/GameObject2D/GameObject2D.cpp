#include "GameObject2D.h"

#include "ImGuiManager.h"

// Sceneで読み込んだテクスチャをセット
void GameObject2D::SetTexture(const std::string name) {

	std::filesystem::path textureName(name);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifier = textureName.stem().string();

	// テクスチャがあるかチェック
	NewMoon::CheckTextureAvailability(identifier);

	// 使用するテクスチャ名を設定
	textureName_ = identifier;
	objectName_ = identifier;
}
// オブジェクトの名前のセット
void GameObject2D::SetObjectName(const std::string name) {
	objectName_ = name;
}
// 座標のセット
void GameObject2D::SetPos(Vector2 pos) {

	transform2D_.pos = pos;
}
// サイズのセット
void GameObject2D::SetSize(Vector2 size) {
	transform2D_.size = size;
}

// 色のセット
void GameObject2D::SetColor(Vector4 color) {
	color_ = color;
}

// テクスチャアンカーポイントのセット
void GameObject2D::SetAnchor(Vector2 anchor) {

	transform2D_.anchorPoint = anchor;
}

// テクスチャ左上座標のセット
void GameObject2D::SetTextureLeftTop(Vector2 textureLeftTop) {

	transform2D_.textureLeftTop = textureLeftTop;
}

// サイズ適応の有無
void GameObject2D::SetAjustSize(bool ajustSize) {

	ajustSize_ = ajustSize;
}

// 中心座標取得 transform_.translate getter
Vector2 GameObject2D::GetCenterPos() const {

	return transform2D_.pos;
}
Vector2 GameObject2D::GetPreviousPos() const{

	return prePos_;
}
Vector4 GameObject2D::GetColor() const {

	return color_;
}
float GameObject2D::GetAnchor() const {

	// 両方一緒とする
	return transform2D_.anchorPoint.x;
}
// type_ getter
GameObjectType GameObject2D::GetType() const {

	return type_;
}
// objectName_ getter
std::string GameObject2D::GetObjectName() const {

	return objectName_;
}

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameObject2D::GameObject2D() {}

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameObject2D::~GameObject2D() {}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void GameObject2D::Initialize() {

	/*- Default -*/

	// 二次元座標
	transform2D_.pos = { 0.0f,0.0f };
	// 回転
	transform2D_.rotate = 0.0f;
	//　描画サイズ 必要な場合のみセット
	transform2D_.size = { 0.0f,0.0f };
	// アンカーポイント 基本中心
	transform2D_.anchorPoint = { 0.0f,0.0f };
	// テクスチャ左上座標
	transform2D_.textureLeftTop = { 0.0f,0.0f };
	// テクスチャ切り出しサイズ
	transform2D_.textureSize = { 0.0f,0.0f };
	// フリップ設定
	transform2D_.isFlipX = false;
	transform2D_.isFlipY = false;

	// 色
	color_ = { 1.0f,1.0f,1.0f,1.0f };
}

/*////////////////////////////////////////////////////////////////////////////////
*								   更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameObject2D::Update() {

	prePos_ = transform2D_.pos;
}

/*////////////////////////////////////////////////////////////////////////////////
*								   描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameObject2D::Draw() {

	NewMoon::DrawSprite(transform2D_, color_, textureName_, ajustSize_, kBlendModeNormal);
}

/*////////////////////////////////////////////////////////////////////////////////
*								衝突コールバック関数
////////////////////////////////////////////////////////////////////////////////*/
void GameObject2D::OnCollision() {

	// 当たっているときは赤くする
	color_ = { 1.0f,0.0f,0.0f,1.0f };
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGuiの操作
////////////////////////////////////////////////////////////////////////////////*/
void GameObject2D::ImGui() {

	ImGui::Begin("NewMoon");

	ImGui::Separator();
	ImGui::ColorEdit4("color", &color_.x);
	ImGui::DragFloat2("translate", &transform2D_.pos.x, 1.0f);
	ImGui::DragFloat("rotate", &transform2D_.rotate, 0.01f);
	ImGui::DragFloat2("size", &transform2D_.size.x, 1.0f);
	ImGui::DragFloat2("anchorPoint", &transform2D_.anchorPoint.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat2("textureLeftTop", &transform2D_.textureLeftTop.x, 1.0f, 0.0f, 1000.0f);
	ImGui::DragFloat2("textureSize", &transform2D_.textureSize.x, 1.0f, 0.0f, 1000.0f);
	ImGui::Checkbox("isFlipX", &transform2D_.isFlipX);
	ImGui::Checkbox("isFlipY", &transform2D_.isFlipY);

	ImGui::End();
}