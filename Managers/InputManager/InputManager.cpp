#include "InputManager.h"



// keys_ getter
char* InputManager::GetKeys() {

	return keys_;
}

// preKeys_ getter
char* InputManager::GetPreKeys() {

	return preKeys;
}



/*////////////////////////////////////////////////////////////////////////////////

*								singleton

////////////////////////////////////////////////////////////////////////////////*/
InputManager* InputManager::GetInstance() {

	static InputManager instance;

	return &instance;
}