#include "PipelineManager.h"

#include "DXCommon.h"

/*////////////////////////////////////////////////////////////////////////////////
*								 デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
PipelineManager::~PipelineManager() {

	rootSignature_.reset();
	inputLayout_.reset();
	rasterizerState_.reset();
	depthStencil_.reset();
}

// DXCを使用してShaderをCompileする
IDxcBlob* PipelineManager::CompileShader(
	// CompilerするShaderファイルへのパス
	const std::wstring& filePath,
	// Compilerに使用するProfile
	const wchar_t* profile,
	// 初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler
) {

	HRESULT hr;

	///////////////////////////////////////////////////////////
	// 1.hlslファイルを読み込む
	///////////////////////////////////////////////////////////

	// ここからシェーダーをコンパイルする旨をログに出す
	Log(ConvertWString(std::format(L"Begin CompilerShader, path:{}. profile:{}\n", filePath, profile)));
	// hlslファイルを読み込む
	IDxcBlobEncoding* shaderSouce = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSouce);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSouce->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSouce->GetBufferSize();
	// UTF8の文字コードであることを通知
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	///////////////////////////////////////////////////////////
	// 2.Compileする
	///////////////////////////////////////////////////////////

	LPCWSTR arguments[] = {
		filePath.c_str(),          // コンパイル対象のファイル名
		L"-E",L"main",             // エントリーポイントの指定、基本的にmain以外にはしない
		L"-T",profile,             // ShaderProfileの設定
		L"-Zi",L"-Qembed_debug",   // デバッグ用の情報を埋め込む
		L"-Od",                    // 最適化を外しておく
		L"-Zpr",                   // メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,          // 読み込んだファイル
		arguments,                    // コンパイルオプション
		_countof(arguments),          // コンパイルオプションの数
		includeHandler,               // includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)   // コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	///////////////////////////////////////////////////////////
	// 3.警告、エラーが出ていないか確認する
	///////////////////////////////////////////////////////////

	// 警告、エラーが出たらログに出して止める
	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		// 警告、エラー
		assert(false);
	}

	///////////////////////////////////////////////////////////
	// 4.Complie結果を受け取って返す
	///////////////////////////////////////////////////////////

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	Log(ConvertWString(std::format(L"Complie Succeeded, path:{}, profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderSouce->Release();
	shaderResult->Release();
	shaderError->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
}

/*////////////////////////////////////////////////////////////////////////////////
*								  ShaderのCompile
////////////////////////////////////////////////////////////////////////////////*/
void PipelineManager::ShaderCompile(
	DXCommon* dxCommon, PipelineType pipelineType) {

	// 各タイプのシェーダーコンパイル
	if (pipelineType == Primitive) {

		/*------------------------------------------------------------------------------------------------------------------------*/

		//												Primitive Shader

		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Primitive.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Primitive.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == Object2D) {

		/*------------------------------------------------------------------------------------------------------------------------*/

		//												 Object2D Shader

		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object2D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object2D.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == pObject3D) {

		/*------------------------------------------------------------------------------------------------------------------------*/

		//												 Object3D Shader

		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3D.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == Object3DUnTex) {

		/*------------------------------------------------------------------------------------------------------------------------*/

		//												Object3DUnTex Shader

		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3D.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Object3DUnTex.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	} else if (pipelineType == pParticle) {

		/*------------------------------------------------------------------------------------------------------------------------*/

		//												  Particle Shader

		/*------------------------------------------------------------------------------------------------------------------------*/

		// 頂点シェーダ
		vsBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Particle.VS.hlsl", L"vs_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(vsBlob_[pipelineType] != nullptr);

		// ピクセルシェーダ
		psBlob_[pipelineType] = CompileShader(L"./Resources/Shaders/Particle.PS.hlsl", L"ps_6_0",
			dxCommon->GetDxcUtils(), dxCommon->GetDxcCompiler(), dxCommon->GetIncludeHandler());
		assert(psBlob_[pipelineType] != nullptr);
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								PipelineStateの作成
////////////////////////////////////////////////////////////////////////////////*/
void PipelineManager::CreatePipelineState(
	DXCommon* dxCommon, IDxcBlob* vs, IDxcBlob* ps, ID3D12RootSignature* rootSigature, D3D12_INPUT_LAYOUT_DESC inputLayout,
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc, D3D12_RASTERIZER_DESC rasterizerDesc, D3D12_DEPTH_STENCIL_DESC depthStencilDesc,
	PipelineType pipelineType, BlendMode blendMode) {

	HRESULT hr;

	// Pipeline State Objectの生成
	graphicsPipelineStateDesc_.pRootSignature = rootSigature;
	graphicsPipelineStateDesc_.InputLayout = inputLayout;
	graphicsPipelineStateDesc_.VS = { vs->GetBufferPointer(),vs->GetBufferSize() };
	graphicsPipelineStateDesc_.PS = { ps->GetBufferPointer(),ps->GetBufferSize() };
	graphicsPipelineStateDesc_.BlendState.RenderTarget[0] = blendDesc;
	graphicsPipelineStateDesc_.RasterizerState = rasterizerDesc;
	graphicsPipelineStateDesc_.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc_.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 書き込むRTVの情報
	graphicsPipelineStateDesc_.NumRenderTargets = 1;
	graphicsPipelineStateDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ(形状)のタイプ、三角形
	graphicsPipelineStateDesc_.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定
	graphicsPipelineStateDesc_.SampleDesc.Count = 1;
	graphicsPipelineStateDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// 実際に生成
	pipelineStates_[pipelineType][blendMode] = nullptr;
	hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc_,
		IID_PPV_ARGS(&pipelineStates_[pipelineType][blendMode]));
	assert(SUCCEEDED(hr));
}

/*////////////////////////////////////////////////////////////////////////////////
*								PSOの作成
////////////////////////////////////////////////////////////////////////////////*/
void PipelineManager::CreatePipelineStateObject(DXCommon* dxCommon) {

	rootSignature_ = std::make_unique<DXRootSignature>();
	inputLayout_ = std::make_unique<DXInputLayout>();
	rasterizerState_ = std::make_unique<DXRasterizerState>();
	depthStencil_ = std::make_unique<DXDepthStencil>();

	// パイプラインの名前
	pipelineTypes_ = { Primitive, Object2D, pObject3D, Object3DUnTex, pParticle };
	// ブレンドモードの名前
	blendModeTypes = { kBlendModeNone ,kBlendModeNormal ,kBlendModeAdd ,kBlendModeSubtract ,kBlendModeMultiply ,kBlendModeScreen };

	for (PipelineType pipelineType : pipelineTypes_) {

		// ShaderCompile
		ShaderCompile(dxCommon, pipelineType);
	}

	for (PipelineType pipelineType : pipelineTypes_) {
		for (BlendMode blendType : blendModeTypes) {

			// RootSignature
			rootSignature_->Create(dxCommon, pipelineType);

			// InputLayout
			inputLayout_->Create(pipelineType);

			// BlendState
			D3D12_RENDER_TARGET_BLEND_DESC blendState = blendState_.Create(blendType);

			// RasterizerState
			rasterizerState_->Create(pipelineType);

			// depthStencil
			depthStencil_->Create(pipelineType);

			// PipelineState
			CreatePipelineState(
				dxCommon, vsBlob_[pipelineType].Get(), psBlob_[pipelineType].Get(), rootSignature_->GetRootSignature(pipelineType), inputLayout_->GetInputLayoutDesc(pipelineType),
				blendState, rasterizerState_->GetRasterizerDesc(pipelineType), depthStencil_->GetDepthStencilDesc(),
				pipelineType, blendType);
		}
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								パイプラインのセット
////////////////////////////////////////////////////////////////////////////////*/
void PipelineManager::SetGraphicsPipeline(
	ID3D12GraphicsCommandList* commandList, PipelineType pipelineType, BlendMode blendMode) {

	// RootSignatureの設定
	commandList->SetGraphicsRootSignature(rootSignature_->GetRootSignature(pipelineType));

	// PipelineStateの設定
	commandList->SetPipelineState(pipelineStates_[pipelineType][blendMode].Get());
}