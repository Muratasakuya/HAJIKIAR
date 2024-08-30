#include "SceneManager.h"

// 解放忘れのチェック
struct LeakChecker {

	~LeakChecker() {

		ComPtr<IDXGIDebug1> debug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {

			debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};

// WindowSize
static const uint32_t windowWidth = 1920;
static const uint32_t windowHeight = 1080;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// メモリリークチェッカー
	LeakChecker leakCheck;

	// メインシステムの初期化
	NewMoon::Initialize(windowWidth, windowHeight);

	///===============================================================================
	/// Load

#pragma region
	// タイトルロゴ
	const std::string titleLogoTextureName = "titleSceneLogoType.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + titleLogoTextureName);
	// クレジット 今はActiveのみ
	const std::string creditTextureName = "titleSceneCredit.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + creditTextureName);
	// スタートの仕方表記文字
	std::string startNotationTextureName = "titleSceneSetBitToStart.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Title/" + startNotationTextureName);

	// 背景画像
	const std::string bgTextureName = "bg.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Backgrounds/" + bgTextureName);
	const std::string gridTextureName = "grid.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Backgrounds/" + gridTextureName);

	// チュートリアル文字
	std::string tutorialChar[3];
	tutorialChar[0] = "tutorialText1.png";
	tutorialChar[1] = "tutorialText2.png";
	tutorialChar[2] = "tutorialText3.png";
	NewMoon::LoadTexture("./Resources/Images/Scenes/Tutorial/" + tutorialChar[0]);
	NewMoon::LoadTexture("./Resources/Images/Scenes/Tutorial/" + tutorialChar[1]);
	NewMoon::LoadTexture("./Resources/Images/Scenes/Tutorial/" + tutorialChar[2]);

	// DebugHajiki
	const std::string playerHajikiTextureName = "playerHajiki.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + playerHajikiTextureName);
	const std::string targetHAJIKIActiveTextureName = "targetHAJIKIActive.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Objects/" + targetHAJIKIActiveTextureName);

	// 波
	const std::string waveTextureName = "wave.png";
	NewMoon::LoadTexture("./Resources/Images/Common/Backgrounds/" + waveTextureName);

	// debug用White
	const std::string whiteTexName = "white.png";
	NewMoon::LoadTexture("./Resources/Images/" + whiteTexName);
	// lineHajiki
	const std::string lineHajikiModelName[2] = { "lineHajiki.gltf" ,"lineHajiki2.gltf" };
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName[0]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineHajikiModelName[1]);
	// line
	const std::string lineModelName = "line.gltf";
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", lineModelName);
	// mainHajiki
	const std::string playerHajikiModelName[2] = { "mainHajiki0.gltf","mainHajiki1.gltf" };
	const std::string targetHajikiModelName[2] = { "mainHajiki2.gltf","mainHajiki3.gltf" };
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", playerHajikiModelName[0]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", playerHajikiModelName[1]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", targetHajikiModelName[0]);
	NewMoon::LoadGltfModel("./Resources/Gltf/Hajiki/", targetHajikiModelName[1]);

	// Cube
	std::string cubeModelName[5];
	cubeModelName[0] = "cube.gltf";
	for (int i = 1; i < 5; ++i) {
		std::ostringstream oss;
		oss << "cube" << i + 1 << ".gltf";
		cubeModelName[i] = oss.str();
	}
	for (uint32_t i = 0; i < 5; i++) {

		NewMoon::LoadGltfModel("./Resources/Gltf/Objects/", cubeModelName[i]);
	}

	// Kirai
	std::string kiraiModelName[5];
	kiraiModelName[0] = "kirai.gltf";
	for (int i = 1; i < 5; ++i) {
		std::ostringstream oss;
		oss << "kirai" << i + 1 << ".gltf";
		kiraiModelName[i] = oss.str();
	}
	for (uint32_t i = 0; i < 5; i++) {

		NewMoon::LoadGltfModel("./Resources/Gltf/Objects/", kiraiModelName[i]);
	}

	const std::string soulModelName = "mainHajiki4.gltf";
	NewMoon::LoadModel("./Resources/Gltf/Hajiki/", soulModelName);
#pragma endregion

	///===============================================================================

	// シーン管理クラスのインスタンス
	SceneManager* sceneManager = SceneManager::GetInstance();

	// メインループ
	sceneManager->Run();

	// ライブラリ終了
	NewMoon::Finalize();

	return 0;
}