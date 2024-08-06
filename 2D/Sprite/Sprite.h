#pragma once

// DirectX
#include "CBufferStructure.h"
#include "VertexResource.h"

// Lib
#include "MainCamera2D.h"

// c++
#include <array>
#include <cassert>

///===============================================================================
/// クラス前方宣言
#pragma region
class DXCommon;
class TextureManager;
#pragma endregion
///===============================================================================

// スプライト最大数 srvDescriptorの数と同じ
static const uint32_t kMaxSpriteNum_ = 16;
// スプライトの頂点数 Vertex
static const UINT kSpriteVertexNum_ = 6;
// スプライトの頂点数　Index
static const UINT kSpriteIndexNum_ = 4;

/*////////////////////////////////////////////////////////////////////////////////
*									Sprite Class
////////////////////////////////////////////////////////////////////////////////*/
class Sprite {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	Sprite() = default;
	~Sprite() = default;

	// スプライトデータ
	struct SpriteData {

		/*-------------------------------------------------------*/
		// 頂点

		// 頂点バッファ
		ComPtr<ID3D12Resource> vertexResource;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		// インデックス頂点バッファ
		ComPtr<ID3D12Resource> indexResource;
		// インデックス頂点バッファビュー
		D3D12_INDEX_BUFFER_VIEW indexBufferView{};

		// 頂点バッファデータ
		SpriteVertexData* vertexData = nullptr;
		// インデックスバッファデータ
		uint32_t* indexData = nullptr;

		/*-------------------------------------------------------*/
		// マテリアル

		// マテリアルリソース
		ComPtr<ID3D12Resource> materialResource;

		// マテリアルバッファデータ
		SpriteMaterial* materialData = nullptr;

		/*-------------------------------------------------------*/
		// 座標変換

		// 座標変換行列リソース
		ComPtr<ID3D12Resource> matrixResource;

		// 座標変換行列バッファデータ
		SpriteTransformationMatrix* matrixData = nullptr;

	};

	void Initialize(DXCommon* dxCommon, TextureManager* textureManager);

	void Update(const std::string textureName);

	void SetBufferData(ID3D12GraphicsCommandList* commandList);
	void DrawCall(ID3D12GraphicsCommandList* commandList);

	// setter

	void SetTransform2D(const Transform2D& transform2D_);
	void SetColor(const Vector4& color);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	DXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager_ = nullptr;

	// CreateBuffer
	VertexResource vertexResource_;

	// 2DCamera
	MainCamera2D* camera2D_ = nullptr;

	Transform2D transform2D_{};
	Vector4 color_;

	/*--------------------------------------------------------------------*/
	/// スプライト

	// スプライトデータ
	std::unique_ptr<SpriteData> sprite_;

	/*--------------------------------------------------------------------*/

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	// テクスチャのサイズを合わせる
	void AjustTextureSize(const std::string textureName);
	// スプライト生成
	std::unique_ptr<SpriteData> CreateData(UINT vertexCount, UINT indexCount);

};