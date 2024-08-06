#pragma once

// DirectX
#include <d3d12.h>
#include <dxgi1_6.h>

#include "ComPtr.h"

// c++
#include <cassert>

/*////////////////////////////////////////////////////////////////////////////////
*
*							DXDevice Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXDevice {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	void Initialize();

	// getter

	ID3D12Device* GetDevice() const ;
	IDXGIFactory7* GetDxgiFactory() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	ComPtr<ID3D12Device> device_;
	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<IDXGIAdapter4> useAdapter_;
};

