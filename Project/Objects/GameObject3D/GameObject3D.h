#pragma once

// NewMoon
#include "NewMoon.h"

// 構造体
#include "CBufferStructure.h"

// CollisionBase
#include "Collider.h"

// c++
#include <string>
#include <filesystem>

/*////////////////////////////////////////////////////////////////////////////////
*								GameObject3D Class
////////////////////////////////////////////////////////////////////////////////*/
class GameObject3D :
	public Collider<Vector3> {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	GameObject3D(GameObjectType type);
	~GameObject3D();

	void Initialize();
	void Update();
	void Draw();

	void MoveTranslate(const Vector3& velocity);

	void ImGui();

	// 衝突コールバック関数
	void OnCollision() override;

	// setter

	void SetTexture(const std::string name);
	void SetModel(const std::string name);

	void SetTranslate(Vector3 pos);
	void SetScale(Vector3 scale);
	void SetRotate(Vector3 rotate);

	void SetTriangleVertices(const std::array<Vector3, kTriangleVertexNum_>& vertices);

	void SetColor(Vector4 color);
	template<typename... Args>
	void SetColors(Args&&... args);

	void SetMaterialNum(uint32_t materialNum);

	void SetEnableHalfLambert(bool enable);

	void SetObjectName(const std::string name);

	// getter

	Vector3 GetCenterPos() const override;
	Vector3 GetScale()const;
	Vector3 GetRotate() const;
	Vector4 GetColor() const;
	GameObjectType GetType() const;
	std::string GetObjectName() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	/*--------------------------------------------------------------*/
	// メインオブジェクト

	// SRT
	Transform transform_{};

	// マテリアル
	Material material_{};
	std::vector<Material> materials_{};

	// ライト
	PunctualLight light_{};

	// ライティングフラグ
	bool enableLighting_ = false;
	bool enableHalfLambert_ = false;
	bool enablePhongReflection_ = false;
	bool enableBlinnPhongReflection_ = false;

	/*--------------------------------------------------------------*/
	// サブオブジェクト

	// 三角形頂点
	std::array<Vector3, kTriangleVertexNum_> triangleVertices_;

	// オブジェクトのタイプ
	GameObjectType type_;
	// オブジェクトの名前
	std::string objectName_;

	// 使用するテクスチャの名前
	std::string textureName_ = "Un";
	// 使用するモデルの名前
	std::string modelName_;

};

///===============================================================================
/// template関数定義

// 複数色のセット
template<typename ...Args>
void GameObject3D::SetColors(Args && ...args) {

	Vector4 colors[] = { std::forward<Args>(args)... };

	for (size_t i = 0; i < sizeof...(args); ++i) {
		materials_[i].color = colors[i];
	}
}
///===============================================================================