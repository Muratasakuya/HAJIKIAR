#pragma once

// DirectInput
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// DXLib
#include "ComPtr.h"

// c++
#include <array>
#include <cassert>
#include <cstdint>
#include <string>

// 前方宣言
class WinApp;

// キーの数
static const uint32_t inputKeyMaxNum = 256;

/*////////////////////////////////////////////////////////////////////////////////
*
*									Input Class
*
////////////////////////////////////////////////////////////////////////////////*/
class Input {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	Input() = default;
	~Input() = default;

	void Initialize(WinApp* winApp);
	void Update();

	/*--------------------------------------------------------------------*/
	/// 入力関数

	// キーの押下判定
	bool PushKey(BYTE keyNumber);

	// キーのトリガー判定
	bool TriggerKey(BYTE keyNumber);

	// getter

	const std::array<BYTE, inputKeyMaxNum>& GetAllKey() const;

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::array<BYTE, inputKeyMaxNum> key_{};
	std::array<BYTE, inputKeyMaxNum> keyPre_{};

	ComPtr<IDirectInput8> dInput_;
	ComPtr<IDirectInputDevice8> keyboard_;

};