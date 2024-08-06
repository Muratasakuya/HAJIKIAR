#pragma once

// Input
#include "Input.h"

/*////////////////////////////////////////////////////////////////////////////////
*
*								InputManager Class
*
////////////////////////////////////////////////////////////////////////////////*/
class InputManager {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// singleton
	static InputManager* GetInstance();

	// getter

	char* GetKeys();
	char* GetPreKeys();

private:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	char keys_[inputKeyMaxNum]{};
	char preKeys[inputKeyMaxNum]{};

	InputManager() = default;
	~InputManager() = default;
	// コピー禁止
	InputManager(const InputManager&) = delete;
	const InputManager& operator=(const InputManager&) = delete;
};