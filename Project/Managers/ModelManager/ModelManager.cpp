#include "ModelManager.h"

// models_[modelName] getter
ModelData ModelManager::GetModelData(const std::string& modelName) {

	assert(models_.find(modelName) != models_.end());
	return models_[modelName];
}

/*////////////////////////////////////////////////////////////////////////////////
*								Mtlファイルを読む関数
////////////////////////////////////////////////////////////////////////////////*/
ModelMaterialData ModelManager::LoadMaterialTemplateFile(const std::string& directorypath, const std::string& filename) {

	ModelMaterialData materialData;
	std::string line;
	std::ifstream file(directorypath + "/" + filename);

	// 開けなかったら止める
	assert(file.is_open());

	while (std::getline(file, line)) {

		std::string identifier;
		std::istringstream s(line);
		s >> identifier;            // 先頭の識別子を読む

		// identifierに応じた処理
		// map_Kd = textureのファイル名
		if (identifier == "map_Kd") {

			std::string textureFilename;
			s >> textureFilename;

			// 連結してファイルパスにする
			materialData.textureFilePath = directorypath + "/" + textureFilename;
		}
	}

	return materialData;
}

/*////////////////////////////////////////////////////////////////////////////////
*							aiNodeをNodeに変換する関数
////////////////////////////////////////////////////////////////////////////////*/
Node ModelManager::ReadNode(aiNode* node) {

	Node result;

	// nodeのlocalMatrixを取得
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;

	// 列ベクトル形式を行ベクトル形式に転置
	aiLocalMatrix.Transpose();

	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {

			result.localMatrix.m[i][j] = aiLocalMatrix[i][j];
		}
	}

	// Node名を格納
	result.name = node->mName.C_Str();
	// 子どもの数だけ確保
	result.children.resize(node->mNumChildren);

	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {

		// 再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}

/*////////////////////////////////////////////////////////////////////////////////
*							モデルassert あるかチェック
////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::CheckAvailability(const std::string name) {

	// nameがmodels_の中にあるかチェック
	auto it = models_.find(name);

	// ない場合はassert
	if (it == models_.end()) {

		assert(false && "Model not found");
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								Objファイルを読む関数
////////////////////////////////////////////////////////////////////////////////*/
ModelData ModelManager::LoadObjFile(const std::string& directoryPath, const std::string& filename) {

	ModelData modelData;            // 構築するModelData
	std::string line;               // ファイルから読んだ1行を格納するもの

	Assimp::Importer impoter;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = impoter.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	// メッシュがないのには対応しない
	assert(scene->HasMeshes());

	// メッシュ解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

		aiMesh* mesh = scene->mMeshes[meshIndex];

		// 法線がないMeshは今回は非対応
		assert(mesh->HasNormals());

		// TexcoordがないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0));

		MeshModelData meshModelData;

		// face解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {

			aiFace& face = mesh->mFaces[faceIndex];

			// 三角形のみサポート
			assert(face.mNumIndices == 3);

			// vertex解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {

				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex;

				vertex.pos = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };

				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.pos.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				meshModelData.vertices.push_back(vertex);
			}
		}

		// マテリアル解析
		if (scene->HasMaterials()) {

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {

				aiString textureFilePath;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
				meshModelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
			}
		}

		// メッシュデータをモデルデータに追加
		modelData.meshes.push_back(meshModelData);
	}

	// マテリアルカウントを記憶
	modelData.numMaterial = scene->mNumMaterials;

	return modelData;
}

