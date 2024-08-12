#pragma once

// GameObject
#include "GameObject2D.h"
#include "GameObject3D.h"

// c++
#include <memory>
#include <list>
#include <map>
#include <variant>
#include <string>
#include <iostream>

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

	void Render();

	// setter

	void Set(GameObject2D* object);
	void Set(GameObject3D* object);

private:
	/*-----------------------------*/
	///			メンバ変数
	/*-----------------------------*/

	using GameObjectVariant = std::variant<GameObject2D*, GameObject3D*>;
	std::map<GameObjectType, std::list<GameObjectVariant>> objectsByType_;
	std::map<GameObjectType, std::map<std::string, int>> nameCounts_;

	GameObjectVariant* selectedObject_ = nullptr;

private:
	/*-----------------------------*/
	///			private関数
	/*-----------------------------*/

	std::string GetTypeName(GameObjectType type);
	std::string GenerateUniqueName(GameObjectType type, const std::string& baseName);
};