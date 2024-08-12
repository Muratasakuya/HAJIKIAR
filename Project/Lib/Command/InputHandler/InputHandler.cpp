#include "InputHandler.h"

#include "NewMoon.h"

/*////////////////////////////////////////////////////////////////////////////////
*								キーコマンドの結びつけ
////////////////////////////////////////////////////////////////////////////////*/
void InputHandler::AssignMoveLeftCommandToPressKeyA() {

	ICommand* command = new MoveLeftCommand();
	this->pressKeyA_ = command;
}
void InputHandler::AssignMoveRightCommandToPressKeyD() {

	ICommand* command = new MoveRightCommand();
	this->pressKeyD_ = command;
}
ICommand* InputHandler::HandleInput() {

	if (NewMoon::PushKey(DIK_D)) {

		return pressKeyD_;
	}
	if (NewMoon::PushKey(DIK_A)) {

		return pressKeyA_;
	}

	return nullptr;
}