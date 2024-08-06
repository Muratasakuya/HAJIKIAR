#include "DXDepthStencil.h"



// depthStencilDesc_ getter
D3D12_DEPTH_STENCIL_DESC DXDepthStencil::GetDepthStencilDesc() {

	return depthStencilDesc_;
}



/*////////////////////////////////////////////////////////////////////////////////

*								DXDepthStencilの生成

////////////////////////////////////////////////////////////////////////////////*/
void DXDepthStencil::Create(bool depthEnable) {

	// Depth機能
	depthStencilDesc_.DepthEnable = depthEnable;
	// 書き込みを行う
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual、近ければ描画される
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
}