/*////////////////////////////////////////////////////////////////////////////////
*								GLTFファイルを読む関数
////////////////////////////////////////////////////////////////////////////////*/
ModelData ModelManager::LoadGLTFFile(const std::string& directoryPath, const std::string& filename) {

	ModelData modelData;            // 構築するModelData
	std::string line;               // ファイルから読んだ1行を格納するもの

	Assimp::Importer impoter;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = impoter.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	// メッシュがないのには対応しない
	assert(scene->HasMeshes());

	// メッシュ解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {

		aiMesh* mesh = scene->mMeshes[meshIndex];

		// 法線がないMeshは今回は非対応
		assert(mesh->HasNormals());

		// TexcoordがないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0));

		MeshModelData meshModelData;

		// face解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {

			aiFace& face = mesh->mFaces[faceIndex];

			// 三角形のみサポート
			assert(face.mNumIndices == 3);

			// vertex解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {

				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex;

				vertex.pos = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };

				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.pos.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				meshModelData.vertices.push_back(vertex);
			}
		}

		// マテリアル解析
		if (scene->HasMaterials()) {

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {

				aiString textureFilePath;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
				meshModelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
			}

			// Diffuse色の取得
			aiColor4D diffuseColor;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor)) {
				meshModelData.material.diffuseColor = {
					diffuseColor.r,
					diffuseColor.g,
					diffuseColor.b,
					diffuseColor.a
				};
			}
		}

		// メッシュデータをモデルデータに追加
		modelData.meshes.push_back(meshModelData);
	}

	// 階層構造の作成
	modelData.rootNode = ReadNode(scene->mRootNode);

	// マテリアルカウントを記憶
	modelData.numMaterial = scene->mNumMaterials;

	return modelData;
}

/*////////////////////////////////////////////////////////////////////////////////
*								モデルを作成する関数
////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::LoadModel(const std::string& directoryPath, const std::string& filename) {

	// ファイルパスからモデル名を取得
	std::filesystem::path path(filename);
	// 拡張子を除いたモデル名を取得
	std::string identifier = path.stem().string();

	// モデルがすでにあれば早期リターン
	if (models_.find(identifier) != models_.end()) {
		return;
	}

	ModelData modelData = LoadObjFile(directoryPath, filename);
	models_[identifier] = modelData;

	model_->CreateModelMesh(dxCommon_, identifier, models_[identifier]);
}

/*////////////////////////////////////////////////////////////////////////////////
*							   GLTFモデルを作成する関数
////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::LoadGLTFModel(const std::string& directoryPath, const std::string& filename) {

	// ファイルパスからモデル名を取得
	std::filesystem::path path(filename);
	// 拡張子を除いたモデル名を取得
	std::string identifier = path.stem().string();

	// モデルがすでにあれば早期リターン
	if (models_.find(identifier) != models_.end()) {
		return;
	}

	ModelData modelData = LoadGLTFFile(directoryPath, filename);
	models_[identifier] = modelData;

	model_->CreateModelMesh(dxCommon_, identifier, models_[identifier]);
}

/*////////////////////////////////////////////////////////////////////////////////
*							自作したモデルを作成する関数
////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::MakeModel(ModelData modelData, const std::string& modelName) {

	// モデルがすでにあれば早期リターン
	if (models_.find(modelName) != models_.end()) {
		return;
	}

	model_->CreateModelMesh(dxCommon_, modelName, modelData);

	models_[modelName] = modelData;
}

/*////////////////////////////////////////////////////////////////////////////////
*							モデルメッシュインスタンスの生成
////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::Initialize(DXCommon* dxCommon, TextureManager* textureManager) {

	assert(dxCommon);
	assert(textureManager);

	dxCommon_ = dxCommon;
	textureManager_ = textureManager;

	model_ = std::make_unique<Model>();
}

/*////////////////////////////////////////////////////////////////////////////////
*									更新処理
////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::Update(
	const std::string& modelName, const Transform& transform, const Material& material, const PunctualLight& punctualLight) {

	model_->Update(modelName, models_[modelName], transform, material, punctualLight);
}

/*////////////////////////////////////////////////////////////////////////////////
*								更新処理 マルチマテリアル
////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::MultiMaterialUpdate(
	const std::string& modelName, const Transform& transform, const std::vector<Material>& materials, const PunctualLight& punctualLight) {

	model_->MultiMaterialUpdate(modelName, models_[modelName], transform, materials, punctualLight);
}

/*////////////////////////////////////////////////////////////////////////////////
*								   モデル描画
////////////////////////////////////////////////////////////////////////////////*/
void ModelManager::Draw(const std::string& modelName, const std::string textureName, ID3D12GraphicsCommandList* commandList) {

	model_->Draw(modelName, textureName, models_[modelName], textureManager_, commandList);
}