#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"
#include "PipelineStateStructure.h"

/*////////////////////////////////////////////////////////////////////////////////
*
*								DXDepthStencil Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXDepthStencil{
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	DXDepthStencil() {};
	~DXDepthStencil() {};

	void Create(bool depthEnable);

	// getter

	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{};
};

