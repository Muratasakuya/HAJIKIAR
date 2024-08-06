#pragma once

// DirectX
#include <d3d12.h>

#include "ComPtr.h"

// c++
#include <cassert>
#include <array>

// 前方宣言
class DXCommon;

/*////////////////////////////////////////////////////////////////////////////////
*
*							SrvManager Class
*
////////////////////////////////////////////////////////////////////////////////*/
class SrvManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	uint32_t Allocate();
	bool CanAllocate();

	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	void CreateSRVForStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	void PreDraw();

	void Initialize(DXCommon* dxCommon);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	// getter

	ID3D12DescriptorHeap* GetDescriptorHeap() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	DXCommon* dxCommon_ = nullptr;

	static const uint32_t kMaxSRVCount_;
	uint32_t useIndex_ = 0;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	uint32_t descriptorSize_;

	ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(
		ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible
	);
};