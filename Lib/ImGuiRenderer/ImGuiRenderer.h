#pragma once

// GameObject
#include "GameObject3D.h"

// c++
#include <memory>
#include <list>
#include <string>

/*////////////////////////////////////////////////////////////////////////////////
*								ImGuiRenderer Class
////////////////////////////////////////////////////////////////////////////////*/
class ImGuiRenderer {
public:
	/*-----------------------------*/
	///			メンバ関数
	/*-----------------------------*/

	// default
	ImGuiRenderer() = default;
	~ImGuiRenderer() = default;

	void Render(std::list<std::unique_ptr<GameObject3D>>& objects);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	int currentObjectIndex_ = 0;

	std::string GetGameObjectName(GameObjectType type, int index);
};