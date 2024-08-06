#pragma once

// DirectX
#include <d3d12.h>

#include "ComPtr.h"

// c++
#include <cassert>
#include <array>

// デスクリプタの種類
enum class DescriptorType {

	RTV,
	DSV,
	DescriptorTypeNum
};

// デスクリプタの数
static const uint32_t descriptorNum = static_cast<size_t>(DescriptorType::DescriptorTypeNum);

/*////////////////////////////////////////////////////////////////////////////////
*
*							DXDescriptor Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXDescriptor {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	ComPtr<ID3D12Resource> MakeDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	ComPtr<ID3D12DescriptorHeap> MakeDescriptorHeap(
		DescriptorType descriptorType,
		ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible
	);

	void CreateRTV(ID3D12Device* device, ID3D12Resource* swapChainResources1, ID3D12Resource* swapChainResources2);
	void CreateDSV(ID3D12Device* device, uint32_t width, uint32_t height);

	void Initialize(
		ID3D12Device* device,ID3D12Resource* swapChainResources1, ID3D12Resource* swapChainResources2 ,
		uint32_t width, uint32_t height);

	// getter

	ID3D12DescriptorHeap* GetDSVDescriptorHeap() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandles(uint32_t num);
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::array<ComPtr<ID3D12DescriptorHeap>, descriptorNum> descriptorHeaps_;
	std::array< D3D12_DESCRIPTOR_HEAP_DESC, descriptorNum> descriptorHeapDescs_;

	// RTV
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];

	// DSV
	ComPtr<ID3D12Resource> depthStencilResource_;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle_;
};