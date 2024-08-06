#pragma once

// DirectX
#include "CBufferStructure.h"
#include "VertexResource.h"

// Lib
#include "MainCamera3D.h"

// 3D
#include "Model.h"

// c++
#include <memory>
#include <numbers>
#include <cassert>

///===============================================================================
/// クラス前方宣言
class DXCommon;
///===============================================================================

///* 使用可能3Dオブジェクト *///
// 三角形 Triangle
// 球 SPhere
enum Object3DType {

	ObjectTriangle,
	ObjectSphere,
};

// 球の頂点数 
static const UINT kTriangleVertexNum_ = 3;

// 球の分割数
static const uint32_t kSphereSubdivision = 16;
// 球の頂点数 Vertex1536
static const UINT kSphereVertexNum_ = kSphereSubdivision * kSphereSubdivision * 6;
// 球の頂点数　Index1024
static const UINT kSphereIndexNum_ = kSphereSubdivision * kSphereSubdivision * 4;

/*////////////////////////////////////////////////////////////////////////////////
*								Object3D Class
////////////////////////////////////////////////////////////////////////////////*/
class Object3D {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	Object3D() = default;
	~Object3D() = default;

	// 三角形データ
	struct TriangleData {

		/*-------------------------------------------------------*/
		// 頂点

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexResource;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// 頂点バッファデータ
		VertexData* vertexData = nullptr;

		// 定数バッファデータ
		ConstBufferData cBufferData_;
	};

	// 球データ
	struct SphereData {

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexResource;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// インデックス頂点バッファ
		ComPtr<ID3D12Resource> indexResource;
		// インデックス頂点バッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView{};

		// 頂点バッファデータ
		VertexData* vertexData = nullptr;
		// インデックスバッファデータ
		uint32_t* indexData = nullptr;

		// 定数バッファデータ
		ConstBufferData cBufferData_;
	};

	void Initialize(DXCommon* dxCommon);

	void Update(const Object3DType& objectType, const Transform& transform, const Material& material, const PunctualLight& punctualLight);

	void SetBufferData(ID3D12GraphicsCommandList* commandList, const Object3DType& objectType);
	void DrawCall(ID3D12GraphicsCommandList* commandList, const Object3DType& objectType);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	DXCommon* dxCommon_ = nullptr;

	// CreateBuffer
	VertexResource vertexResource_;

	// 3DCamera
	MainCamera3D* camera3D_ = nullptr;
	std::string modelName_;

	/*--------------------------------------------------------------------*/
	/// 三角形

	// 三角形データ
	std::unique_ptr<TriangleData> triangle_;
	// 三角形生成
	std::unique_ptr<TriangleData> CreateTriangleData(UINT vertexCount);

	/*--------------------------------------------------------------------*/
	/// 球

	// 球データ
	std::unique_ptr<SphereData> sphere_;
	// 球生成
	std::unique_ptr<SphereData> CreateSphereData(UINT vertexCount, UINT indexCount);

	/*--------------------------------------------------------------------*/

};