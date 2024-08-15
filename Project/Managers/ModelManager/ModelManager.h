#pragma once

// DirectX
#include "CBufferStructure.h"
#include "VertexResource.h"

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// 3D
#include "Model.h"

// c++
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream> 
#include <unordered_map>
#include <filesystem>
#include <cassert>

///===============================================================================
/// クラス前方宣言
class DXCommon;
class TextureManager;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							ModelManager Class
////////////////////////////////////////////////////////////////////////////////*/
class ModelManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	ModelManager() = default;
	~ModelManager() = default;

	void Initialize(DXCommon* dxCommon, TextureManager* textureManager);

	void LoadModel(const std::string& directoryPath, const std::string& filename);
	void LoadGLTFModel(const std::string& directoryPath, const std::string& filename);
	void MakeModel(ModelData modelData, const std::string& modelName);

	ModelMaterialData LoadMaterialTemplateFile(const std::string& directorypath, const std::string& filename);
	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
	ModelData LoadGLTFFile(const std::string& directoryPath, const std::string& filename);
	Node ReadNode(aiNode* node);

	void CheckAvailability(const std::string name);

	void Update(const std::string& modelName, const Transform& transform, std::vector<Material> materials, const PunctualLight& punctualLight);

	void Draw(const std::string& modelName, const std::string textureName, ID3D12GraphicsCommandList* commandList);

	// getter

	ModelData GetModelData(const std::string& modelName);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	DXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager_ = nullptr;

	VertexResource vertexResource_;

	std::unordered_map<std::string, ModelData> models_;
	std::unique_ptr<Model> model_;
	

	static uint32_t indexModel_;
};