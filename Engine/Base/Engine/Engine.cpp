#include "Engine.h"

#include "WinApp.h"
#include "DXCommon.h"
#include "SrvManager.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Audio.h"

#pragma region /// staticMembers ///
std::unique_ptr<WinApp> Engine::winApp_ = nullptr;
std::unique_ptr<DXCommon> Engine::dxCommon_ = nullptr;
std::unique_ptr<SrvManager> Engine::srvManager_ = nullptr;
ImGuiManager* Engine::imguiManager_ = nullptr;
TextureManager* Engine::textureManger_ = nullptr;
ModelManager* Engine::modelManager_ = nullptr;
ParticleManager* Engine::particleManager_ = nullptr;
Input* Engine::input_ = nullptr;
Audio* Engine::audio_ = nullptr;
std::unique_ptr<Object3D> Engine::objects3D_ = nullptr;
std::array<std::unique_ptr<Sprite>, kMaxSpriteNum_> Engine::sprites_ = { nullptr };
std::unique_ptr<PipelineManager> Engine::pipelineManager_ = nullptr;
uint32_t Engine::indexSprite_ = 0;
#pragma endregion



/*////////////////////////////////////////////////////////////////////////////////

*								 フレーム開始処理

////////////////////////////////////////////////////////////////////////////////*/
void Engine::BeginFrame() {

	input_->Update();
#ifdef _DEBUG
	imguiManager_->Begin();
#endif
	dxCommon_->PreDraw();
	srvManager_->PreDraw();
}



/*////////////////////////////////////////////////////////////////////////////////

*								 フレーム終了処理

////////////////////////////////////////////////////////////////////////////////*/
void Engine::EndFrame() {

#ifdef _DEBUG
	imguiManager_->End();
#endif
	imguiManager_->Draw();
	dxCommon_->PostDraw();

	// 使用カウントリセット
	indexSprite_ = 0;
}



/*////////////////////////////////////////////////////////////////////////////////

*							  メッセージの受け渡し処理

////////////////////////////////////////////////////////////////////////////////*/
bool Engine::ProcessMessage() {

	if (winApp_->ProcessMessage()) {

		return 1;
	} else {

		return 0;
	}
}



/*////////////////////////////////////////////////////////////////////////////////

*									終了処理

////////////////////////////////////////////////////////////////////////////////*/
void Engine::Finalize() {

#ifdef _DEBUG
	imguiManager_->Finalize();
#endif
	dxCommon_->Finalize(winApp_.get());
	dxCommon_.reset();
	winApp_.reset();

	srvManager_.reset();
	objects3D_.reset();
	pipelineManager_.reset();
	for (auto& sprite : sprites_) {
		sprite.reset();
	}
	audio_->Finalize();

	// ComFinalize
	CoUninitialize();
}



