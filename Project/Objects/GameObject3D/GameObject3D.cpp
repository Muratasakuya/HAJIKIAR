#include "GameObject3D.h"

#include "ImGuiManager.h"

// Sceneで読み込んだテクスチャをセット
void GameObject3D::SetTexture(const std::string name) {

	std::filesystem::path textureName(name);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifier = textureName.stem().string();

	// テクスチャがあるかチェック
	NewMoon::CheckTextureAvailability(identifier);

	// 使用するテクスチャ名を設定
	textureName_ = identifier;
}
// Sceneで読み込んだモデルをセット
void GameObject3D::SetModel(const std::string name) {

	std::filesystem::path modelName(name);
	// 拡張子を除いたテクスチャ名を取得
	std::string identifier = modelName.stem().string();

	// モデルがあるかチェック
	NewMoon::CheckModelAvailability(identifier);

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
// type_ getter
GameObjectType GameObject3D::GetType() const {

	return type_;
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
	enableLighting_ = false;
	enableHalfLambert_ = false;
	enablePhongReflection_ = false;
	enableBlinnPhongReflection_ = false;
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

		NewMoon::DrawTriangle(transform_, material_, light_, textureName_, Primitive, kBlendModeNormal);
		break;
		// テクスチャあり三角形
	case GameObjectType::Triangle:

		NewMoon::DrawTriangle(transform_, material_, light_, textureName_, Normal, kBlendModeNormal);
		break;
		// 球
	case GameObjectType::Sphere:

		NewMoon::DrawSphere(transform_, material_, light_, textureName_, Normal, kBlendModeNormal);
		break;
		// モデル
	case GameObjectType::Model:

		NewMoon::DrawModel(transform_, material_, light_, modelName_, textureName_, Normal, kBlendModeNormal);
		break;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								  移動処理 右
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::MoveRight() {

	const float speed = 0.1f;

	this->transform_.translate.x += speed;
}
/*////////////////////////////////////////////////////////////////////////////////
*								  移動処理 左
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::MoveLeft() {

	const float speed = 0.1f;

	this->transform_.translate.x -= speed;
}

/*////////////////////////////////////////////////////////////////////////////////
*								衝突コールバック関数
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::OnCollision() {

	material_.color = { 1.0f,0.0f,0.0f,1.0f };
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGuiの操作
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::ImGui() {

	ImGui::Begin("NewMoon");

	static bool showTransform = false;
	static bool showMaterial = false;
	static bool showLight = false;

	// Transform ボタン
	if (ImGui::Button("Transform")) {

		showTransform = !showTransform;
		showMaterial = false;
		showLight = false;
	}
	if (showTransform) {

		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
		ImGui::DragFloat3("Rotation", &transform_.rotate.x, 0.01f);
		ImGui::DragFloat3("Translation", &transform_.translate.x, 0.01f);
	}

	// Material ボタン
	if (ImGui::Button("Material")) {

		showMaterial = !showMaterial;
		showTransform = false;
		showLight = false;
	}
	if (showMaterial) {

		ImGui::ColorEdit4("Color", &material_.color.x);
	}

	// Light ボタン
	if (ImGui::Button("Light")) {

		showLight = !showLight;
		showTransform = false;
		showMaterial = false;
	}
	if (showLight) {

		ImGui::Checkbox("EnableLighting", &enableLighting_);
		if (enableLighting_) {

			ImGui::Checkbox("HalfLambert", &enableHalfLambert_);
			ImGui::Checkbox("PhongReflection", &enablePhongReflection_);
			ImGui::Checkbox("BlinnPhongReflection", &enableBlinnPhongReflection_);
		}

		material_.enableLighting = enableLighting_;
		material_.enableHalfLambert = enableHalfLambert_;
		material_.enablePhongReflection = enablePhongReflection_;
		material_.enableBlinnPhongReflection = enableBlinnPhongReflection_;
	}

	// 一回やめ、あしたからはALの評価課題やってください。それまでNewMoon制作はお預け
	// やりたいことリスト
	// 1. Lightingを柔軟に入れ替える、HalfLambertにpointとspotもいれる
	// 2. Googleスライドみたいなグリッド線表示
	// 3. OpenCVカメラの高速化、NPPとか色々試したい
	// 4. フィルム越しの色の複数読み取り

	// 優先順位はHAJIKIがあるので下から順にかなって思う

	ImGui::End();
}