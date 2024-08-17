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
	objectName_ = identifier;
}

// 座標のセット
void GameObject3D::SetTranslate(Vector3 pos) { transform_.translate = pos; }
// スケールのセット
void GameObject3D::SetScale(Vector3 scale) { transform_.scale = scale; }
// 回転のセット
void GameObject3D::SetRotate(Vector3 rotate) { transform_.rotate = rotate; }

// 三角形頂点のセット
void GameObject3D::SetTriangleVertices(const std::array<Vector3, kTriangleVertexNum_>& vertices) { triangleVertices_ = vertices; }

// 色のセット
void GameObject3D::SetColor(Vector4 color) { material_.color = color; }

// マテリアルの数のセット
void GameObject3D::SetMaterialNum(uint32_t materialNum){

	for (uint32_t i = 0; i < materialNum; i++) {

		materials_.push_back(material_);
	}
}

// オブジェクトの名前のセット
void GameObject3D::SetObjectName(const std::string name) { objectName_ = name; }

// 中心座標取得 transform_.translate getter
Vector3 GameObject3D::GetCenterPos() const { return transform_.translate; }
// スケールの取得
Vector3 GameObject3D::GetScale() const { return transform_.scale; }
// 回転の取得
Vector3 GameObject3D::GetRotate() const { return transform_.rotate; }
// type_ getter
GameObjectType GameObject3D::GetType() const { return type_; }
// objectName_ getter
std::string GameObject3D::GetObjectName() const { return objectName_; }

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameObject3D::GameObject3D(GameObjectType type) : type_(type) {}

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameObject3D::~GameObject3D() {}

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
void GameObject3D::Update() {}

/*////////////////////////////////////////////////////////////////////////////////
*								   描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameObject3D::Draw() {

	switch (type_) {
		// テクスチャなし三角形
	case GameObjectType::PrimitiveTriangle:

		NewMoon::DrawTriangle(triangleVertices_, transform_, material_, light_, textureName_, Primitive, kBlendModeNormal);
		break;
		// テクスチャあり三角形
	case GameObjectType::Triangle:

		NewMoon::DrawTriangle(triangleVertices_, transform_, material_, light_, textureName_, pObject3D, kBlendModeNormal);
		break;
		// 球
	case GameObjectType::Sphere:

		NewMoon::DrawSphere(transform_, material_, light_, textureName_, pObject3D, kBlendModeNormal);
		break;
		// モデル
	case GameObjectType::Model:

		if (textureName_ != "Un") {

			NewMoon::DrawModel(transform_, material_, light_, modelName_, textureName_, pObject3D, kBlendModeNormal);
		} else {

			NewMoon::DrawMultiModel(transform_, materials_, light_, modelName_, textureName_, Object3DUnTex, kBlendModeNormal);
		}
		break;
	}
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
		ImGui::SliderAngle("RotationX", &transform_.rotate.x);
		ImGui::SliderAngle("RotationY", &transform_.rotate.y);
		ImGui::SliderAngle("RotationZ", &transform_.rotate.z);
		ImGui::DragFloat3("Translation", &transform_.translate.x, 0.001f);
	}

	// Material ボタン
	if (ImGui::Button("Material")) {

		showMaterial = !showMaterial;
		showTransform = false;
		showLight = false;
	}
	if (showMaterial) {

		if (materials_.size() == 0) {
		
			ImGui::ColorEdit4("Color", &material_.color.x);
		} else {
			// Material が複数ある場合
			for (size_t i = 0; i < materials_.size(); i++) {
				std::string label = "Material " + std::to_string(i + 1);

				if (ImGui::CollapsingHeader(label.c_str())) {

					std::string colorLabel = "Color##" + std::to_string(i + 1);
					ImGui::ColorEdit4(colorLabel.c_str(), &materials_[i].color.x);
				}
			}
		}
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

			// HalfLambert処理
			if (enableHalfLambert_) {

				enablePhongReflection_ = false;
				enableBlinnPhongReflection_ = false;
			}

			// PhongReflectionの処理
			if (enablePhongReflection_) {

				enableHalfLambert_ = false;
				enableBlinnPhongReflection_ = false;
			}

			// BlinnPhongReflectionの処理
			if (enableBlinnPhongReflection_) {

				enableHalfLambert_ = false;
				enablePhongReflection_ = false;
			}
		}

		material_.enableLighting = enableLighting_;
		material_.enableHalfLambert = enableHalfLambert_;
		material_.enablePhongReflection = enablePhongReflection_;
		material_.enableBlinnPhongReflection = enableBlinnPhongReflection_;
	}

	for (uint32_t i = 0; i < materials_.size(); i++) {

		materials_[i].enableLighting = material_.enableLighting;
		materials_[i].enableHalfLambert = material_.enableHalfLambert;
	}

	ImGui::End();
}