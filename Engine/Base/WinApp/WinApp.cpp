#include "WinApp.h"

// imgui
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam
);

#pragma comment(lib,"winmm.lib")



// device_ getter
HWND WinApp::GetHwnd() const {

	return hwnd_;
}

// wc_ getter
WNDCLASS WinApp::GetWindowClass() const {

	return wc_;
}



/*////////////////////////////////////////////////////////////////////////////////

*							メッセージの受け渡し処理

////////////////////////////////////////////////////////////////////////////////*/
bool WinApp::ProcessMessage() {

	MSG msg{};

	// Windowにメッセージが来てたら最優先で処理させる
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// 終了メッセージを受け取ったら
	if (msg.message == WM_QUIT) {

		// フラグをtrueで返す
		return true;
	}

	return false;
}



/*////////////////////////////////////////////////////////////////////////////////

*							  ウィンドウプロシージャ

////////////////////////////////////////////////////////////////////////////////*/
LRESULT CALLBACK WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	// ImGuiでマウス操作を可能にする
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {

		return true;
	}

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}



/*////////////////////////////////////////////////////////////////////////////////

*							ウィンドウクラスを登録する

////////////////////////////////////////////////////////////////////////////////*/
void WinApp::RegisterWindowClass() {

	// ウィンドウプロシージャ(Window Procedure)
	wc_.lpfnWndProc = WindowProc;
	// ウィンドウクラス名
	wc_.lpszClassName = L"CG2WindowClass";
	// インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	// カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc_);
}



/*////////////////////////////////////////////////////////////////////////////////

*								メインウィンドウ作成

////////////////////////////////////////////////////////////////////////////////*/
void WinApp::CreateMainWindow(uint32_t width, uint32_t height) {

	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);

	RegisterWindowClass();

	// ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = {};
	wrc.right = width;
	wrc.bottom = height;

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hwnd_ = CreateWindow(
		L"CG2WindowClass",          // 利用するクラス名
		L"LE2B_26_ムラタ_サクヤ_TR", // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,        // よく見るウィンドウスタイル
		CW_USEDEFAULT,              // 表示X座標(Windowsに任せる)
		CW_USEDEFAULT,              // 表示Y座標(Windowsに任せる)
		wrc.right - wrc.left,       // ウィンドウの横幅
		wrc.bottom - wrc.top,       // ウィンドウの縦幅
		nullptr,                    // 親ウィンドウハンドル
		nullptr,                    // メニューハンドル
		GetModuleHandle(nullptr),   // インスタンスハンドル
		nullptr                     // オプション
	);

	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}