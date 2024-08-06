#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"
#include "PipelineStateStructure.h"

// c++
#include <array>

/*////////////////////////////////////////////////////////////////////////////////
*
*							DXInputLayout Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXInputLayout{
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	DXInputLayout() {};
	~DXInputLayout() {};

	void Create(PipelineType pipelineType);

	// getter

	D3D12_INPUT_LAYOUT_DESC GetInputLayoutDesc(PipelineType pipelineType);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::array<D3D12_INPUT_LAYOUT_DESC, pipelineTypeNum> inputLayoutDesc_{};
};