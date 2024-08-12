#pragma once

// Command
#include "ICommand.h"

/*////////////////////////////////////////////////////////////////////////////////
*								InputHandler Class
////////////////////////////////////////////////////////////////////////////////*/
class InputHandler {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	InputHandler() = default;
	~InputHandler() = default;

	ICommand* HandleInput();

	void AssignMoveLeftCommandToPressKeyA();
	void AssignMoveRightCommandToPressKeyD();

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	ICommand* pressKeyD_;
	ICommand* pressKeyA_;

};