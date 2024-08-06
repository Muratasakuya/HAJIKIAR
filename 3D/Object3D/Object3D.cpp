#include "Object3D.h"

#include "DXCommon.h"



/*////////////////////////////////////////////////////////////////////////////////

*								  初期化

////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Initialize(DXCommon* dxCommon) {

	assert(dxCommon);

	dxCommon_ = dxCommon;

	camera3D_ = MainCamera3D::GetInstance();
	// 3DCamera初期化
	camera3D_->Initialize();

	// 三角形
	triangle_ = CreateTriangleData(kTriangleVertexNum_);
	// 球
	sphere_ = CreateSphereData(kSphereIndexNum_, kSphereVertexNum_);
}


/*////////////////////////////////////////////////////////////////////////////////

*								三角形生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<Object3D::TriangleData> Object3D::CreateTriangleData(UINT vertexCount) {

	HRESULT hr;
	std::unique_ptr<TriangleData> data = std::make_unique<TriangleData>();

	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertexCount);

		// 頂点バッファの生成
		data->vertexResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeVB);

		// 頂点バッファビューの作成
		data->vertexBufferView.BufferLocation = data->vertexResource->GetGPUVirtualAddress();
		data->vertexBufferView.SizeInBytes = sizeVB;
		data->vertexBufferView.StrideInBytes = sizeof(VertexData);

		// 頂点データのマッピング
		hr = data->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&data->vertexData));
		assert(SUCCEEDED(hr));
	}

	/*////////////////////////////////////////////////////////////////////////////////*/
	// マテリアル
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// マテリアル ///
	// マテリアルバッファの生成
	data->cBufferData_.materialResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	// マテリアルデータのマッピング
	hr = data->cBufferData_.materialResource->Map(
		0, nullptr, reinterpret_cast<void**>(&data->cBufferData_.material));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// マテリアル初期値
	data->cBufferData_.material->color = { 1.0f,1.0f,1.0f,1.0f };
	data->cBufferData_.material->enableLighting = false;
	data->cBufferData_.material->enableHalfLambert = false;
	data->cBufferData_.material->enablePhongReflection = false;
	data->cBufferData_.material->enableBlinnPhongReflection = false;
	data->cBufferData_.material->phongRefShininess = 32.0f;
	data->cBufferData_.material->specularColor = { 1.0f,1.0f,1.0f };
	data->cBufferData_.material->uvTransform = Matrix4x4::MakeIdentity4x4();
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// 座標変換データ
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// 座標変換データ ///
	// 座標変換データバッファの生成
	data->cBufferData_.matrixResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));

	// 座標変換データのマッピング
	hr = data->cBufferData_.materialResource->Map(
		0, nullptr, reinterpret_cast<void**>(&data->cBufferData_.transformationMatrix));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// 座標変換データの初期値
	data->cBufferData_.transformationMatrix->World = Matrix4x4::MakeIdentity4x4();
	data->cBufferData_.transformationMatrix->WVP = Matrix4x4::MakeIdentity4x4();
	data->cBufferData_.transformationMatrix->WorldInverseTranspose = Matrix4x4::MakeIdentity4x4();
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// ライト
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// ライト ///
	// ライトバッファの生成
	data->cBufferData_.lightResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(PunctualLight));

	// ライトデータのマッピング
	hr = data->cBufferData_.lightResource->Map(
		0, nullptr, reinterpret_cast<void**>(&data->cBufferData_.light));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// ライトデータの初期値
	// 平行光源
	data->cBufferData_.light->directional.color = { 1.0f,1.0f,1.0f,1.0f };
	data->cBufferData_.light->directional.direction = { 0.0f,-1.0f,0.0f };
	data->cBufferData_.light->directional.intensity = 1.0f;
	// ポイントライト
	data->cBufferData_.light->point.color = { 1.0f,1.0f,1.0f,1.0f };
	data->cBufferData_.light->point.pos = { 0.0f,0.0f,0.0f };
	data->cBufferData_.light->point.intensity = 1.0f;
	data->cBufferData_.light->point.radius = 5.0f;
	data->cBufferData_.light->point.decay = 1.0f;
	// スポットライト
	data->cBufferData_.light->spot.color = { 1.0f,1.0f,1.0f,1.0f };
	data->cBufferData_.light->spot.pos = { 2.0f,1.0f,0.0f };
	data->cBufferData_.light->spot.intensity = 1.0f;
	data->cBufferData_.light->spot.direction = { -1.0f,-1.0f,0.0f };
	data->cBufferData_.light->spot.distance = 5.0f;
	data->cBufferData_.light->spot.decay = 1.0f;
	data->cBufferData_.light->spot.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	data->cBufferData_.light->spot.cosFalloffStart = std::cos(std::numbers::pi_v<float> / 3.0f);
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// カメラ
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// カメラ ///
	// カメラバッファの生成
	data->cBufferData_.cameraResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(CameraForGPU));

	// カメラデータのマッピング
	hr = data->cBufferData_.cameraResource->Map(
		0, nullptr, reinterpret_cast<void**>(&data->cBufferData_.cameraForGPU));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// カメラデータの初期値
	data->cBufferData_.cameraForGPU->worldPosition = camera3D_->GetWorldPos();
