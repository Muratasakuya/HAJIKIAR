#pragma once

// Base
#include "IScene.h"

// 2D
#include "Object2D.h"

// 3D
#include "Sphere.h"
#include "Plane.h"
#include "Teapot.h"

// Object
#include "ParticleEmitter.h"

// Managers
#include "InputManager.h"
#include "TextureManager.h"

// Audio
#include "Audio.h"

// Lib
#include "OpenCV.h"
#include "Hash.h"

// c++
#include <memory>
#include <array>
#include <vector>
#include <string>
#include <numbers>

/*////////////////////////////////////////////////////////////////////////////////
*
*								GameScene Class
*
////////////////////////////////////////////////////////////////////////////////*/
class GameScene :
	public IScene {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	GameScene();
	~GameScene();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// 入力
	InputManager* input_ = nullptr;
	// テクスチャ
	TextureManager* textureManager_ = nullptr;
	// オーディオ
	Audio* audio_ = nullptr;

	// OpenCV
	std::unique_ptr<OpenCV> openCV_;
	// 複数のQRコードデータを保持するためのベクター
	std::vector<std::string> keepQRCodeData_;
	// カメラが起動中かどうか
	bool isCameraOpened_;

	// Hash SHA256
	std::vector<std::unique_ptr<Hash>> hashes_;

	// ゲーム開始
	bool gameStart_;

	/*----------------------------------------------------------------------*/
	// 2Dオブジェクト

	std::unique_ptr<Object2D> background_;

	std::unique_ptr<Object2D> player_;
	std::unique_ptr<Object2D> target_;

	/*----------------------------------------------------------------------*/
	// 3Dオブジェクト



	/*----------------------------------------------------------------------*/
	// 3Dモデル



	/*----------------------------------------------------------------------*/
	// パーティクル


	
};