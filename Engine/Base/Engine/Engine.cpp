#include "Engine.h"

#include "WinApp.h"
#include "DXCommon.h"
#include "SrvManager.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Input.h"
#include "Audio.h"

///===============================================================================
/// staticメンバ変数初期化
#pragma region
std::unique_ptr<WinApp> Engine::winApp_ = nullptr;
std::unique_ptr<DXCommon> Engine::dxCommon_ = nullptr;
std::unique_ptr<SrvManager> Engine::srvManager_ = nullptr;
std::unique_ptr<ImGuiManager> Engine::imguiManager_ = nullptr;
std::unique_ptr<TextureManager> Engine::textureManager_ = nullptr;
std::unique_ptr<ModelManager> Engine::modelManager_ = nullptr;
std::unique_ptr<ParticleManager> Engine::particleManager_ = nullptr;
std::unique_ptr<ParticleEmitter> Engine::particleEmitter_ = nullptr;
std::unique_ptr<Input> Engine::input_ = nullptr;
std::unique_ptr<Audio> Engine::audio_ = nullptr;
std::array<std::unique_ptr<Object3D>, kMaxObject3DNum_> Engine::objects3D_ = { nullptr };
std::array<std::unique_ptr<Sprite>, kMaxSpriteNum_> Engine::sprites_ = { nullptr };
std::unique_ptr<PipelineManager> Engine::pipelineManager_ = nullptr;
uint32_t Engine::indexObject3D_ = 0;
uint32_t Engine::indexSprite_ = 0;
#pragma endregion
///===============================================================================



///===============================================================================
/// メインシステム

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
	indexObject3D_ = 0;
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

	// 手動で解放
	dxCommon_->Finalize(winApp_.get());
	dxCommon_.reset();
	winApp_.reset();

	srvManager_.reset();
	pipelineManager_.reset();

	textureManager_.reset();
	modelManager_.reset();
	particleManager_.reset();

	for (auto& object3D : objects3D_) {
		object3D.reset();
	}
	for (auto& sprite : sprites_) {
		sprite.reset();
	}
	particleEmitter_.reset();

	imguiManager_.reset();
	input_.reset();
	audio_->Finalize();
	audio_.reset();

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
	textureManager_ = std::make_unique<TextureManager>();
	textureManager_->Initialize(dxCommon_.get(), srvManager_.get());
	/*-----------------------------------------------------------------------*/
	/// ImGuiManager

#ifdef _DEBUG
	// ImGuiの初期化
	imguiManager_ = std::make_unique<ImGuiManager>();
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
	input_ = std::make_unique<Input>();
	input_->Initialize(winApp_.get());
	/*-----------------------------------------------------------------------*/
	/// Input

	// オーディオ初期化
	audio_ = std::make_unique<Audio>();
	audio_->Initialize();
	/*-----------------------------------------------------------------------*/
	/// ModelManager

	// モデル初期化
	modelManager_ = std::make_unique<ModelManager>();
	modelManager_->Initialize(dxCommon_.get());
	/*-----------------------------------------------------------------------*/
	/// Object3D

	// 3Dオブジェクトの初期化
	for (auto& objects3D : objects3D_) {
		objects3D = std::make_unique<Object3D>();
		objects3D->Initialize(dxCommon_.get());
	}
	/*-----------------------------------------------------------------------*/
	/// Sprite

	// スプライトの初期化
	for (auto& sprite : sprites_) {
		sprite = std::make_unique<Sprite>();
		sprite->Initialize(dxCommon_.get(), textureManager_.get());
	}
	/*-----------------------------------------------------------------------*/
	/// Particle

	// パーティクル初期化
	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_->Initialize(dxCommon_.get(), srvManager_.get(), textureManager_.get());

	// エミッタ初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleEmitter_->Initialize(particleManager_.get());
}



///===============================================================================
/// 描画関数

/*////////////////////////////////////////////////////////////////////////////////
*									三角形
////////////////////////////////////////////////////////////////////////////////*/
void Engine::DrawTriangle(
	const Transform& transform, const Material& material, const PunctualLight& punctualLight,
	const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	auto& object3D = objects3D_[indexObject3D_];

	// 更新
	object3D->Update(ObjectTriangle, transform, material, punctualLight);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファのセット
	object3D->SetBufferData(commandList.Get(), ObjectTriangle);
	if (pipelineType == Normal) {
		// SRVのセット
		textureManager_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	}
	// DrawCall
	object3D->DrawCall(commandList.Get(), ObjectTriangle);

	// 使用カウント上昇
	indexObject3D_++;
}

/*////////////////////////////////////////////////////////////////////////////////
*								   スプライト
////////////////////////////////////////////////////////////////////////////////*/
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
	textureManager_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	// DrawCall
	sprite->DrawCall(commandList.Get());

	// 使用カウント上昇
	indexSprite_++;
}

/*////////////////////////////////////////////////////////////////////////////////
*									  球
////////////////////////////////////////////////////////////////////////////////*/
void Engine::DrawSphere(
	const Transform& transform, const Material& material, const PunctualLight& punctualLight,
	const std::string textureName, PipelineType pipelineType, BlendMode blendMode) {

	// CommandListをdxCommonClassからもってくる
	ComPtr<ID3D12GraphicsCommandList> commandList = dxCommon_->GetCommandList();

	auto& object3D = objects3D_[indexObject3D_];

	// 更新
	object3D->Update(ObjectSphere, transform, material, punctualLight);
	// パイプラインのセット
	pipelineManager_->SetGraphicsPipeline(commandList.Get(), pipelineType, blendMode);
	// 頂点バッファのセット
	object3D->SetBufferData(commandList.Get(), ObjectSphere);
	// SRVのセット
	textureManager_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	// DrawCall
	object3D->DrawCall(commandList.Get(), ObjectSphere);

	// 使用カウント上昇
	indexObject3D_++;
}

/*////////////////////////////////////////////////////////////////////////////////
*									 モデル
////////////////////////////////////////////////////////////////////////////////*/
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
	textureManager_->SetGraphicsRootDescriptorTable(commandList.Get(), 2, textureName);
	// DrawCall
	modelManager_->DrawCall(modelName, commandList.Get());
}

/*////////////////////////////////////////////////////////////////////////////////
*									 パーティクル
////////////////////////////////////////////////////////////////////////////////*/
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



///===============================================================================
/// ライブラリ関数

/*////////////////////////////////////////////////////////////////////////////////
*									 画像読み込み
////////////////////////////////////////////////////////////////////////////////*/
void Engine::LoadTexture(const std::string filePath) {

	textureManager_->LoadTexture(filePath);
}

/*////////////////////////////////////////////////////////////////////////////////
*							  テクスチャassert あるかチェック
////////////////////////////////////////////////////////////////////////////////*/
void Engine::CheckTextureAvailability(const std::string textureName) {

	textureManager_->CheckAvailability(textureName);
}

/*////////////////////////////////////////////////////////////////////////////////
*								モデルassert あるかチェック
////////////////////////////////////////////////////////////////////////////////*/
void Engine::CheckModelAvailability(const std::string modelName) {

	modelManager_->CheckAvailability(modelName);
}