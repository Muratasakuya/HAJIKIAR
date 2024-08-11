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
// isFullscreen_ getter
bool WinApp::IsFullscreen() const {

	return isFullscreen_;
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

	windowStyle_ = WS_OVERLAPPEDWINDOW;

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, windowStyle_, false);

	// ウィンドウの生成
	hwnd_ = CreateWindow(
		L"CG2WindowClass",          // 利用するクラス名
		L"HAJIKIAR",                // タイトルバーの文字
		windowStyle_,        // よく見るウィンドウスタイル
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

/*////////////////////////////////////////////////////////////////////////////////
*								フルスクリーン設定
////////////////////////////////////////////////////////////////////////////////*/
void WinApp::SetFullscreen(bool fullscreen) {

	if (isFullscreen_ != fullscreen) {
		if (fullscreen) {

			// 元の状態を覚えておく
			GetWindowRect(hwnd_, &windowRect_);

			// 仮想フルスクリーン化
			SetWindowLong(
				hwnd_, GWL_STYLE,
				windowStyle_ &
				~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

			RECT fullscreenRect{ 0 };
			HMONITOR monitor = MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST);
			MONITORINFO info;
			info.cbSize = sizeof(info);
			GetMonitorInfo(monitor, &info);
			fullscreenRect.right = info.rcMonitor.right - info.rcMonitor.left;
			fullscreenRect.bottom = info.rcMonitor.bottom - info.rcMonitor.top;

			SetWindowPos(
				hwnd_, HWND_TOPMOST, fullscreenRect.left, fullscreenRect.top, fullscreenRect.right,
				fullscreenRect.bottom, SWP_FRAMECHANGED | SWP_NOACTIVATE);
			ShowWindow(hwnd_, SW_MAXIMIZE);

		} else {
			// 通常ウィンドウに戻す
			SetWindowLong(hwnd_, GWL_STYLE, windowStyle_);

			SetWindowPos(
				hwnd_, HWND_NOTOPMOST, windowRect_.left, windowRect_.top,
				windowRect_.right - windowRect_.left, windowRect_.bottom - windowRect_.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE);

			ShowWindow(hwnd_, SW_NORMAL);
		}
	}

	isFullscreen_ = fullscreen;
}