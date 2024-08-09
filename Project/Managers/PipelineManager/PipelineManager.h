#pragma once

// DirectX
#include <d3d12.h>
#include <dxgidebug.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

#include "DXRootSignature.h"
#include "DXInputLayout.h"
#include "DXBlendState.h"
#include "DXRasterizerState.h"
#include "DXDepthStencil.h"

// c++
#include <memory>
#include <array>
#include <cassert>

///===============================================================================
/// クラス前方宣言
class DXCommon;
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*							PipelineManager Class
////////////////////////////////////////////////////////////////////////////////*/
class PipelineManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	PipelineManager() {};
	~PipelineManager();

	void CreatePipelineStateObject(DXCommon* dxCommon);

	void SetGraphicsPipeline(ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendMode blendMode);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::array<PipelineType, pipelineTypeNum> pipelineTypes_{};
	std::array<BlendMode, blendModeNum> blendModeTypes{};

	std::unique_ptr<DXRootSignature> rootSignature_;
	std::unique_ptr<DXInputLayout> inputLayout_;
	std::unique_ptr<DXRasterizerState> rasterizerState_;
	std::unique_ptr<DXDepthStencil> depthStencil_;

	DXBlendState blendState_;

	std::array<ComPtr<IDxcBlob>, pipelineTypeNum> vsBlob_;
	std::array<ComPtr<IDxcBlob>, pipelineTypeNum> psBlob_;

	std::array<std::array<ComPtr<ID3D12PipelineState>, blendModeNum>, pipelineTypeNum> pipelineStates_;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	// シェーダのコンパイル
	IDxcBlob* CompileShader(
		// CompilerするShaderファイルへのパス
		const std::wstring& filePath,
		// Compilerに使用するProfile
		const wchar_t* profile,
		// 初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void CreatePipelineState(
		DXCommon* dxCommon, IDxcBlob* vs, IDxcBlob* ps, ID3D12RootSignature* rootSigature, D3D12_INPUT_LAYOUT_DESC inputLayout,
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc, D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_DEPTH_STENCIL_DESC depthStencilDesc,
		PipelineType pipelineType, BlendMode blendMode);

	void ShaderCompile(DXCommon* dxCommon, PipelineType pipelineType);
};