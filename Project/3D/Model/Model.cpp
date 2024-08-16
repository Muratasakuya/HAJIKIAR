#include "Model.h"

#include "DXCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "MainCamera3D.h"


// models_.find(modelName) it->second.get() getter
Model::Object3DModelData* Model::GetModel(const std::string& modelName) const {

	auto it = models_.find(modelName);

	if (it != models_.end()) {

		return it->second.get();
	} else {

		return nullptr;
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*							  モデルメッシュの生成
////////////////////////////////////////////////////////////////////////////////*/
void Model::CreateModelMesh(DXCommon* dxCommon, const std::string modelName, const ModelData& modelData) {

	assert(dxCommon);

	if (!dxCommon_) {

		dxCommon_ = dxCommon;
	}

	HRESULT hr;
	models_[modelName] = std::make_unique<Object3DModelData>();

	for (uint32_t i = 0; i < modelData.meshes.size(); i++) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * modelData.meshes[i].vertices.size());

		// 頂点バッファの生成
		ComPtr<ID3D12Resource> vertexResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeVB);
		models_[modelName]->vertexResources.push_back(vertexResource);

		// 頂点バッファビューの作成
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		vertexBufferView.BufferLocation = models_[modelName]->vertexResources[i]->GetGPUVirtualAddress();
		vertexBufferView.SizeInBytes = sizeVB;
		vertexBufferView.StrideInBytes = sizeof(VertexData);
		models_[modelName]->vertexBufferViews.push_back(vertexBufferView);

		VertexData* vertexData = nullptr;
		models_[modelName]->vertexDatas.push_back(vertexData);

		// 頂点データのマッピング
		hr = models_[modelName]->vertexResources[i]->Map(0, nullptr, reinterpret_cast<void**>(&models_[modelName]->vertexDatas[i]));
		assert(SUCCEEDED(hr));
	}

	/*////////////////////////////////////////////////////////////////////////////////*/
	// マテリアル
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// マテリアル ///

	models_[modelName]->cBufferData_.materialResources.clear();
	models_[modelName]->cBufferData_.materials.clear();

	for (uint32_t i = 0; i < modelData.meshes.size(); i++) {

		// マテリアルバッファの生成
		ComPtr<ID3D12Resource> materialResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));
		models_[modelName]->cBufferData_.materialResources.push_back(materialResource);

		Material* material = nullptr;
		models_[modelName]->cBufferData_.materials.push_back(material);

		// マテリアルデータのマッピング
		hr = models_[modelName]->cBufferData_.materialResources[i]->Map(
			0, nullptr, reinterpret_cast<void**>(&models_[modelName]->cBufferData_.materials[i]));
		// 作れなければエラー
		assert(SUCCEEDED(hr));

		// マテリアル初期値
		models_[modelName]->cBufferData_.materials[i]->color = { 1.0f,1.0f,1.0f,1.0f };
		models_[modelName]->cBufferData_.materials[i]->enableLighting = false;
		models_[modelName]->cBufferData_.materials[i]->enableHalfLambert = false;
		models_[modelName]->cBufferData_.materials[i]->enablePhongReflection = false;
		models_[modelName]->cBufferData_.materials[i]->enableBlinnPhongReflection = false;
		models_[modelName]->cBufferData_.materials[i]->phongRefShininess = 32.0f;
		models_[modelName]->cBufferData_.materials[i]->specularColor = { 1.0f,1.0f,1.0f };
		models_[modelName]->cBufferData_.materials[i]->uvTransform = Matrix4x4::MakeIdentity4x4();
	}
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// 座標変換データ
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// 座標変換データ ///
	// 座標変換データバッファの生成
	models_[modelName]->cBufferData_.matrixResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));

	// 座標変換データのマッピング
	hr = models_[modelName]->cBufferData_.matrixResource->Map(
		0, nullptr, reinterpret_cast<void**>(&models_[modelName]->cBufferData_.transformationMatrix));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// 座標変換データの初期値
	models_[modelName]->cBufferData_.transformationMatrix->World = Matrix4x4::MakeIdentity4x4();
	models_[modelName]->cBufferData_.transformationMatrix->WVP = Matrix4x4::MakeIdentity4x4();
	models_[modelName]->cBufferData_.transformationMatrix->WorldInverseTranspose = Matrix4x4::MakeIdentity4x4();
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// ライト
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// ライト ///
	// ライトバッファの生成
	models_[modelName]->cBufferData_.lightResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(PunctualLight));

	// ライトデータのマッピング
	hr = models_[modelName]->cBufferData_.lightResource->Map(
		0, nullptr, reinterpret_cast<void**>(&models_[modelName]->cBufferData_.light));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// ライトデータの初期値
	// 平行光源
	models_[modelName]->cBufferData_.light->directional.color = { 1.0f,1.0f,1.0f,1.0f };
	models_[modelName]->cBufferData_.light->directional.direction = { 0.0f,-1.0f,0.0f };
	models_[modelName]->cBufferData_.light->directional.intensity = 1.0f;
	// ポイントライト
	models_[modelName]->cBufferData_.light->point.color = { 1.0f,1.0f,1.0f,1.0f };
	models_[modelName]->cBufferData_.light->point.pos = { 0.0f,0.0f,0.0f };
	models_[modelName]->cBufferData_.light->point.intensity = 1.0f;
	models_[modelName]->cBufferData_.light->point.radius = 5.0f;
	models_[modelName]->cBufferData_.light->point.decay = 1.0f;
	// スポットライト
	models_[modelName]->cBufferData_.light->spot.color = { 1.0f,1.0f,1.0f,1.0f };
	models_[modelName]->cBufferData_.light->spot.pos = { 2.0f,1.0f,0.0f };
	models_[modelName]->cBufferData_.light->spot.intensity = 1.0f;
	models_[modelName]->cBufferData_.light->spot.direction = { -1.0f,-1.0f,0.0f };
	models_[modelName]->cBufferData_.light->spot.distance = 5.0f;
	models_[modelName]->cBufferData_.light->spot.decay = 1.0f;
	models_[modelName]->cBufferData_.light->spot.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	models_[modelName]->cBufferData_.light->spot.cosFalloffStart = std::cos(std::numbers::pi_v<float> / 3.0f);
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// カメラ
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// カメラ ///
	// カメラバッファの生成
	models_[modelName]->cBufferData_.cameraResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(CameraForGPU));

	// カメラデータのマッピング
	hr = models_[modelName]->cBufferData_.cameraResource->Map(
		0, nullptr, reinterpret_cast<void**>(&models_[modelName]->cBufferData_.cameraForGPU));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// カメラデータの初期値
	models_[modelName]->cBufferData_.cameraForGPU->worldPosition = MainCamera3D::GetInstance()->GetWorldPos();
