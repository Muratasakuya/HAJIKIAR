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

///===============================================================================
/// クラス前方宣言
class DXCommon;
class TextureManager;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*									Model Class
////////////////////////////////////////////////////////////////////////////////*/
class Model {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	Model() = default;
	~Model() = default;

	// モデルデータ
	struct Object3DModelData {

		// 頂点バッファ
		std::vector<ComPtr<ID3D12Resource>> vertexResources;
		// 頂点バッファビュー
		std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferViews{};

		// 頂点バッファデータ
		std::vector<VertexData*> vertexDatas;

		// 定数バッファデータ
		ConstBufferData cBufferData_;
	};

	void CreateModelMesh(DXCommon* dxCommon, const std::string modelName, const ModelData& modelData);

	void Update(const std::string& modelName, const ModelData& modelData,
		const Transform& transform, std::vector<Material> materials, const PunctualLight& punctualLight);

	void Draw(const std::string& modelName,const std::string textureName, const ModelData& modelData, TextureManager* textureManager,ID3D12GraphicsCommandList* commandList);

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