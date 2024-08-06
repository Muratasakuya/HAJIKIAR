#include "Input.h"

// DirectInput
#pragma comment(lib,"dInput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinApp.h"



// key_<array> getter
const std::array<BYTE, inputKeyMaxNum>& Input::GetAllKey() const {

	return key_;
}



// キーの押下判定
bool Input::PushKey(BYTE keyNumber) {

	// 指定キーを押していればtrue
	if (key_[keyNumber]) {

		return true;
	}

	return false;
}

// キーのトリガー判定
bool Input::TriggerKey(BYTE keyNumber) {

	// 現在のフレームで押されていて、前のフレームで押されていなかった場合にtrueを返す
	if (key_[keyNumber] && !keyPre_[keyNumber]) {

		return true;
	}

	return false;
}



/*////////////////////////////////////////////////////////////////////////////////

*								Inputの初期化

////////////////////////////////////////////////////////////////////////////////*/
void Input::Initialize(WinApp* winApp) {

	HRESULT hr;

	// DirectInputの初期化
	dInput_ = nullptr;
	hr = DirectInput8Create(winApp->GetWindowClass().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput_, nullptr);
	assert(SUCCEEDED(hr));

	// キーボードデバイスの初期化
	keyboard_ = nullptr;
	hr = dInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));

	// 入力データ形式のセット 標準形式
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	// 排他制御レベルのリセット
	hr = keyboard_->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));
}



/*////////////////////////////////////////////////////////////////////////////////

*								Inputの更新処理

////////////////////////////////////////////////////////////////////////////////*/
void Input::Update() {

	HRESULT hr;

	// キーボード情報の取得開始
	hr = keyboard_->Acquire();

	// 前回のキー入力を保存
	std::memcpy(keyPre_.data(), key_.data(), key_.size());

	// 全キーの入力状態を取得する
	hr = keyboard_->GetDeviceState(static_cast<DWORD>(key_.size()), key_.data());

}