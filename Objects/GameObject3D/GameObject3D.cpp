#include "GameObject3D.h"

// Sceneで読み込んだテクスチャをセット
void GameObject3D::SetTexture(const std::string name) {

	std::filesystem::path textureName(name);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifier = textureName.stem().string();

	// テクスチャがあるかチェック
	Engine::CheckTextureAvailability(identifier);

	// 使用するテクスチャ名を設定
	textureName_ = identifier;
}
// Sceneで読み込んだモデルをセット
void GameObject3D::SetModel(const std::string name) {

	std::filesystem::path modelName(name);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifier = modelName.stem().string();

	// モデルがあるかチェック
	Engine::CheckModelAvailability(identifier);

	// 使用するテクスチャを設定
	modelName_ = identifier;
}
// 座標のセット
void GameObject3D::SetPos(Vector3 pos) {

	transform_.translate = pos;
}

// 中心座標取得 transform_.translate getter
Vector3 GameObject3D::GetCenterPos() const {

	return transform_.translate;
}

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameObject3D::GameObject3D(GameObjectType type) : type_(type) {}

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameObject3D::~GameObject3D() {

}

/*////////////////////////////////////////////////////////////////////////////////
*									初期化
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::Initialize() {

	/*- Default -*/

	// SRT
	transform_.scale = { 1.0f,1.0f,1.0f };
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { 0.0f,0.0f,0.0f };

	// マテリアル
	material_.color = { 1.0f,1.0f,1.0f,1.0f };
	material_.enableLighting = false;
	material_.uvTransform = Matrix4x4::MakeIdentity4x4();

	// ライト
	light_.Initialize();
}

/*////////////////////////////////////////////////////////////////////////////////
*								   更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::Update() {

	if (!GetIsHit()) {

		material_.color = { 1.0f,1.0f,1.0f,1.0f };
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								   描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::Draw() {

	switch (type_) {
		// テクスチャなし三角形
	case GameObjectType::PrimitiveTriangle:

		Engine::DrawTriangle(transform_, material_, light_, textureName_, Primitive, kBlendModeNormal);
		break;
		// テクスチャあり三角形
	case GameObjectType::Triangle:

		Engine::DrawTriangle(transform_, material_, light_, textureName_, Normal, kBlendModeNormal);
		break;
		// 球
	case GameObjectType::Sphere:

		Engine::DrawSphere(transform_, material_, light_, textureName_, Normal, kBlendModeNormal);
		break;
		// モデル
	case GameObjectType::Model:

		Engine::DrawModel(transform_, material_, light_, modelName_, textureName_, Normal, kBlendModeNormal);
		break;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								衝突コールバック関数
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::OnCollision() {

	material_.color = { 1.0f,0.0f,0.0f,1.0f };
}