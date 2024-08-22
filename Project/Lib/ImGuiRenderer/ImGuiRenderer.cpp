#include "ImGuiRenderer.h"

#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								singleton
////////////////////////////////////////////////////////////////////////////////*/
ImGuiRenderer* ImGuiRenderer::GetInstance() {

	static ImGuiRenderer instance;

	return &instance;
}

/*////////////////////////////////////////////////////////////////////////////////
*									ImGui描画
////////////////////////////////////////////////////////////////////////////////*/
void ImGuiRenderer::Render() {

	ImGui::Begin("NewMoon");

	// ノードの開閉状態を管理するためのマップ
	std::map<GameObjectType, bool> treeNodeOpenStates;

	for (auto& [type, objects] : objectsByType_) {
		
		bool isOpen = ImGui::TreeNodeEx(GetTypeName(type).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
		treeNodeOpenStates[type] = isOpen;

		if (isOpen) {
			for (auto& objectVariant : objects) {
				std::visit([&](auto* object) {
					bool isSelected = selectedObject_ && std::visit([&](auto* selected) {
						using T = std::decay_t<decltype(selected)>;
						if constexpr (std::is_same_v<T, std::decay_t<decltype(object)>>) {
							return selected == object;
						}
						return false;
						}, *selectedObject_);

					if (ImGui::Selectable(object->GetObjectName().c_str(), isSelected)) {
						selectedObject_ = &objectVariant;
					}
					}, objectVariant);
			}
			ImGui::TreePop();
		}
	}

	// 選択されたオブジェクトの詳細表示
	if (selectedObject_) {
		std::visit([this, &treeNodeOpenStates](auto* object) {
			auto type = object->GetType();
			if (treeNodeOpenStates[type]) {
				
				object->ImGui();
			}
			}, *selectedObject_);
	}

	ImGui::End();
}

/*////////////////////////////////////////////////////////////////////////////////
*									名前変換
////////////////////////////////////////////////////////////////////////////////*/
std::string ImGuiRenderer::GetTypeName(GameObjectType type) {
	switch (type) {
	case GameObjectType::Object2D:
		return "Object2D";
	case GameObjectType::PrimitiveTriangle:
		return "PrimitiveTriangle";
	case GameObjectType::Triangle:
		return "Triangle";
	case GameObjectType::Sphere:
		return "Sphere";
	case GameObjectType::Model:
		return "Model";
	default:
		return "Unknown";
	}
}

/*////////////////////////////////////////////////////////////////////////////////
*								名前の一意化
////////////////////////////////////////////////////////////////////////////////*/
std::string ImGuiRenderer::GenerateUniqueName(GameObjectType type, const std::string& baseName) {

	auto& nameCount = nameCounts_[type];
	std::string uniqueName = baseName;

	// 名前が既に存在する場合、番号を付ける
	if (nameCount[baseName] > 0) {
		uniqueName += std::to_string(nameCount[baseName] + 1);
	}

	nameCount[baseName]++;

	return uniqueName;
}

/*////////////////////////////////////////////////////////////////////////////////
*									setter
////////////////////////////////////////////////////////////////////////////////*/
void ImGuiRenderer::Set(GameObject2D* object) {

	std::string uniqueName = GenerateUniqueName(object->GetType(), object->GetObjectName());
	object->SetObjectName(uniqueName);

	objectsByType_[object->GetType()].emplace_back(object);
}
void ImGuiRenderer::Set(GameObject3D* object) {

	std::string uniqueName = GenerateUniqueName(object->GetType(), object->GetObjectName());
	object->SetObjectName(uniqueName);

	objectsByType_[object->GetType()].emplace_back(object);
}