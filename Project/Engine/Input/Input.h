#pragma once

// DirectInput
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <XInput.h>

// DXLib
#include "ComPtr.h"

// Lib
#include "Vector2.h"

// c++
#include <array>
#include <cassert>
#include <cstdint>
#include <string>

///===============================================================================
/// クラス前方宣言
class WinApp;
///===============================================================================

// キーの数
static const uint32_t inputKeyMaxNum = 256;

// XINPUTGamePadのボタンの種類
enum class InputGamePadButtons {

	ARROW_UP,       // 十字ボタンの上方向
	ARROW_DOWN,     // 十字ボタンの下方向
	ARROW_LEFT,     // 十字ボタンの左方向
	ARROW_RIGHT,    // 十字ボタンの右方向
	START,          // スタートボタン
	BACK,           // バックボタン
	LEFT_THUMB,     // 左スティックのボタン
	RIGHT_THUMB,    // 右スティックのボタン
	LEFT_SHOULDER,  // 左ショルダーボタン（LB）
	RIGHT_SHOULDER, // 右ショルダーボタン（RB）
	A,              // Aボタン
	B,              // Bボタン
	X,              // Xボタン
	Y,              // Yボタン
	Counts          // ボタンの数を表すための定数
};


/*////////////////////////////////////////////////////////////////////////////////
*									Input Class
////////////////////////////////////////////////////////////////////////////////*/
class Input {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
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

	// ゲームパッドのボタンの押下判定
	bool PushGamepadButton(InputGamePadButtons button);

	// ゲームパッドのボタンのトリガー判定
	bool TriggerGamepadButton(InputGamePadButtons button);

	// Vector2型 左スティックと右スティックの状態を取得
	Vector2 GetLeftStickVal() const;
	Vector2 GetRightStickVal() const;

	// getter

	const std::array<BYTE, inputKeyMaxNum>& GetAllKey() const;

	// setter

	void SetDeadZone(float deadZone);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	std::array<BYTE, inputKeyMaxNum> key_{};
	std::array<BYTE, inputKeyMaxNum> keyPre_{};

	ComPtr<IDirectInput8> dInput_;
	ComPtr<IDirectInputDevice8> keyboard_;

	XINPUT_STATE gamepadState_{};
	XINPUT_STATE gamepadStatePre_{};

	std::array<bool, static_cast<size_t>(InputGamePadButtons::Counts)> gamepadButtons_{};
	std::array<bool, static_cast<size_t>(InputGamePadButtons::Counts)> gamepadButtonsPre_{};

	float leftThumbX_;
	float leftThumbY_;
	float rightThumbX_;
	float rightThumbY_;

	// デッドゾーンの閾値
	float deadZone_ = 2000.0f;

	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	float ApplyDeadZone(float value);
};