#include "DXDevice.h"

// device_ getter
ID3D12Device* DXDevice::GetDevice() const {

	return device_.Get();
}

// dxgiFactory_ getter
IDXGIFactory7* DXDevice::GetDxgiFactory() const {

	return dxgiFactory_.Get();
}

/*////////////////////////////////////////////////////////////////////////////////
*							DXDeviceの初期化
////////////////////////////////////////////////////////////////////////////////*/
void DXDevice::Initialize() {

	HRESULT hr;

	// DXGIファクトリーの生成
	dxgiFactory_ = nullptr;

	// HRESULTはWindows系のエラーコードであり、
	// 関数が成功したかどうかをSUCCEEDEDマクロで判定できる
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	// 初期化の根本的な部分でエラーが出た場合はプログラムが間違っているか、
	// どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(hr));

	// 使用するアダプタ(GPU)を決定する

	// 使用するアダプタ用の変数、最初にnullptrを入れておく
	useAdapter_ = nullptr;

	// 良い順にアダプタを頼む
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(
			i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND; ++i) {

		// アダプタの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr)); // 取得できないなんてことはない → 一大事

		// ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {

			// forループを抜ける
			break;
		}

		// ソフトウェアアダプタの場合は見なかったことにする
		useAdapter_ = nullptr;
	}

	// 適切なアダプタが見つからなかったので起動できない
	assert(useAdapter_ != nullptr);

	// D3D12Deviceの生成

	// 最初にnullptrを入れておく
	device_ = nullptr;

	// 機能レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featuerLevels[] = {
		D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
	};
	const char* featuerLevelStrings[] = { "12.2","12.1","12.0" };

	// 高い順に生成出来るか試していく
	for (size_t i = 0; i < _countof(featuerLevels); i++) {

		// 採用したアダプタでデバイスを生成
		hr = D3D12CreateDevice(useAdapter_.Get(), featuerLevels[i], IID_PPV_ARGS(&device_));

		// 指定した機能レベルでデバイスを生成できたかを確認
		if (SUCCEEDED(hr)) {

			// ループを抜ける
			break;
		}
	}

	// デバイスの生成がうまくいかなかったので起動できない
	assert(device_ != nullptr);
}
