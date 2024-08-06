#pragma once

// DirectX
#include <d3d12.h>
#include <DirectXTex.h>

#include "ComPtr.h"

// Lib
#include "Logger.h"

// c++
#include <unordered_map>
#include <string>
#include <filesystem>
#include <vector>

///===============================================================================
/// クラス前方宣言
class DXCommon;
class SrvManager;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							TextureManager Class
////////////////////////////////////////////////////////////////////////////////*/
class TextureManager{
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	TextureManager() = default;
	~TextureManager() = default;

	void LoadTexture(const std::string& filePath);

	void SetGraphicsRootDescriptorTable(
		ID3D12GraphicsCommandList* commandList, UINT rootParamaterIndex, std::string identifier);

	void Initialize(DXCommon* dxCommon, SrvManager* srvManager);

	// getter

	const DirectX::TexMetadata& GetMetaData(const std::string textureName);
	
private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	DXCommon* dxCommon_ = nullptr;
	SrvManager* srvManager_ = nullptr;

	struct TextureData {

		ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
		DirectX::TexMetadata metadata;
		uint32_t srvIndex;
	};

	std::unordered_map<std::string, TextureData> textures_;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	DirectX::ScratchImage Load(const std::string& filePath);
	ComPtr<ID3D12Resource> CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
	
};