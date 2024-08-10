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
static const uint32_t windowWidth = 1280;
static const uint32_t windowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// メモリリークチェッカー
	LeakChecker leakCheck;

	// メインシステムの初期化
	NewMoon::Initialize(windowWidth, windowHeight);

	// シーン管理クラスのインスタンス
	SceneManager* sceneManager = SceneManager::GetInstance();

	// メインループ
	sceneManager->Run();

	// ライブラリ終了
	NewMoon::Finalize();

	return 0;
}