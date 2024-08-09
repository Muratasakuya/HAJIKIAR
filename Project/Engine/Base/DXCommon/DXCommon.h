#pragma once

// DirectX
#include <dxgidebug.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

#include "DXDevice.h"
#include "DXSwapChain.h"
#include "DXDescriptor.h"
#include "ComPtr.h"

// c++
#include <cstdint>
#include <memory>
#include <cassert>
#include <chrono>
#include <thread>

///===============================================================================
/// クラス前方宣言
class WinApp;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							DirectXCommon Class
////////////////////////////////////////////////////////////////////////////////*/
class DXCommon {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	DXCommon() = default;
	~DXCommon() = default;

	void Initialize(WinApp* winApp, uint32_t width, uint32_t height);

	void PreDraw();
	void PostDraw();

	void Finalize(WinApp* winApp);

	// getter

	ID3D12Device* GetDevice() const;
	ID3D12GraphicsCommandList* GetCommandList() const;
	IDxcUtils* GetDxcUtils() const;
	IDxcCompiler3* GetDxcCompiler() const;
	IDxcIncludeHandler* GetIncludeHandler() const;
	DXGI_SWAP_CHAIN_DESC1& GetSwapChainDesc();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	HRESULT hr_;

	int32_t kClientWidth_;
	int32_t kClientHeight_;

	std::chrono::steady_clock::time_point reference_;

	UINT backBufferIndex_;

	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};

	uint64_t fenceValue_;
	HANDLE fenceEvent_;

	std::unique_ptr<DXDevice> device_;
	std::unique_ptr<DXSwapChain> swapChain_;
	std::unique_ptr<DXDescriptor> descriptor_;

#pragma region /// DXComObject ///
	ComPtr<ID3D12Fence> fence_;
	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
#pragma endregion

#pragma region /// DXObject ///

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};
	D3D12_RESOURCE_BARRIER barrier_{};

#pragma endregion

	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void DebugLayer();
	void DebugInfo();

	void CreateFenceEvent();
	void InitializeDXCompiler();
	void CreateCommand();
	void TransitionBarrier();
	void ClearWindow();

	void InitializeFixFPS();
	void UpdateFixFPS();
};

