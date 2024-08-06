#pragma once

// DirectX
#include <d3d12.h>
#include "ComPtr.h"
#include "PipelineStateStructure.h"

/*////////////////////////////////////////////////////////////////////////////////
*								DXBlendState Class
////////////////////////////////////////////////////////////////////////////////*/
class DXBlendState {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	DXBlendState() = default;
	~DXBlendState() = default;

	D3D12_RENDER_TARGET_BLEND_DESC Create(BlendMode blendMode);

};