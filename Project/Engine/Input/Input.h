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

	void ImGui();

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

	// マウスの入力判定
	bool PushMouseLeft() const;
	bool PushMouseRight() const;

	// getter

	const std::array<BYTE, inputKeyMaxNum>& GetAllKey() const;
	// Vector2型 左スティックと右スティックの状態を取得
	Vector2 GetLeftStickVal() const;
	Vector2 GetRightStickVal() const;
	Vector2 GetMousePos() const;
	Vector2 GetMouseMoveValue() const;

	// setter

	void SetDeadZone(float deadZone);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	WinApp* winApp_ = nullptr;

	///-------------------------------------------------------------------------------
	// Keyboard

	std::array<BYTE, inputKeyMaxNum> key_{};
	std::array<BYTE, inputKeyMaxNum> keyPre_{};

	ComPtr<IDirectInput8> dInput_;
	ComPtr<IDirectInputDevice8> keyboard_;

	///-------------------------------------------------------------------------------
	// Gamepad

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

	///-------------------------------------------------------------------------------
	// Mouse

	// マウスの状態を保存
	DIMOUSESTATE mouseState_;

	ComPtr<IDirectInputDevice8> mouse_;  // マウスデバイス

	std::array<bool, 2> mouseButtons_;   // マウスボタンの状態
	Vector2 mousePos_;                   // マウスの移動量

	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	float ApplyDeadZone(float value);
};