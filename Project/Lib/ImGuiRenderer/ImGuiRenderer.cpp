#include "ImGuiRenderer.h"

#include "ImGuiManager.h"

/*////////////////////////////////////////////////////////////////////////////////
*								ImGui表示
////////////////////////////////////////////////////////////////////////////////*/
void ImGuiRenderer::Render(std::list<std::unique_ptr<GameObject3D>>& objects) {

	std::vector<std::string> names;
	int index = 0;

	for (const auto& object : objects) {

		names.push_back(GetGameObjectName(object->GetType(), index));
		index++;
	}

	ImGui::Begin("Engine");

	const char* comboPreviewValue = "Select";
	if (ImGui::BeginCombo("GameObject", comboPreviewValue)) {
		index = 0;

		for (const auto& name : names) {
			bool isSelected = (currentObjectIndex_ == index);
			if (ImGui::Selectable(name.c_str(), isSelected)) {

				currentObjectIndex_ = index;
			}
			if (isSelected) {

				ImGui::SetItemDefaultFocus();
			}
			index++;
		}
		ImGui::EndCombo();
	}

	auto it = objects.begin();
	std::advance(it, currentObjectIndex_);
	if (it != objects.end()) {

		ImGui::Text("%s", names[currentObjectIndex_].c_str());
		(*it)->ImGui();
	}

	ImGui::End();
}

/*////////////////////////////////////////////////////////////////////////////////
*									名前変換
////////////////////////////////////////////////////////////////////////////////*/
std::string ImGuiRenderer::GetGameObjectName(GameObjectType type, int index) {

	switch (type) {
	case GameObjectType::Object2D:

		return "Object2D " + std::to_string(index);
	case GameObjectType::PrimitiveTriangle:

		return "PrimitiveTriangle " + std::to_string(index);
	case GameObjectType::Triangle:

		return "Triangle " + std::to_string(index);
	case GameObjectType::Sphere:

		return "Sphere " + std::to_string(index);
	case GameObjectType::Model:

		return "Model " + std::to_string(index);
	default:

		return "Unknown " + std::to_string(index);
	}
}
