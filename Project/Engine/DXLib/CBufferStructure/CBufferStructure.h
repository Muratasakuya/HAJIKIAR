#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"

// Lib
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

// c++
#include <vector>
#include <memory>
#include <list>
#include <numbers>
#include <string>

/*==========================================================*/
/// Transform
struct Transform {

	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct Transform2D {

	Vector2 pos;            // 座標
	float rotate;           // 回転
	Vector2 size;           // 大きさ
	Vector2 anchorPoint;    // アンカーポイント /* {0.5f,0.5f}で真ん中 */
	Vector2 textureLeftTop; // テクスチャ左上座標
	Vector2 textureSize;    // テクスチャ切り出しサイズ
	bool isFlipX;           // 左右反転
	bool isFlipY;           // 上下反転
};
/*==========================================================*/
/// 頂点データ
struct VertexData {

	Vector4 pos;
	Vector2 texcoord;
	Vector3 normal;
};
struct SpriteVertexData {

	Vector2 pos;
	Vector2 texcoord;
};
/*==========================================================*/
/// マテリアル
struct Material {

	Vector4 color;
	int32_t enableLighting;
	int32_t enableHalfLambert;
	int32_t enablePhongReflection;
	int32_t enableBlinnPhongReflection;
	float phongRefShininess;
	Vector3 specularColor;
	Matrix4x4 uvTransform;
};
struct SpriteMaterial {

	Vector4 color;
	Matrix4x4 uvTransform;
};
/*==========================================================*/
/// PunctualLight
struct DirectionalLight {

	Vector4 color;
	Vector3 direction;
	float intensity;

	// DefaultInitialize
	void Initialize() {

		color = { 1.0f,1.0f,1.0f,1.0f };
		direction = { 0.0f,-1.0f,0.0f };
		intensity = 1.0f;
	};
};
struct PointLight {

	Vector4 color;
	Vector3 pos;
	float intensity;
	float radius;
	float decay;
	float padding[2];

	// DefaultInitialize
	void Initialize() {

		color = { 1.0f,1.0f,1.0f,1.0f };
		pos = { 0.0f,0.0f,0.0f };
		radius = 5.0f;
		intensity = 1.0f;
		decay = 1.0f;
	};
};
struct SpotLight {

	Vector4 color;
	Vector3 pos;
	float intensity;
	Vector3 direction;
	float distance;
	float decay;
	float cosAngle;
	float cosFalloffStart;
	float padding[2];

	// DefaultInitialize
	void Initialize() {

		color = { 1.0f,1.0f,1.0f,1.0f };
		pos = { 0.0f,0.0f,0.0f };
		distance = 10.0f;
		intensity = 1.0f;
		direction = { -1.0f,-1.0f,0.0f };
		decay = 1.0f;
		cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
		cosFalloffStart = 1.0f;
	};
};
struct PunctualLight {

	DirectionalLight directional;
	PointLight point;
	SpotLight spot;

	// DefaultInitialize
	void Initialize() {

		directional.Initialize();
		point.Initialize();
		spot.Initialize();
	};
};
/*==========================================================*/
/// 座標変換データ
struct TransformationMatrix {

	Matrix4x4 World;
	Matrix4x4 WVP;
	Matrix4x4 WorldInverseTranspose;
};
struct SpriteTransformationMatrix {

	Matrix4x4 WVP;
};
/*==========================================================*/
/// パーティクル
struct ParticleForGPU {

	Matrix4x4 World;
	Matrix4x4 WVP;
	Vector4 color;
};
struct ParticleData {

	Transform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};
struct Emitter {

	Transform transform;
	uint32_t count;
	float frequency;
	float frequencyTime;
};
struct AABB {

	Vector3 min;
	Vector3 max;
};
struct AccelerationField {

	Vector3 acceleration;
	AABB area;
};
/*==========================================================*/
/// カメラ座標
struct CameraForGPU {

	Vector3 worldPosition;
};
/*==========================================================*/
/// モデル
struct ModelMaterialData {

	std::string textureFilePath;
};
struct Node {

	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};
struct ModelData {

	std::vector<VertexData> vertices;
	ModelMaterialData material;
	Node rootNode;
};
/*==========================================================*/
/// 定数バッファ
struct ConstBufferData {

	/*-------------------------------------------------------*/
	// Material

	// マテリアルリソース
	ComPtr<ID3D12Resource> materialResource;
	// マテリアルバッファデータ
	Material* material = nullptr;

	/*-------------------------------------------------------*/
	// TransformationMatrix

	// 座標変換データリソース
	ComPtr<ID3D12Resource> matrixResource;
	// 座標変換データバッファデータ
	TransformationMatrix* transformationMatrix = nullptr;

	/*-------------------------------------------------------*/
	// PunctualLight

	// ライトリソース
	ComPtr<ID3D12Resource> lightResource;
	// ライトバッファデータ
	PunctualLight* light = nullptr;

	/*-------------------------------------------------------*/
	// Camera

	// カメラリソース
	ComPtr<ID3D12Resource> cameraResource;
	// カメラバッファデータ
	CameraForGPU* cameraForGPU = nullptr;
};