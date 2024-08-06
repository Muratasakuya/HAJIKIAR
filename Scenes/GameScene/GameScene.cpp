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

	/*======================================================*/
	// Lib その他

	// シングルトン初期化
	input_ = InputManager::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	audio_ = Audio::GetInstance();

	// OpenCV
	// 生成
	openCV_ = std::make_unique<OpenCV>();
	openCV_->Initialize();
	isCameraOpened_ = false;

	// Hash
	// サイズ設定
	hashes_.resize(static_cast<size_t>(Hash::HAJIKI_TYPE::TYPENUM));

	// 設定したサイズ分生成
	for (auto& hash : hashes_) {

		hash = std::make_unique<Hash>();
	}

	// 使用できるHAJIKIのタイプをセット
	hashes_[0]->SetType({ Hash::HAJIKI_TYPE::NORMAL });
	hashes_[1]->SetType({ Hash::HAJIKI_TYPE::FEATHER });
	hashes_[2]->SetType({ Hash::HAJIKI_TYPE::HEAVY });

	/*======================================================*/
	// 2Dオブジェクト

	background_ = std::make_unique<Object2D>();
	background_->Initialize();
	background_->SetSize({ 1920.0f,1080.0f });

	player_ = std::make_unique<Object2D>();
	player_->Initialize();
	player_->SetSize({ 96.0f,96.0f });
	player_->SetPos({ 120.0f,200.0f });

	target_ = std::make_unique<Object2D>();
	target_->Initialize();
	target_->SetSize({ 96.0f,96.0f });
	target_->SetPos({ 320.0f,200.0f });

	// テクスチャ読み込み
	textureManager_->LoadTexture("./Resources/Images/background2.png");
	textureManager_->LoadTexture("./Resources/Images/playerHajiki.png");
	textureManager_->LoadTexture("./Resources/Images/targetHAJIKIActive.png");

	/*======================================================*/
	// 3Dオブジェクト

}



/*////////////////////////////////////////////////////////////////////////////////
*								    更新処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Update() {

	/*======================================================*/
	// シーン管理



	/*======================================================*/
	// ImGui

#pragma region /// ImGui
	//ImGui::Begin("HashType");

	//for (const auto& hash : hashes_) {

	//	// それぞれのタイプのハッシュ値の出力
	//	ImGui::Text("Type: %s, Hash: %s", hash->GetTypeName().c_str(), hash->GetTypeHash().c_str());
	//}

	//// 新しいQRコードデータの取得
	//std::string qrCodeData = openCV_->GetQRCodeData();
	//if (!qrCodeData.empty()) {

	//	// 同じタイプのものがあるかどうかチェック
	//	if (std::find(keepQRCodeData_.begin(), keepQRCodeData_.end(), qrCodeData) == keepQRCodeData_.end()) {
	//		for (const auto& hash : hashes_) {
	//			// 一致するタイプがあれば追加
	//			if (qrCodeData == hash->GetTypeHash()) {

	//				keepQRCodeData_.push_back(qrCodeData);
	//			}
	//		}
	//	}
	//}

	//// QRコードの値を表示
	//for (const auto& data : keepQRCodeData_) {

	//	bool found = false;
	//	for (const auto& hash : hashes_) {
	//		if (data == hash->GetTypeHash()) {

	//			// QRコードの値がハッシュ値と一致する場合、TypeNameを表示
	//			ImGui::Text("QRCodeHash: %s (sameType: %s)", data.c_str(), hash->GetTypeName().c_str());
	//			found = true;

	//			break;
	//		}
	//	}
	//	if (!found) {

	//		// 一致するものがなければTypeNoneでQRコードの値を表示
	//		ImGui::Text("QRCodeHash: %s (sameType: NONE)", data.c_str());
	//	}
	//}

	//ImGui::End();
#pragma endregion

	/*======================================================*/
	// Lib その他

	/*---------------------------------------------------------------------------------------------------------*/
	/// Input処理

	// スペースキーを押したら
	if (!isCameraOpened_) {
		if (input_->GetKeys()[DIK_SPACE] && !input_->GetPreKeys()[DIK_SPACE]) {

			// カメラ起動
			openCV_->OpenCamera();
			isCameraOpened_ = true;
		}
	}

	// カメラ起動後QRコードを読み取ってからQキーを押したら
	if (input_->GetKeys()[DIK_Q] && !input_->GetPreKeys()[DIK_Q]) {

		// ゲーム開始 (仮)
		gameStart_ = true;
	}

	/*---------------------------------------------------------------------------------------------------------*/
	
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
		player_->SetPos(openCV_->GetBlueCenterPos());
		target_->SetPos(openCV_->GetGreenCenterPos());
	}

	/*======================================================*/
	// 3Dオブジェクト



	/*======================================================*/
	// パーティクル



}

/*////////////////////////////////////////////////////////////////////////////////
*								    描画処理
////////////////////////////////////////////////////////////////////////////////*/
void GameScene::Draw() {

	/*======================================================*/
	// Lib その他

	// OpenCV
	openCV_->Draw();

	/*======================================================*/
	// 2Dオブジェクト
	
	background_->Draw("background2.png", kBlendModeNormal);
	player_->Draw("playerHajiki.png",kBlendModeNormal);
	target_->Draw("targetHAJIKIActive.png", kBlendModeNormal);

	/*======================================================*/
	// 3Dオブジェクト



}