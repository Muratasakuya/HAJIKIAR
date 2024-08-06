#include "DXSwapChain.h"

#include "WinApp.h"



// swapChain_ getter
IDXGISwapChain4* DXSwapChain::GetSwapChain() const {

	return swapChain_.Get();
}

// swapChainResources_[num] getter
ID3D12Resource* DXSwapChain::GetSwapChainResources(uint32_t num) const {

	return swapChainResources_[num].Get();
}

// swapChainDesc_ getter
DXGI_SWAP_CHAIN_DESC1& DXSwapChain::GetSwapChainDesc() {

	return swapChainDesc_;
}




/*////////////////////////////////////////////////////////////////////////////////

*							DXSwapChainの初期化

////////////////////////////////////////////////////////////////////////////////*/
void DXSwapChain::Initialize(
	WinApp* winApp, IDXGIFactory7* dxgiFactory, ID3D12CommandQueue* commandQueue,
	uint32_t width, uint32_t height){

	HRESULT hr;

	swapChain_ = nullptr;
	swapChainDesc_.Width = width;                                   // 画面の幅。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Height = height;                                 // 画面の高さ。ウィンドウのクライアント領域を同じものにしておく
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;             // 色の形式
	swapChainDesc_.SampleDesc.Count = 1;                            // マルチサンプルしない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 描画のターゲットとして利用する
	swapChainDesc_.BufferCount = 2;                                 // ダブルバッファ
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;      // モニタにうつしたら、中身を破壊

	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	hr = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, winApp->GetHwnd(), &swapChainDesc_, nullptr, nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));

	assert(SUCCEEDED(hr));

	// swapChainからResourcesを引っ張る
	hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));

	assert(SUCCEEDED(hr));

	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));

	assert(SUCCEEDED(hr));
}