#include "Model.h"

#include "DXCommon.h"
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
void Model::CreateModelMesh(DXCommon* dxCommon, const std::string modelName, UINT vertexCount) {

	assert(dxCommon);

	if (!dxCommon_) {

		dxCommon_ = dxCommon;
	}

	HRESULT hr;
	models_[modelName] = std::make_unique<Object3DModelData>();

	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertexCount);

		// 頂点バッファの生成
		models_[modelName]->vertexResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeVB);

		// 頂点バッファビューの作成
		models_[modelName]->vertexBufferView.BufferLocation = models_[modelName]->vertexResource->GetGPUVirtualAddress();
		models_[modelName]->vertexBufferView.SizeInBytes = sizeVB;
		models_[modelName]->vertexBufferView.StrideInBytes = sizeof(VertexData);

		// 頂点データのマッピング
		hr = models_[modelName]->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&models_[modelName]->vertexData));
		assert(SUCCEEDED(hr));
	}

	/*////////////////////////////////////////////////////////////////////////////////*/
	// マテリアル
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// マテリアル ///
	// マテリアルバッファの生成
	models_[modelName]->cBufferData_.materialResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	// マテリアルデータのマッピング
	hr = models_[modelName]->cBufferData_.materialResource->Map(
		0, nullptr, reinterpret_cast<void**>(&models_[modelName]->cBufferData_.material));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// マテリアル初期値
	models_[modelName]->cBufferData_.material->color = { 1.0f,1.0f,1.0f,1.0f };
	models_[modelName]->cBufferData_.material->enableLighting = false;
	models_[modelName]->cBufferData_.material->enableHalfLambert = false;
	models_[modelName]->cBufferData_.material->enablePhongReflection = false;
	models_[modelName]->cBufferData_.material->enableBlinnPhongReflection = false;
	models_[modelName]->cBufferData_.material->phongRefShininess = 32.0f;
	models_[modelName]->cBufferData_.material->specularColor = { 1.0f,1.0f,1.0f };
	models_[modelName]->cBufferData_.material->uvTransform = Matrix4x4::MakeIdentity4x4();
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

*									更新処理

////////////////////////////////////////////////////////////////////////////////*/
void Model::Update(const std::string& modelName, const Transform& transform, const Material& material, const PunctualLight& punctualLight) {

	// 頂点バッファへデータ転送
	std::memcpy(models_[modelName]->vertexData,
		ModelManager::GetInstance()->GetModelData(modelName).vertices.data(),
		sizeof(VertexData) * ModelManager::GetInstance()->GetModelData(modelName).vertices.size());

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

	// マテリアルの更新
	models_[modelName]->cBufferData_.material->color = material.color;
	models_[modelName]->cBufferData_.material->enableLighting = material.enableLighting;
	models_[modelName]->cBufferData_.material->enableHalfLambert = material.enableHalfLambert;
	models_[modelName]->cBufferData_.material->enablePhongReflection = material.enablePhongReflection;
	models_[modelName]->cBufferData_.material->enableBlinnPhongReflection = material.enableBlinnPhongReflection;
	models_[modelName]->cBufferData_.material->phongRefShininess = material.phongRefShininess;
	models_[modelName]->cBufferData_.material->specularColor = material.specularColor;
	models_[modelName]->cBufferData_.material->uvTransform = material.uvTransform;

	// パンクチュアルライトの更新
	// Lighting時のみ
	if (models_[modelName]->cBufferData_.material->enableLighting) {

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

	// カメラのワールド座標の更新
	models_[modelName]->cBufferData_.cameraForGPU->worldPosition = MainCamera3D::GetInstance()->GetWorldPos();
#pragma endregion

}




/*////////////////////////////////////////////////////////////////////////////////

*							  頂点バッファセット

////////////////////////////////////////////////////////////////////////////////*/
void Model::SetBufferData(const std::string& modelName, ID3D12GraphicsCommandList* commandList) {

	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &models_[modelName]->vertexBufferView);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, models_[modelName]->cBufferData_.materialResource.Get()->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, models_[modelName]->cBufferData_.matrixResource.Get()->GetGPUVirtualAddress());
	// ライトCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(3, models_[modelName]->cBufferData_.lightResource.Get()->GetGPUVirtualAddress());
	// カメラCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(4, models_[modelName]->cBufferData_.cameraResource.Get()->GetGPUVirtualAddress());
}



/*////////////////////////////////////////////////////////////////////////////////

*								   モデル描画

////////////////////////////////////////////////////////////////////////////////*/
void Model::DrawCall(const std::string& modelName, ID3D12GraphicsCommandList* commandList) {

	commandList->DrawInstanced(UINT(ModelManager::GetInstance()->GetModelData(modelName).vertices.size()), 1, 0, 0);
}