#pragma endregion

	return data;
}



/*////////////////////////////////////////////////////////////////////////////////

*								  球生成

////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<Object3D::SphereData> Object3D::CreateSphereData(UINT vertexCount, UINT indexCount) {

	HRESULT hr;
	std::unique_ptr<SphereData> data = std::make_unique<SphereData>();

	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(VertexData) * vertexCount);

		// 頂点バッファの生成
		data->vertexResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeVB);

		// 頂点バッファビューの作成
		data->vertexBufferView.BufferLocation = data->vertexResource->GetGPUVirtualAddress();
		data->vertexBufferView.SizeInBytes = sizeVB;
		data->vertexBufferView.StrideInBytes = sizeof(VertexData);

		// 頂点データのマッピング
		hr = data->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&data->vertexData));
		assert(SUCCEEDED(hr));
	}

	if (indexCount > 0) {

		// インデックスデータのサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(uint32_t) * indexCount);
		// インデックスバッファの生成
		data->indexResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeIB);

		// インデックスバッファビューの作成
		data->indexBufferView.BufferLocation = data->indexResource->GetGPUVirtualAddress();
		data->indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		data->indexBufferView.SizeInBytes = sizeIB;

		// インデックスバッファのマッピング
		hr = data->indexResource->Map(0, nullptr, reinterpret_cast<void**>(&data->indexData));
		assert(SUCCEEDED(hr));
	}

	/*////////////////////////////////////////////////////////////////////////////////*/
	// マテリアル
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// マテリアル ///
	// マテリアルバッファの生成
	data->cBufferData_.materialResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(Material));

	// マテリアルデータのマッピング
	hr = data->cBufferData_.materialResource->Map(
		0, nullptr, reinterpret_cast<void**>(&data->cBufferData_.material));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// マテリアル初期値
	data->cBufferData_.material->color = { 1.0f,1.0f,1.0f,1.0f };
	data->cBufferData_.material->enableLighting = false;
	data->cBufferData_.material->enableHalfLambert = false;
	data->cBufferData_.material->enablePhongReflection = false;
	data->cBufferData_.material->enableBlinnPhongReflection = false;
	data->cBufferData_.material->phongRefShininess = 32.0f;
	data->cBufferData_.material->specularColor = { 1.0f,1.0f,1.0f };
	data->cBufferData_.material->uvTransform = Matrix4x4::MakeIdentity4x4();
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// 座標変換データ
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// 座標変換データ ///
	// 座標変換データバッファの生成
	data->cBufferData_.matrixResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(TransformationMatrix));

	// 座標変換データのマッピング
	hr = data->cBufferData_.matrixResource->Map(
		0, nullptr, reinterpret_cast<void**>(&data->cBufferData_.transformationMatrix));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// 座標変換データの初期値
	data->cBufferData_.transformationMatrix->World = Matrix4x4::MakeIdentity4x4();
	data->cBufferData_.transformationMatrix->WVP = Matrix4x4::MakeIdentity4x4();
	data->cBufferData_.transformationMatrix->WorldInverseTranspose = Matrix4x4::MakeIdentity4x4();
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// ライト
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// ライト ///
	// ライトバッファの生成
	data->cBufferData_.lightResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(PunctualLight));

	// ライトデータのマッピング
	hr = data->cBufferData_.lightResource->Map(
		0, nullptr, reinterpret_cast<void**>(&data->cBufferData_.light));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// ライトデータの初期値
	// 平行光源
	data->cBufferData_.light->directional.color = { 1.0f,1.0f,1.0f,1.0f };
	data->cBufferData_.light->directional.direction = { 0.0f,-1.0f,0.0f };
	data->cBufferData_.light->directional.intensity = 1.0f;
	// ポイントライト
	data->cBufferData_.light->point.color = { 1.0f,1.0f,1.0f,1.0f };
	data->cBufferData_.light->point.pos = { 0.0f,0.0f,0.0f };
	data->cBufferData_.light->point.intensity = 1.0f;
	data->cBufferData_.light->point.radius = 5.0f;
	data->cBufferData_.light->point.decay = 1.0f;
	// スポットライト
	data->cBufferData_.light->spot.color = { 1.0f,1.0f,1.0f,1.0f };
	data->cBufferData_.light->spot.pos = { 2.0f,1.0f,0.0f };
	data->cBufferData_.light->spot.intensity = 1.0f;
	data->cBufferData_.light->spot.direction = { -1.0f,-1.0f,0.0f };
	data->cBufferData_.light->spot.distance = 5.0f;
	data->cBufferData_.light->spot.decay = 1.0f;
	data->cBufferData_.light->spot.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	data->cBufferData_.light->spot.cosFalloffStart = std::cos(std::numbers::pi_v<float> / 3.0f);
