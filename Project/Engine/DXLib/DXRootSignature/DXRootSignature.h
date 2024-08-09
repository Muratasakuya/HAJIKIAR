#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"
#include "PipelineStateStructure.h"

// Lib
#include "Logger.h"

// c++
#include <array>

///===============================================================================
/// クラス前方宣言
class DXCommon;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							DXRootSignature Class
////////////////////////////////////////////////////////////////////////////////*/
class DXRootSignature {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	DXRootSignature() = default;
	~DXRootSignature() = default;

	void Create(DXCommon* dxCommon, PipelineType pipelineType);

	// getter

	ID3D12RootSignature* GetRootSignature(PipelineType pipelineType) const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::array<ComPtr<ID3D12RootSignature>, pipelineTypeNum> rootSignature_ = { nullptr,nullptr };
	ComPtr<ID3DBlob> signatureBlob_ = nullptr;
	ComPtr<ID3DBlob> errorBlob_ = nullptr;
};