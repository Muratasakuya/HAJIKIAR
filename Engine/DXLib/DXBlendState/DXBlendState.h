#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"
#include "PipelineStateStructure.h"

// c++
#include <array>

/*////////////////////////////////////////////////////////////////////////////////
*
*								DXBlendState Class
*
////////////////////////////////////////////////////////////////////////////////*/
class DXBlendState {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	DXBlendState() {};
	~DXBlendState() {};

	D3D12_RENDER_TARGET_BLEND_DESC Create(BlendMode blendMode);

};