#pragma endregion

	/*////////////////////////////////////////////////////////////////////////////////*/
	// カメラ
	/*////////////////////////////////////////////////////////////////////////////////*/

#pragma region /// カメラ ///
	// カメラバッファの生成
	data->cBufferData_.cameraResource
		= vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(CameraForGPU));

	// カメラデータのマッピング
	hr = data->cBufferData_.cameraResource->Map(
		0, nullptr, reinterpret_cast<void**>(&data->cBufferData_.cameraForGPU));
	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// カメラデータの初期値
	data->cBufferData_.cameraForGPU->worldPosition = camera3D_->GetWorldPos();
#pragma endregion

	return data;
}



/*////////////////////////////////////////////////////////////////////////////////

*								 更新処理

////////////////////////////////////////////////////////////////////////////////*/
void Object3D::Update(const Object3DType& objectType, const Transform& transform, const Material& material, const PunctualLight& punctualLight) {

	// 3DCameraの更新
	camera3D_->ImGuiDraw();
	camera3D_->Update();

	// 各オブジェクトの更新処理
	if (objectType == ObjectTriangle) {

#pragma region /// VertexData ///
		// 法線の計算
		Vector3 normal =
			Vector3::CalculateTriangleNormal(
				// 各頂点座標
				{ -1.0f,-1.0f,0.0f,1.0f }, { 0.0f,1.0f,0.0f,1.0f }, { 1.0f,-1.0f,0.0f,1.0f });

		triangle_->vertexData[0].pos = { -1.0f,-1.0f,0.0f,1.0f };
		triangle_->vertexData[0].texcoord = { 0.0f,1.0f };
		triangle_->vertexData[0].normal = normal;

		triangle_->vertexData[1].pos = { 0.0f,1.0f,0.0f,1.0f };
		triangle_->vertexData[1].texcoord = { 0.5f,0.0f };
		triangle_->vertexData[1].normal = normal;

		triangle_->vertexData[2].pos = { 1.0f,-1.0f,0.0f,1.0f };
		triangle_->vertexData[2].texcoord = { 1.0f,1.0f };
		triangle_->vertexData[2].normal = normal;
#pragma endregion

#pragma region /// ConstBufferの更新 ///

		//Affine
		Matrix4x4 worldMatrix =
			Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 wvpMatrix =
			Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(camera3D_->GetViewMatrix(), camera3D_->GetProjectionMatrix()));
		Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

		// 行列の更新
		triangle_->cBufferData_.transformationMatrix->World = worldMatrix;
		triangle_->cBufferData_.transformationMatrix->WVP = worldMatrix;
		triangle_->cBufferData_.transformationMatrix->WorldInverseTranspose = worldInverseTranspose;

		// マテリアルの更新
		triangle_->cBufferData_.material->color = material.color;
		triangle_->cBufferData_.material->enableLighting = material.enableLighting;
		if (triangle_->cBufferData_.material->enableLighting) {

			triangle_->cBufferData_.material->enableHalfLambert = material.enableHalfLambert;
			triangle_->cBufferData_.material->enablePhongReflection = material.enablePhongReflection;
			triangle_->cBufferData_.material->enableBlinnPhongReflection = material.enableBlinnPhongReflection;
			triangle_->cBufferData_.material->phongRefShininess = material.phongRefShininess;
			triangle_->cBufferData_.material->specularColor = material.specularColor;
		}
		triangle_->cBufferData_.material->uvTransform = material.uvTransform;

		// パンクチュアルライトの更新
		// Lighting時のみ
		if (triangle_->cBufferData_.material->enableLighting) {

			// 平行光源
			triangle_->cBufferData_.light->directional.color = punctualLight.directional.color;
			triangle_->cBufferData_.light->directional.direction = punctualLight.directional.direction;
			triangle_->cBufferData_.light->directional.intensity = punctualLight.directional.intensity;
			// ポイントライト
			triangle_->cBufferData_.light->point.color = punctualLight.point.color;
			triangle_->cBufferData_.light->point.pos = punctualLight.point.pos;
			triangle_->cBufferData_.light->point.intensity = punctualLight.point.intensity;
			triangle_->cBufferData_.light->point.radius = punctualLight.point.radius;
			triangle_->cBufferData_.light->point.decay = punctualLight.point.decay;
			// スポットライト
			triangle_->cBufferData_.light->spot.color = punctualLight.spot.color;
			triangle_->cBufferData_.light->spot.pos = punctualLight.spot.pos;
			triangle_->cBufferData_.light->spot.intensity = punctualLight.spot.intensity;
			triangle_->cBufferData_.light->spot.direction = punctualLight.spot.direction;
			triangle_->cBufferData_.light->spot.distance = punctualLight.spot.distance;
			triangle_->cBufferData_.light->spot.decay = punctualLight.spot.decay;
			triangle_->cBufferData_.light->spot.cosAngle = punctualLight.spot.cosAngle;
			triangle_->cBufferData_.light->spot.cosFalloffStart = punctualLight.spot.cosFalloffStart;
			// カメラのワールド座標の更新
			triangle_->cBufferData_.cameraForGPU->worldPosition = camera3D_->GetWorldPos();
		}
#pragma endregion
	} else if (objectType == ObjectSphere) {

#pragma region // VertexData //
		// 経度分割1つ分の角度
		const float kLonEvery = std::numbers::pi_v<float> *2.0f / float(kSphereSubdivision);
		// 緯度分割1つ分の角度
		const float kLatEvery = std::numbers::pi_v<float> / float(kSphereSubdivision);

		// 緯度方向に分割
		for (uint32_t latIndex = 0; latIndex < kSphereSubdivision; latIndex++) {

			float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;

			// 経度の方向に分割
			for (uint32_t lonIndex = 0; lonIndex < kSphereSubdivision; lonIndex++) {

				uint32_t start = (latIndex * kSphereSubdivision + lonIndex) * 4;
				float lon = lonIndex * kLonEvery + std::numbers::pi_v<float> / 2.0f;

				float u0 = static_cast<float>(lonIndex) / kSphereSubdivision;
				float u1 = static_cast<float>(lonIndex + 1) / kSphereSubdivision;
				float v0 = 1.0f - static_cast<float>(latIndex) / kSphereSubdivision;
				float v1 = 1.0f - static_cast<float>(latIndex + 1) / kSphereSubdivision;

				/*=========================================================================================*/
				// 各頂点の値を書き込む
				// 基準点 a
				sphere_->vertexData[start].pos.x =
					std::cos(lat) * std::cos(lon);

				sphere_->vertexData[start].pos.y =
					std::sin(lat);

				sphere_->vertexData[start].pos.z =
					std::cos(lat) * std::sin(lon);

				sphere_->vertexData[start].pos.w = 1.0f;

				sphere_->vertexData[start].texcoord = { u0, v0 };

				sphere_->vertexData[start].normal.x = sphere_->vertexData[start].pos.x;
				sphere_->vertexData[start].normal.y = sphere_->vertexData[start].pos.y;
				sphere_->vertexData[start].normal.z = sphere_->vertexData[start].pos.z;

				/*=========================================================================================*/
				// 基準点 b
				sphere_->vertexData[start + 1].pos.x =
					std::cos(lat + kLatEvery) * std::cos(lon);

				sphere_->vertexData[start + 1].pos.y =
					std::sin(lat + kLatEvery);

				sphere_->vertexData[start + 1].pos.z =
					std::cos(lat + kLatEvery) * std::sin(lon);

				sphere_->vertexData[start + 1].pos.w = 1.0f;

				sphere_->vertexData[start + 1].texcoord = { u0, v1 };

				sphere_->vertexData[start + 1].normal.x = sphere_->vertexData[start + 1].pos.x;
				sphere_->vertexData[start + 1].normal.y = sphere_->vertexData[start + 1].pos.y;
				sphere_->vertexData[start + 1].normal.z = sphere_->vertexData[start + 1].pos.z;

				/*=========================================================================================*/
				// 基準点 c
				sphere_->vertexData[start + 2].pos.x =
					std::cos(lat) * std::cos(lon + kLonEvery);

				sphere_->vertexData[start + 2].pos.y =
					std::sin(lat);

				sphere_->vertexData[start + 2].pos.z =
					std::cos(lat) * std::sin(lon + kLonEvery);

				sphere_->vertexData[start + 2].pos.w = 1.0f;

				sphere_->vertexData[start + 2].texcoord = { u1, v0 };

				sphere_->vertexData[start + 2].normal.x = sphere_->vertexData[start + 2].pos.x;
				sphere_->vertexData[start + 2].normal.y = sphere_->vertexData[start + 2].pos.y;
				sphere_->vertexData[start + 2].normal.z = sphere_->vertexData[start + 2].pos.z;

				/*=========================================================================================*/
				// 基準点 d
				sphere_->vertexData[start + 3].pos.x =
					std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);

				sphere_->vertexData[start + 3].pos.y =
					std::sin(lat + kLatEvery);

				sphere_->vertexData[start + 3].pos.z =
					std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);

				sphere_->vertexData[start + 3].pos.w = 1.0f;

				sphere_->vertexData[start + 3].texcoord = { u1, v1 };

				sphere_->vertexData[start + 3].normal.x = sphere_->vertexData[start + 3].pos.x;
				sphere_->vertexData[start + 3].normal.y = sphere_->vertexData[start + 3].pos.y;
				sphere_->vertexData[start + 3].normal.z = sphere_->vertexData[start + 3].pos.z;
			}
		}
