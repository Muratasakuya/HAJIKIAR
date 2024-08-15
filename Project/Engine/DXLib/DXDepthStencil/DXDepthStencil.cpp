#include "DXDepthStencil.h"

// depthStencilDesc_ getter
D3D12_DEPTH_STENCIL_DESC DXDepthStencil::GetDepthStencilDesc() {

	return depthStencilDesc_;
}

/*////////////////////////////////////////////////////////////////////////////////
*								DXDepthStencilの生成
////////////////////////////////////////////////////////////////////////////////*/
void DXDepthStencil::Create(PipelineType pipelineType) {

	if (pipelineType != Sprite2D) {

		// Depth機能
		depthStencilDesc_.DepthEnable = true;
		// 書き込みを行う
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		// 比較関数はLessEqual、近ければ描画される
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	} else if (pipelineType == Sprite2D) {

		// Depth機能
		depthStencilDesc_.DepthEnable = false;
		// 書き込みを行う
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		// 比較関数はLessEqual、近ければ描画される
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}
}