#pragma endregion

}

/*////////////////////////////////////////////////////////////////////////////////
*								　更新処理
////////////////////////////////////////////////////////////////////////////////*/
void Model::Update(const std::string& modelName, const ModelData& modelData,
	const Transform& transform, const Material& material, const PunctualLight& punctualLight) {

	MainCamera3D::GetInstance()->ImGuiDraw();
	MainCamera3D::GetInstance()->Update();

	for (uint32_t i = 0; i < modelData.meshes.size(); i++) {

		// 頂点バッファへデータ転送
		std::memcpy(models_[modelName]->vertexDatas[i],
			modelData.meshes[i].vertices.data(),
			sizeof(VertexData) * modelData.meshes[i].vertices.size());
	}

#pragma region /// ConstBufferの更新 ///

	//Affine
	Matrix4x4 worldMatrix =
		Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 wvpMatrix =
		Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(MainCamera3D::GetInstance()->GetViewMatrix(), MainCamera3D::GetInstance()->GetProjectionMatrix()));
	Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

	// 行列の更新
	models_[modelName]->cBufferData_.transformationMatrix->World = worldMatrix;
	models_[modelName]->cBufferData_.transformationMatrix->WVP = wvpMatrix;
	models_[modelName]->cBufferData_.transformationMatrix->WorldInverseTranspose = worldInverseTranspose;

	for (uint32_t index = 0; index < models_[modelName]->cBufferData_.materials.size(); index++) {

		models_[modelName]->cBufferData_.materials[index]->color = material.color;
		models_[modelName]->cBufferData_.materials[index]->enableLighting = material.enableLighting;
		models_[modelName]->cBufferData_.materials[index]->enableHalfLambert = material.enableHalfLambert;
		models_[modelName]->cBufferData_.materials[index]->enablePhongReflection = material.enablePhongReflection;
		models_[modelName]->cBufferData_.materials[index]->enableBlinnPhongReflection = material.enableBlinnPhongReflection;
		models_[modelName]->cBufferData_.materials[index]->phongRefShininess = material.phongRefShininess;
		models_[modelName]->cBufferData_.materials[index]->specularColor = material.specularColor;
		models_[modelName]->cBufferData_.materials[index]->uvTransform = material.uvTransform;

		// パンクチュアルライトの更新
		// Lighting時のみ
		if (models_[modelName]->cBufferData_.materials[index]->enableLighting) {

			// 平行光源
			models_[modelName]->cBufferData_.light->directional.color = punctualLight.directional.color;
			models_[modelName]->cBufferData_.light->directional.direction = punctualLight.directional.direction;
			models_[modelName]->cBufferData_.light->directional.intensity = punctualLight.directional.intensity;
			// ポイントライト
			models_[modelName]->cBufferData_.light->point.color = punctualLight.point.color;
			models_[modelName]->cBufferData_.light->point.pos = punctualLight.point.pos;
			models_[modelName]->cBufferData_.light->point.intensity = punctualLight.point.intensity;
			models_[modelName]->cBufferData_.light->point.radius = punctualLight.point.radius;
			models_[modelName]->cBufferData_.light->point.decay = punctualLight.point.decay;
			// スポットライト
			models_[modelName]->cBufferData_.light->spot.color = punctualLight.spot.color;
			models_[modelName]->cBufferData_.light->spot.pos = punctualLight.spot.pos;
			models_[modelName]->cBufferData_.light->spot.intensity = punctualLight.spot.intensity;
			models_[modelName]->cBufferData_.light->spot.direction = punctualLight.spot.direction;
			models_[modelName]->cBufferData_.light->spot.distance = punctualLight.spot.distance;
			models_[modelName]->cBufferData_.light->spot.decay = punctualLight.spot.decay;
			models_[modelName]->cBufferData_.light->spot.cosAngle = punctualLight.spot.cosAngle;
			models_[modelName]->cBufferData_.light->spot.cosFalloffStart = punctualLight.spot.cosFalloffStart;
		}
	}

	// カメラのワールド座標の更新
	models_[modelName]->cBufferData_.cameraForGPU->worldPosition = MainCamera3D::GetInstance()->GetWorldPos();
