#pragma once

// Windows
#include <Windows.h>

// Lib
#include "Logger.h"

/*////////////////////////////////////////////////////////////////////////////////
*							Windows Application Class 
////////////////////////////////////////////////////////////////////////////////*/
class WinApp {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	WinApp() = default;
	~WinApp() = default;

	bool ProcessMessage();
	void CreateMainWindow(uint32_t width, uint32_t height);

	// getter

	HWND GetHwnd() const;
	WNDCLASS GetWindowClass() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	// windowHandle
	HWND hwnd_{};
	WNDCLASS wc_{};

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void RegisterWindowClass();
};