/*////////////////////////////////////////////////////////////////////////////////

*								メインシステムの初期化

////////////////////////////////////////////////////////////////////////////////*/
void Engine::Initialize(uint32_t width, uint32_t height) {

	// ComInitialize
#pragma warning(push)
	// C6031の警告無視
#pragma warning(disable:6031)
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#pragma warning(pop)

	/*-----------------------------------------------------------------------*/
	/// WinApp

	// ウィンドウ生成
	winApp_ = std::make_unique<WinApp>();
	winApp_->CreateMainWindow(width, height);
	/*-----------------------------------------------------------------------*/
	/// DXCommon SrvManager

	// DirectXの初期化
	dxCommon_ = std::make_unique<DXCommon>();
	dxCommon_->Initialize(winApp_.get(), width, height);
	// srvの初期化
	srvManager_ = std::make_unique<SrvManager>();
	srvManager_->Initialize(dxCommon_.get());
	/*-----------------------------------------------------------------------*/
	/// TextureManager

	// インスタンスのセット
	textureManger_ = TextureManager::GetInstance();
	textureManger_->SetInstance(dxCommon_.get(), srvManager_.get());
	/*-----------------------------------------------------------------------*/
	/// ImGuiManager

#ifdef _DEBUG
	// ImGuiの初期化
	imguiManager_ = ImGuiManager::GetInstance();
	imguiManager_->Initialize(winApp_.get(), dxCommon_.get(), srvManager_.get());
#endif
	/*-----------------------------------------------------------------------*/
	/// PipelineManager

	// パイプラインの各種初期化
	pipelineManager_ = std::make_unique<PipelineManager>();
	pipelineManager_->CreatePipelineStateObject(dxCommon_.get());
	/*-----------------------------------------------------------------------*/
	/// Input

	// インプットの初期化
	input_ = Input::GetInstance();
	input_->Initialize(winApp_.get());
	/*-----------------------------------------------------------------------*/
	/// Input

	// オーディオ初期化
	audio_ = Audio::GetInstance();
	audio_->Initialize();
	/*-----------------------------------------------------------------------*/
	/// ModelManager

	// モデルメッシュインスタンスの生成
	modelManager_ = ModelManager::GetInstance();
	modelManager_->Initialize(dxCommon_.get());
	/*-----------------------------------------------------------------------*/
	/// Object3D

	// 3Dオブジェクトの初期化
	objects3D_ = std::make_unique<Object3D>();
	objects3D_->Initialize(dxCommon_.get());
	/*-----------------------------------------------------------------------*/
	/// Sprite

	// スプライトの初期化
	for (auto& sprite : sprites_) {
		sprite = std::make_unique<Sprite>();
		sprite->Initialize(dxCommon_.get());
	}
	/*-----------------------------------------------------------------------*/
	/// ParticleManager

	// パーティクル初期化
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Initialize(dxCommon_.get(), srvManager_.get());
}

/*--------------------------------------------------------------------*/
/// 入力関数

// 全てのキーの入力状態取得
void Engine::GetHitKeyStateAll(char* keyState) {

	const std::array<BYTE, inputKeyMaxNum>& keys = input_->GetAllKey();
	std::memcpy(keyState, keys.data(), keys.size());
}

/*------------------------------------------------------------------------------*/
/// 描画関数

// 三角形
void Engine::DrawTriangle(
	const Transform& transform, const Material& material, const PunctualLight& punctualLight,
	const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 更新
	objects3D_->Update(ObjectTriangle, transform, material, punctualLight);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファのセット
	objects3D_->SetBufferData(commandList.Get(), ObjectTriangle);
	if (pipelineType == Normal) {
		// SRVのセット
		textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	}
	// DrawCall
	objects3D_->DrawCall(commandList.Get(), ObjectTriangle);
}

// スプライト
void Engine::DrawSprite(const Transform2D& transform2D, Vector4 color, const std::string textureName, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	auto& sprite = sprites_[indexSprite_];

	// 座標のセット
	sprite->SetTransform2D(transform2D);
	// 色のセット
	sprite->SetColor(color);
	// 更新
	sprite->Update(textureName);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), Sprite2D, blendMode);
	// 頂点バッファのセット
	sprite->SetBufferData(commandList.Get());
	// SRVのセット
	textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	// DrawCall
	sprite->DrawCall(commandList.Get());

	// 使用カウント上昇
	indexSprite_++;
}

// 球
void Engine::DrawSphere(
	const Transform& transform, const Material& material, const PunctualLight& punctualLight,
	const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 更新
	objects3D_->Update(ObjectSphere, transform, material, punctualLight);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファのセット
	objects3D_->SetBufferData(commandList.Get(), ObjectSphere);
	// SRVのセット
	textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	// DrawCall
	objects3D_->DrawCall(commandList.Get(), ObjectSphere);
}

// モデル
void Engine::DrawModel(const Transform& transform, const Material& material, const PunctualLight& punctualLight,
	const std::string modelName, const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// 更新
	modelManager_->Update(modelName, transform, material, punctualLight);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファのセット
	modelManager_->SetBufferData(modelName, commandList.Get());
	// SRVのセット
	textureManger_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	// DrawCall
	modelManager_->DrawCall(modelName, commandList.Get());
}

// パーティクル
void Engine::DrawParticle(const std::string name, const std::string textureName, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pParticle, blendMode);
	// 頂点バッファの設定
	particleManager_->SetBufferData(commandList.Get(), name);
	// SRVのセット
	particleManager_->SetGraphicsRootDescriptorTable(commandList.Get(), name, textureName);
	// DrawCall
	particleManager_->DrawCall(commandList.Get(), name);
}