#pragma endregion

}

/*////////////////////////////////////////////////////////////////////////////////
*							更新処理 マルチマテリアル
////////////////////////////////////////////////////////////////////////////////*/
void Model::MultiMaterialUpdate(const std::string& modelName, const ModelData& modelData,
	const Transform& transform, const std::vector<Material>& materials, const PunctualLight& punctualLight){

	MainCamera3D::GetInstance()->ImGuiDraw();
	MainCamera3D::GetInstance()->Update();

	for (uint32_t i = 0; i < modelData.meshes.size(); i++) {

		// 頂点バッファへデータ転送
		std::memcpy(models_[modelName]->vertexDatas[i],
			modelData.meshes[i].vertices.data(),
			sizeof(VertexData) * modelData.meshes[i].vertices.size());
	}

#pragma region /// ConstBufferの更新 ///

	//Affine
	Matrix4x4 worldMatrix =
		Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 wvpMatrix =
		Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(MainCamera3D::GetInstance()->GetViewMatrix(), MainCamera3D::GetInstance()->GetProjectionMatrix()));
	Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

	// 行列の更新
	models_[modelName]->cBufferData_.transformationMatrix->World = worldMatrix;
	models_[modelName]->cBufferData_.transformationMatrix->WVP = wvpMatrix;
	models_[modelName]->cBufferData_.transformationMatrix->WorldInverseTranspose = worldInverseTranspose;

	for (uint32_t index = 0; index < models_[modelName]->cBufferData_.materials.size(); index++) {

		models_[modelName]->cBufferData_.materials[index]->color = materials[index].color;
		models_[modelName]->cBufferData_.materials[index]->enableLighting = materials[index].enableLighting;
		models_[modelName]->cBufferData_.materials[index]->enableHalfLambert = materials[index].enableHalfLambert;
		models_[modelName]->cBufferData_.materials[index]->enablePhongReflection = materials[index].enablePhongReflection;
		models_[modelName]->cBufferData_.materials[index]->enableBlinnPhongReflection = materials[index].enableBlinnPhongReflection;
		models_[modelName]->cBufferData_.materials[index]->phongRefShininess = materials[index].phongRefShininess;
		models_[modelName]->cBufferData_.materials[index]->specularColor = materials[index].specularColor;
		models_[modelName]->cBufferData_.materials[index]->uvTransform = materials[index].uvTransform;

		// パンクチュアルライトの更新
		// Lighting時のみ
		if (models_[modelName]->cBufferData_.materials[index]->enableLighting) {

			// 平行光源
			models_[modelName]->cBufferData_.light->directional.color = punctualLight.directional.color;
			models_[modelName]->cBufferData_.light->directional.direction = punctualLight.directional.direction;
			models_[modelName]->cBufferData_.light->directional.intensity = punctualLight.directional.intensity;
			// ポイントライト
			models_[modelName]->cBufferData_.light->point.color = punctualLight.point.color;
			models_[modelName]->cBufferData_.light->point.pos = punctualLight.point.pos;
			models_[modelName]->cBufferData_.light->point.intensity = punctualLight.point.intensity;
			models_[modelName]->cBufferData_.light->point.radius = punctualLight.point.radius;
			models_[modelName]->cBufferData_.light->point.decay = punctualLight.point.decay;
			// スポットライト
			models_[modelName]->cBufferData_.light->spot.color = punctualLight.spot.color;
			models_[modelName]->cBufferData_.light->spot.pos = punctualLight.spot.pos;
			models_[modelName]->cBufferData_.light->spot.intensity = punctualLight.spot.intensity;
			models_[modelName]->cBufferData_.light->spot.direction = punctualLight.spot.direction;
			models_[modelName]->cBufferData_.light->spot.distance = punctualLight.spot.distance;
			models_[modelName]->cBufferData_.light->spot.decay = punctualLight.spot.decay;
			models_[modelName]->cBufferData_.light->spot.cosAngle = punctualLight.spot.cosAngle;
			models_[modelName]->cBufferData_.light->spot.cosFalloffStart = punctualLight.spot.cosFalloffStart;
		}
	}

	// カメラのワールド座標の更新
	models_[modelName]->cBufferData_.cameraForGPU->worldPosition = MainCamera3D::GetInstance()->GetWorldPos();
