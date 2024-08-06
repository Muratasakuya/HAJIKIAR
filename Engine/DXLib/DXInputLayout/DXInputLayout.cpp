#include "DXInputLayout.h"



// inputLayoutDesc_ getter
D3D12_INPUT_LAYOUT_DESC DXInputLayout::GetInputLayoutDesc(PipelineType pipelineType) {

	return inputLayoutDesc_[pipelineType];
}



/*////////////////////////////////////////////////////////////////////////////////

*								DXInputLayoutの生成

////////////////////////////////////////////////////////////////////////////////*/
void DXInputLayout::Create(PipelineType pipelineType) {

	if (pipelineType == Primitive) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[1]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_[pipelineType] = {};
		inputLayoutDesc_[pipelineType].pInputElementDescs = inputElementDescs;
		inputLayoutDesc_[pipelineType].NumElements = _countof(inputElementDescs);
	} else if (pipelineType == Sprite2D) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[2]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_[pipelineType] = {};
		inputLayoutDesc_[pipelineType].pInputElementDescs = inputElementDescs;
		inputLayoutDesc_[pipelineType].NumElements = _countof(inputElementDescs);
	} else if (pipelineType == Normal) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[3]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[2].SemanticName = "NORMAL";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_[pipelineType] = {};
		inputLayoutDesc_[pipelineType].pInputElementDescs = inputElementDescs;
		inputLayoutDesc_[pipelineType].NumElements = _countof(inputElementDescs);
	} else if (pipelineType == pParticle) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[3]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[2].SemanticName = "COLOR";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_[pipelineType] = {};
		inputLayoutDesc_[pipelineType].pInputElementDescs = inputElementDescs;
		inputLayoutDesc_[pipelineType].NumElements = _countof(inputElementDescs);
	}
}