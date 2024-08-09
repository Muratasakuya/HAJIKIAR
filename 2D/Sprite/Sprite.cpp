#include "Sprite.h"

#include "DXCommon.h"
#include "TextureManager.h"

// this->transform2D_ = transform2D setter
void Sprite::SetTransform2D(const Transform2D& transform2D) {

	this->transform2D_ = transform2D;
}

// color_ = color setter
void Sprite::SetColor(const Vector4& color) {

	this->color_ = color;
}

/*////////////////////////////////////////////////////////////////////////////////
*							スプライトメッシュの生成
////////////////////////////////////////////////////////////////////////////////*/
void Sprite::Initialize(DXCommon* dxCommon, TextureManager* textureManager) {

	assert(dxCommon);
	assert(textureManager);

	dxCommon_ = dxCommon;
	textureManager_ = textureManager;

	// スプライト
	sprite_ = CreateData(kSpriteIndexNum_, kSpriteVertexNum_);

	camera2D_ = MainCamera2D::GetInstance();
	// 2DCamera初期化
	camera2D_->Initialize();
}

/*////////////////////////////////////////////////////////////////////////////////
*							スプライトメッシュの生成
////////////////////////////////////////////////////////////////////////////////*/
std::unique_ptr<Sprite::SpriteData> Sprite::CreateData(UINT vertexCount, UINT indexCount) {

	HRESULT hr;
	std::unique_ptr<SpriteData> data = std::make_unique<SpriteData>();

	if (vertexCount > 0) {

		// 頂点データサイズ
		UINT sizeVB = static_cast<UINT>(sizeof(SpriteVertexData) * vertexCount);

		// 頂点バッファの生成
		data->vertexResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeVB);

		// 頂点バッファビューの作成
		data->vertexBufferView.BufferLocation = data->vertexResource->GetGPUVirtualAddress();
		data->vertexBufferView.SizeInBytes = sizeVB;
		data->vertexBufferView.StrideInBytes = sizeof(SpriteVertexData);

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

	// マテリアルバッファの生成
	data->materialResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(SpriteMaterial));

	// マテリアルデータのマッピング
	hr = data->materialResource->Map(0, nullptr, reinterpret_cast<void**>(&data->materialData));

	// マテリアル初期値
	data->materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	data->materialData->uvTransform = Matrix4x4::MakeIdentity4x4();

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	// 座標変換行列バッファの生成
	data->matrixResource = vertexResource_.CreateBufferResource(dxCommon_->GetDevice(), sizeof(SpriteTransformationMatrix));

	// 座標変換データのマッピング
	hr = data->matrixResource->Map(0, nullptr, reinterpret_cast<void**>(&data->matrixData));

	// マテリアル初期値
	data->matrixData->WVP = Matrix4x4::MakeIdentity4x4();

	// 作れなければエラー
	assert(SUCCEEDED(hr));

	return data;
}

/*////////////////////////////////////////////////////////////////////////////////
*							テクスチャサイズをイメージに合わせる
////////////////////////////////////////////////////////////////////////////////*/
void Sprite::AjustTextureSize(const std::string textureName) {

	const DirectX::TexMetadata& metaData = textureManager_->GetMetaData(textureName);

	transform2D_.textureSize.x = static_cast<float>(metaData.width);
	transform2D_.textureSize.y = static_cast<float>(metaData.height);

	// 画像サイズをテクスチャサイズに合わせる
	transform2D_.size = transform2D_.textureSize;
}

/*////////////////////////////////////////////////////////////////////////////////
*							スプライト頂点データの作成
////////////////////////////////////////////////////////////////////////////////*/
void Sprite::Update(const std::string textureName) {

	// アンカーポイント
	float left = 0.0f - transform2D_.anchorPoint.x;
	float right = 1.0f - transform2D_.anchorPoint.x;
	float top = 0.0f - transform2D_.anchorPoint.y;
	float bottom = 1.0f - transform2D_.anchorPoint.y;

	// 左右反転
	if (transform2D_.isFlipX) {

		left = -left;
		right = -right;
	}
	// 上下反転
	if (transform2D_.isFlipY) {

		top = -top;
		bottom = -bottom;
	}

	// メタデータ取得
	const DirectX::TexMetadata& metadata = textureManager_->GetMetaData(textureName);

	// サイズを合わせる
	AjustTextureSize(textureName);

	// 横
	float texLeft = transform2D_.textureLeftTop.x / metadata.width;
	float texRight =
		(transform2D_.textureLeftTop.x + transform2D_.textureSize.x) / metadata.width;
	// 縦
	float texTop = transform2D_.textureLeftTop.y / metadata.height;
	float texBottom =
		(transform2D_.textureLeftTop.y + transform2D_.textureSize.y) / metadata.height;

#pragma region // VertexData //
	// 1=3、2=5、頂点4つで描画
	// 左下
	sprite_->vertexData[0].pos = { left,bottom };
	sprite_->vertexData[0].texcoord = { texLeft,texBottom };
	// 左上
	sprite_->vertexData[1].pos = { left,top };
	sprite_->vertexData[1].texcoord = { texLeft,texTop };
	// 右下
	sprite_->vertexData[2].pos = { right,bottom };
	sprite_->vertexData[2].texcoord = { texRight,texBottom };
	// 右上
	sprite_->vertexData[3].pos = { right,top };
	sprite_->vertexData[3].texcoord = { texRight,texTop };
#pragma endregion

#pragma region // IndexData //
	sprite_->indexData[0] = 0;
	sprite_->indexData[1] = 1;
	sprite_->indexData[2] = 2;
	sprite_->indexData[3] = 1;
	sprite_->indexData[4] = 3;
	sprite_->indexData[5] = 2;
#pragma endregion

	// 2DCamera更新
	camera2D_->Update();

	Transform transform;
	transform.scale = { transform2D_.size.x,transform2D_.size.y,1.0f };
	transform.rotate = { 0.0f,0.0f,transform2D_.rotate };
	transform.translate = { transform2D_.pos.x,transform2D_.pos.y,0.0f };

	//Affine
	Matrix4x4 worldMatrix =
		Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	Matrix4x4 wvpMatrix =
		Matrix4x4::Multiply(worldMatrix, Matrix4x4::Multiply(camera2D_->GetViewMatrix(), camera2D_->GetOrthoMatrix()));

	// 行列の更新
	sprite_->matrixData->WVP = wvpMatrix;

	// 色の更新
	sprite_->materialData->color = color_;
	sprite_->materialData->uvTransform = Matrix4x4::MakeIdentity4x4();
}

/*////////////////////////////////////////////////////////////////////////////////
*								頂点バッファセット
////////////////////////////////////////////////////////////////////////////////*/
void Sprite::SetBufferData(ID3D12GraphicsCommandList* commandList) {

	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &sprite_->vertexBufferView);
	// インデックスバッファの設定
	commandList->IASetIndexBuffer(&sprite_->indexBufferView);
	// マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, sprite_->materialResource.Get()->GetGPUVirtualAddress());
	// wvp用のCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, sprite_->matrixResource.Get()->GetGPUVirtualAddress());
}

/*////////////////////////////////////////////////////////////////////////////////
*								  スプライト描画
////////////////////////////////////////////////////////////////////////////////*/
void Sprite::DrawCall(ID3D12GraphicsCommandList* commandList) {

	commandList->DrawIndexedInstanced(kSpriteVertexNum_, 1, 0, 0, 0);
}