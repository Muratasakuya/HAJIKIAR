#include "GameObject2D.h"

// Sceneで読み込んだテクスチャをセット
void GameObject2D::SetTexture(const std::string name) {

	std::filesystem::path textureName(name);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifier = textureName.stem().string();

	// テクスチャがあるかチェック
	Engine::CheckTextureAvailability(identifier);

	// 使用するテクスチャ名を設定
	textureName_ = identifier;
}
// 座標のセット
void GameObject2D::SetPos(Vector2 pos) {

	transform2D_.pos = pos;
}
// サイズのセット
void GameObject2D::SetSize(Vector2 size) {
	transform2D_.size = size;
}

// 中心座標取得 transform_.translate getter
Vector2 GameObject2D::GetCenterPos() const {

	return transform2D_.pos;
}
// type_ getter
GameObjectType GameObject2D::GetType() const {

	return type_;
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
	transform2D_.anchorPoint = { 0.5f,0.5f };
	// テクスチャ左上座標
	transform2D_.textureLeftTop = { 0.0f,0.0f };
	// テクスチャ切り出しサイズ
	transform2D_.textureSize = { 100.0f,100.0f };
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

	if (!GetIsHit()) {

		color_ = { 1.0f,1.0f,1.0f,1.0f };
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								   描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameObject2D::Draw() {

	Engine::DrawSprite(transform2D_, color_, textureName_, kBlendModeNormal);
}

/*////////////////////////////////////////////////////////////////////////////////
*								衝突コールバック関数
////////////////////////////////////////////////////////////////////////////////*/
void GameObject2D::OnCollision() {

	// 当たっているときは赤くする
	color_ = { 1.0f,0.0f,0.0f,1.0f };
}