#include "GameScene.h"

#include "SceneManager.h"
#include "ImGuiManager.h"

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


}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Update() {

	/*======================================================*/
	// ImGui

	

	/*======================================================*/
	// 2Dオブジェクト

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

	/*======================================================*/
	// 2Dオブジェクト

	for (const auto& sprite : sprites_) {

		sprite->Draw();
	}


	/*======================================================*/
	// 3Dオブジェクト


}