#pragma endregion

#pragma region // indexData //
		for (uint32_t latIndex = 0; latIndex <= kSphereSubdivision; latIndex++) {
			for (uint32_t lonIndex = 0; lonIndex <= kSphereSubdivision; lonIndex++) {
				uint32_t baseIndex = (latIndex * (kSphereSubdivision)+lonIndex) * 4;

				sphere_->indexData[6 * (latIndex * kSphereSubdivision + lonIndex)] = baseIndex;         // 0
				sphere_->indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 1] = baseIndex + 1; // 1
				sphere_->indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 2] = baseIndex + 2; // 2
				sphere_->indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 3] = baseIndex + 2; // 3
				sphere_->indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 4] = baseIndex + 1; // 4
				sphere_->indexData[6 * (latIndex * kSphereSubdivision + lonIndex) + 5] = baseIndex + 3; // 5
			}
		}
#pragma endregion

#pragma region /// ConstBufferの更新 ///

		//Affine
		Matrix4x4 worldMatrix =
			Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 wvpMatrix =
			Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(camera3D_->GetViewMatrix(), camera3D_->GetProjectionMatrix()));
		Matrix4x4 worldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(worldMatrix));

		// 行列の更新
		sphere_->cBufferData_.transformationMatrix->World = worldMatrix;
		sphere_->cBufferData_.transformationMatrix->WVP = wvpMatrix;
		sphere_->cBufferData_.transformationMatrix->WorldInverseTranspose = worldInverseTranspose;

		// マテリアルの更新
		sphere_->cBufferData_.material->color = material.color;
		sphere_->cBufferData_.material->enableLighting = material.enableLighting;
		sphere_->cBufferData_.material->enableHalfLambert = material.enableHalfLambert;
		sphere_->cBufferData_.material->enablePhongReflection = material.enablePhongReflection;
		sphere_->cBufferData_.material->enableBlinnPhongReflection = material.enableBlinnPhongReflection;
		sphere_->cBufferData_.material->phongRefShininess = material.phongRefShininess;
		sphere_->cBufferData_.material->specularColor = material.specularColor;
		sphere_->cBufferData_.material->uvTransform = material.uvTransform;

		// パンクチュアルライトの更新
		// Lighting時のみ
		if (sphere_->cBufferData_.material->enableLighting) {

			// 平行光源
			sphere_->cBufferData_.light->directional.color = punctualLight.directional.color;
			sphere_->cBufferData_.light->directional.direction = punctualLight.directional.direction;
			sphere_->cBufferData_.light->directional.intensity = punctualLight.directional.intensity;
			// ポイントライト
			sphere_->cBufferData_.light->point.color = punctualLight.point.color;
			sphere_->cBufferData_.light->point.pos = punctualLight.point.pos;
			sphere_->cBufferData_.light->point.intensity = punctualLight.point.intensity;
			sphere_->cBufferData_.light->point.radius = punctualLight.point.radius;
			sphere_->cBufferData_.light->point.decay = punctualLight.point.decay;
			// スポットライト
			sphere_->cBufferData_.light->spot.color = punctualLight.spot.color;
			sphere_->cBufferData_.light->spot.pos = punctualLight.spot.pos;
			sphere_->cBufferData_.light->spot.intensity = punctualLight.spot.intensity;
			sphere_->cBufferData_.light->spot.direction = punctualLight.spot.direction;
			sphere_->cBufferData_.light->spot.distance = punctualLight.spot.distance;
			sphere_->cBufferData_.light->spot.decay = punctualLight.spot.decay;
			sphere_->cBufferData_.light->spot.cosAngle = punctualLight.spot.cosAngle;
			sphere_->cBufferData_.light->spot.cosFalloffStart = punctualLight.spot.cosFalloffStart;
		}

		// カメラのワールド座標の更新
		sphere_->cBufferData_.cameraForGPU->worldPosition = camera3D_->GetWorldPos();
