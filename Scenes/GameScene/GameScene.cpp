#include "GameScene.h"

#include "SceneManager.h"
#include "ImGuiManager.h"
#include "Input.h"

/*////////////////////////////////////////////////////////////////////////////////
*								コンストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::GameScene() {}

/*////////////////////////////////////////////////////////////////////////////////
*								  デストラクタ
////////////////////////////////////////////////////////////////////////////////*/
GameScene::~GameScene() {}

/*////////////////////////////////////////////////////////////////////////////////
*								   初期化
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Initialize() {

	// テクスチャロード
	const std::string playerTextureName = "playerHajiki.png";
	const std::string targetTextureName = "targetHAJIKIActive.png";

	Engine::LoadTexture("./Resources/Images/" + playerTextureName);
	Engine::LoadTexture("./Resources/Images/" + targetTextureName);

	// 衝突管理
	// 生成
	collisionManager_ = std::make_unique<CollisionManager>();

	// ImGui
	// 生成
	imguiRenderer_ = std::make_unique<ImGuiRenderer>();

	/*======================================================*/
	// 2Dオブジェクト

	// 2つ
	for (size_t index = 0; index < sprites_.size(); index++) {

		sprites_[index] = std::make_unique<GameObject2D>();
		sprites_[index]->Initialize();
		sprites_[index]->SetPos({ 96.0f + index * 96.0f,96.0f });
		sprites_[index]->SetHalfSize(48.0f);
		sprites_[index]->SetColliderType(ColliderType::Circle);
	}

	sprites_[0]->SetTexture(playerTextureName);
	sprites_[1]->SetTexture(targetTextureName);

	/*======================================================*/
	// 3Dオブジェクト

	/*======================================================*/
	// OpenCV

	// 生成と初期化
	openCV_ = std::make_unique<OpenCV>();
	openCV_->Initialize();
	isCameraOpened_ = false;
	gameStart_ = false;

}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Update() {

	/*======================================================*/
	// ImGui

	/*======================================================*/
	// 入力処理

	// スペースでカメラ起動
	if (!isCameraOpened_) {
		if (Engine::TriggerKey(DIK_SPACE)) {

			openCV_->OpenCamera();
			isCameraOpened_ = true;
		}
	}

	// カメラ起動後エンターキーを押したら
	if (Engine::TriggerKey(DIK_RETURN)) {

		// ゲーム開始 (仮)
		gameStart_ = true;
	}

	// ゲームが始まってから
	if (gameStart_) {

		// 指定した色を複数同時トラッキングする
		openCV_->ColorTracking();
	} else {

		// ゲームが始まるまで
		openCV_->Update();
	}

	/*======================================================*/
	// 2Dオブジェクト

	if (gameStart_) {

		// OpenCVから取得した値を代入
		sprites_[0]->SetPos(openCV_->GetBlueCenterPos());
		sprites_[1]->SetPos(openCV_->GetGreenCenterPos());
	}

	for (const auto& sprite : sprites_) {

		sprite->Update();
	}

	/*======================================================*/
	// 3Dオブジェクト

	

	/*======================================================*/
	// 衝突判定

	// リセット
	collisionManager_->Reset();

	// コライダー追加
	for (const auto& sprite : sprites_) {

		collisionManager_->AddCollider(sprite.get());
	}

	// 衝突判定と応答
	collisionManager_->CheckCollision();
}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Draw() {

	// OpenCV
	openCV_->Draw();

	/*======================================================*/
	// 2Dオブジェクト

	for (const auto& sprite : sprites_) {

		sprite->Draw();
	}

	/*======================================================*/
	// 3Dオブジェクト


}