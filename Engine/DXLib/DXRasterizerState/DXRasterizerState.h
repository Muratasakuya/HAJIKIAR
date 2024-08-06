#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"
#include "PipelineStateStructure.h"

// c++
#include <array>

/*////////////////////////////////////////////////////////////////////////////////
*								DXBlendState Class
////////////////////////////////////////////////////////////////////////////////*/
class DXRasterizerState {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	DXRasterizerState() = default;
	~DXRasterizerState() = default;

	void Create(PipelineType pipelineType);

	// getter

	D3D12_RASTERIZER_DESC GetRasterizerDesc(PipelineType pipelineType);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// スプライトととの使い分け
	std::array<D3D12_RASTERIZER_DESC, pipelineTypeNum> rasterizerDesc_{};
};