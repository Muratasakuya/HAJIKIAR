#pragma once

// DirectX
#include "CBufferStructure.h"
#include "VertexResource.h"

// c++
#include <memory>
#include <string>
#include <numbers>
#include <cassert>
#include <unordered_map>

// 前方宣言
class DXCommon;

/*////////////////////////////////////////////////////////////////////////////////
*
*									Model Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Model {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// モデルデータ
	struct Object3DModelData {

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexResource;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// 頂点バッファデータ
		VertexData* vertexData = nullptr;

		// 定数バッファデータ
		ConstBufferData cBufferData_;
	};

	void CreateModelMesh(DXCommon* dxCommon,const std::string modelName, UINT vertexCount);

	void Update(const std::string& modelName, const Transform& transform, const Material& material, const PunctualLight& punctualLight);

	void SetBufferData(const std::string& modelName, ID3D12GraphicsCommandList* commandList);
	void DrawCall(const std::string& modelName, ID3D12GraphicsCommandList* commandList);

	// getter

	Object3DModelData* GetModel(const std::string& modelName) const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	DXCommon* dxCommon_ = nullptr;

	// CreateBuffer
	VertexResource vertexResource_;

	/*--------------------------------------------------------------------*/
	/// モデル

	// モデルメッシュデータ
	std::unordered_map<std::string, std::unique_ptr<Object3DModelData>> models_;

	/*--------------------------------------------------------------------*/

};