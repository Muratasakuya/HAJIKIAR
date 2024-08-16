#include "DXRasterizerState.h"

// rasterizerDesc_ getter
D3D12_RASTERIZER_DESC DXRasterizerState::GetRasterizerDesc(PipelineType pipelineType) {

	return rasterizerDesc_[pipelineType];
}

/*////////////////////////////////////////////////////////////////////////////////
*							DXRasterizerStateの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXRasterizerState::Create(PipelineType pipelineType) {

	if (pipelineType != Object2D) {

		// 裏面(時計周り)を表示しない、背面カリング
		rasterizerDesc_[pipelineType].CullMode = D3D12_CULL_MODE_BACK;
		//三角形の中を塗りつぶす
		rasterizerDesc_[pipelineType].FillMode = D3D12_FILL_MODE_SOLID;
	} else if (pipelineType == Object2D) {

		// 裏面(時計周り)を表示、背面カリングしない
		rasterizerDesc_[pipelineType].CullMode = D3D12_CULL_MODE_NONE;
		//三角形の中を塗りつぶす
		rasterizerDesc_[pipelineType].FillMode = D3D12_FILL_MODE_SOLID;
	}
}