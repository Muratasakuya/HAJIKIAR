#pragma once

// 2D
#include "Sprite.h"

// 3D
#include "Object3D.h"

// Managers
#include "PipelineManager.h"

// c++
#include <cstdint>
#include <memory>
#include <array>
#include <iostream>
#include <tuple>
#include <utility>

// 前方宣言
class WinApp;
class DXCommon;
class SrvManager;
class ImGuiManager;
class TextureManager;
class ModelManager;
class ParticleManager;
class Audio;
class Input;

/*////////////////////////////////////////////////////////////////////////////////
*
*									Engine Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Engine {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// 各システムの初期化
	static void Initialize(uint32_t width, uint32_t height);

	// フレーム開始処理
	static void BeginFrame();

	// フレーム終了処理
	static void EndFrame();

	// システムの終了
	static void Finalize();

	// メッセージの受け渡し処理
	static bool ProcessMessage();

	/*--------------------------------------------------------------------*/
	/// 入力関数

	// 全てのキーの入力状態取得
	static void GetHitKeyStateAll(char* keyState);

	/*--------------------------------------------------------------------*/
	/// 描画関数

	// スプライト
	static void DrawSprite(const Transform2D& transform2D, Vector4 color, const std::string textureName, BlendMode blendMode);

	// 三角形
	static void DrawTriangle(
		const Transform& transform, const Material& material, const PunctualLight& punctualLight,
		const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// 球
	static void DrawSphere(
		const Transform& transform, const Material& material, const PunctualLight& punctualLight,
		const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// モデル
	static void DrawModel(const Transform& transform, const Material& material, const PunctualLight& punctualLight, 
		const std::string modelName, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// パーティクル
	static void DrawParticle(const std::string name, const std::string textureName, BlendMode blendMode);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	static std::unique_ptr<WinApp> winApp_;
	static std::unique_ptr<DXCommon> dxCommon_;
	static std::unique_ptr<SrvManager> srvManager_;
	static std::unique_ptr<Object3D> objects3D_;
	static std::array<std::unique_ptr<Sprite>, kMaxSpriteNum_> sprites_;
	static std::unique_ptr<PipelineManager> pipelineManager_;

	static Input* input_;
	static Audio* audio_;
	static ImGuiManager* imguiManager_;
	static TextureManager* textureManger_;
	static ModelManager* modelManager_;
	static ParticleManager* particleManager_;

	// スプライトの使用インデックス
	static uint32_t indexSprite_;
};