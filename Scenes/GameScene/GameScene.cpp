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
	const std::string textureName = "mario.png";
	Engine::LoadTexture("./Resources/Images/" + textureName);

	// 衝突管理
	// 生成
	collisionManager_ = std::make_unique<CollisionManager>();

	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	// 生成するインスタンスの数
	const uint32_t sphereNum = 3;

	for (uint32_t index = 0; index < sphereNum; index++) {

		auto sphere = std::make_unique<GameObject3D>(GameObjectType::Sphere);
		sphere->Initialize();
		sphere->SetTexture(textureName);
		sphere->SetPos({ -1.5f + index * 2.0f,0.0f,0.0f });
		sphere->SetHalfSize(1.0f);
		sphere->SetColliderType(ColliderType::Sphere);

		spheres.push_back(std::move(sphere));
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Update() {

	/*======================================================*/
	// ImGui



	/*======================================================*/
	// 2Dオブジェクト



	/*======================================================*/
	// 3Dオブジェクト

	for (const auto& sphere : spheres) {

		sphere->Update();
	}

	/*======================================================*/
	// 衝突判定

	// リセット
	collisionManager_->Reset();

	// コライダー追加
	for (const auto& sphere : spheres) {

		collisionManager_->AddCollider(sphere.get());
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



	/*======================================================*/
	// 3Dオブジェクト

	for (const auto& sphere : spheres) {

		sphere->Draw();
	}

}