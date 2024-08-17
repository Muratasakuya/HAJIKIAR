#pragma once

// 2D
#include "Sprite.h"

// 3D
#include "Object3D.h"

// Managers
#include "PipelineManager.h"

// Input
#include "Input.h"

// c++
#include <cstdint>
#include <memory>
#include <array>
#include <iostream>
#include <tuple>
#include <utility>

///===============================================================================
/// クラス前方宣言
#pragma region
class WinApp;
class DXCommon;
class SrvManager;
class ImGuiManager;
class TextureManager;
class ModelManager;
class ParticleManager;
class ParticleEmitter;
class Audio;
#pragma endregion
///===============================================================================

/*////////////////////////////////////////////////////////////////////////////////
*									NewMoon Class
////////////////////////////////////////////////////////////////////////////////*/
class NewMoon {
public:
	/*-----------------------------*/
	///			public変数
	/*-----------------------------*/

	// ウィンドウサイズ
	// int
	static uint32_t kWindowWidthd;
	static uint32_t kWindowHeightd;
	// float
	static float kWindowWidthf;
	static float kWindowHeightf;

public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	NewMoon() = default;
	~NewMoon() = default;

	///===============================================================================
	/// メインシステム

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

	///===============================================================================
	/// 描画関数

	// スプライト
	static void DrawSprite(const Transform2D& transform2D, Vector4 color, const std::string textureName, BlendMode blendMode);

	// 三角形
	static void DrawTriangle(const std::array<Vector3, kTriangleVertexNum_>& vertices,
		const Transform& transform, const Material& material, const PunctualLight& punctualLight,
		const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// 球
	static void DrawSphere(
		const Transform& transform, const Material& material, const PunctualLight& punctualLight,
		const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// モデル
	static void DrawModel(const Transform& transform, const Material& material, const PunctualLight& punctualLight,
		const std::string modelName, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// マルチモデル
	static void DrawMultiModel(const Transform& transform, const std::vector<Material>& materials, const PunctualLight& punctualLight,
		const std::string modelName, const std::string textureName, PipelineType pipelineType, BlendMode blendMode);

	// パーティクル
	static void DrawParticle(const std::string name, const std::string textureName, BlendMode blendMode);

	///===============================================================================
	/// ライブラリ関数

	///-------------------------------------------------------------------------------
	// Load

	// 画像読み込み
	static void LoadTexture(const std::string filePath);
	// テクスチャassert あるかチェック
	static void CheckTextureAvailability(const std::string textureName);

	// モデル読み込み
	static void LoadModel(const std::string& directoryPath, const std::string& filename);
	static void LoadGltfModel(const std::string& directoryPath, const std::string& filename);
	static void MakeModel(ModelData modelData, const std::string& modelName);
	// モデルassert あるかチェック
	static void CheckModelAvailability(const std::string modelName);
	///-------------------------------------------------------------------------------
	// Input

	// キーの押下判定
	static bool PushKey(BYTE keyNumber);
	// キーのトリガー判定
	static bool TriggerKey(BYTE keyNumber);
	// ゲームパッドのボタンの押下判定
	static bool PushGamepadButton(InputGamePadButtons button);
	// ゲームパッドのボタンのトリガー判定
	static bool TriggerGamepadButton(InputGamePadButtons button);
	// Vector2型 左スティックと右スティックの状態を取得
	static Vector2 GetLeftStickVal();
	static Vector2 GetRightStickVal();
	// デッドゾーンの設定
	static void SetDeadZone(float deadZone);
	///-------------------------------------------------------------------------------
	// Screen

	// フルスクリーン設定
	static void SetFullScreenMode(bool fullScreen);
	static bool IsFullScreenMode();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	static std::unique_ptr<WinApp> winApp_;
	static std::unique_ptr<DXCommon> dxCommon_;

	static std::unique_ptr<SrvManager> srvManager_;
	static std::unique_ptr<PipelineManager> pipelineManager_;

	static std::unique_ptr<TextureManager> textureManager_;
	static std::unique_ptr<ModelManager> modelManager_;
	static std::unique_ptr<ParticleManager> particleManager_;

	static std::array<std::unique_ptr<Object3D>, kMaxObject3DNum_> objects3D_;
	static std::array<std::unique_ptr<Sprite>, kMaxSpriteNum_> sprites_;
	static std::unique_ptr<ParticleEmitter> particleEmitter_;

	static std::unique_ptr<ImGuiManager> imguiManager_;
	static std::unique_ptr<Input> input_;
	static std::unique_ptr<Audio> audio_;

	// 3Dオブジェクトの使用インデックス
	static uint32_t indexObject3D_;
	// スプライトの使用インデックス
	static uint32_t indexSprite_;
};