#pragma endregion
}

/*////////////////////////////////////////////////////////////////////////////////
*							  頂点バッファセット
////////////////////////////////////////////////////////////////////////////////*/
void Model::Draw(const std::string& modelName, const std::string textureName, const ModelData& modelData, TextureManager* textureManager, ID3D12GraphicsCommandList* commandList) {

	for (uint32_t i = 0; i < modelData.meshes.size(); i++) {

		// 頂点バッファの設定
		commandList->IASetVertexBuffers(0, 1, &models_[modelName]->vertexBufferViews[i]);

		if (textureName != "Un") {

			// マテリアルCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(0, models_[modelName]->cBufferData_.materialResources[i].Get()->GetGPUVirtualAddress());
			// wvp用のCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(1, models_[modelName]->cBufferData_.matrixResource.Get()->GetGPUVirtualAddress());
			// ライトCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(3, models_[modelName]->cBufferData_.lightResource.Get()->GetGPUVirtualAddress());
			// カメラCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(4, models_[modelName]->cBufferData_.cameraResource.Get()->GetGPUVirtualAddress());
		} else {

			// マテリアルCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(0, models_[modelName]->cBufferData_.materialResources[i].Get()->GetGPUVirtualAddress());
			// wvp用のCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(1, models_[modelName]->cBufferData_.matrixResource.Get()->GetGPUVirtualAddress());
			// ライトCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(2, models_[modelName]->cBufferData_.lightResource.Get()->GetGPUVirtualAddress());
			// カメラCBufferの場所を設定
			commandList->SetGraphicsRootConstantBufferView(3, models_[modelName]->cBufferData_.cameraResource.Get()->GetGPUVirtualAddress());
		}

		// SRVのセット
		textureManager->SetGraphicsRootDescriptorTable(commandList, 2, textureName);

		// DrawCall
		commandList->DrawInstanced(static_cast<UINT>(modelData.meshes[i].vertices.size()), 1, 0, 0);
	}
}