#pragma endregion
	}
}



/*////////////////////////////////////////////////////////////////////////////////

*							  頂点バッファセット

////////////////////////////////////////////////////////////////////////////////*/
void Object3D::SetBufferData(ID3D12GraphicsCommandList* commandList, const Object3DType& objectType) {

	if (objectType == ObjectTriangle) {

		// 頂点バッファの設定
		commandList->IASetVertexBuffers(0, 1, &triangle_->vertexBufferView);
		// マテリアルCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(0, triangle_->cBufferData_.materialResource.Get()->GetGPUVirtualAddress());
		// wvp用のCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(1, triangle_->cBufferData_.matrixResource.Get()->GetGPUVirtualAddress());
		// ライトCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(3, triangle_->cBufferData_.lightResource.Get()->GetGPUVirtualAddress());
		// カメラCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(4, triangle_->cBufferData_.cameraResource.Get()->GetGPUVirtualAddress());
	} else if (objectType == ObjectSphere) {

		// 頂点バッファの設定
		commandList->IASetVertexBuffers(0, 1, &sphere_->vertexBufferView);
		// インデックスバッファの設定
		commandList->IASetIndexBuffer(&sphere_->indexBufferView);
		// マテリアルCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(0, sphere_->cBufferData_.materialResource.Get()->GetGPUVirtualAddress());
		// wvp用のCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(1, sphere_->cBufferData_.matrixResource.Get()->GetGPUVirtualAddress());
		// ライトCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(3, sphere_->cBufferData_.lightResource.Get()->GetGPUVirtualAddress());
		// カメラCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(4, sphere_->cBufferData_.cameraResource.Get()->GetGPUVirtualAddress());
	}
}



/*////////////////////////////////////////////////////////////////////////////////

*								  3Dオブジェクト描画

////////////////////////////////////////////////////////////////////////////////*/
void Object3D::DrawCall(ID3D12GraphicsCommandList* commandList, const Object3DType& objectType) {

	if (objectType == ObjectTriangle) {

		commandList->DrawInstanced(kTriangleVertexNum_, 1, 0, 0);
	} else if (objectType == ObjectSphere) {

		commandList->DrawIndexedInstanced(kSphereVertexNum_, 1, 0, 0, 0);
	}
}