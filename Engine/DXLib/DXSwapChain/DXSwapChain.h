#pragma once
#pragma once

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

#include "ComPtr.h"

// c++
#include <cstdint>
#include <cassert>

// 前方宣言
class WinApp;

/*////////////////////////////////////////////////////////////////////////////////
*
*							DXSwapChain Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXSwapChain {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void Initialize(
		WinApp* winApp, IDXGIFactory7* dxgiFactory, ID3D12CommandQueue* commandQueue,
		uint32_t width, uint32_t height);

	// getter

	IDXGISwapChain4* GetSwapChain() const;
	ID3D12Resource* GetSwapChainResources(uint32_t num) const;
	DXGI_SWAP_CHAIN_DESC1& GetSwapChainDesc();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	ComPtr<IDXGISwapChain4> swapChain_;
	ComPtr<ID3D12Resource> swapChainResources_[2];
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
};