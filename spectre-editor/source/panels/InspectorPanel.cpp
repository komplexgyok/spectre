#include "InspectorPanel.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>

#include <entt/include/entt.hpp>
#include <imgui/imgui/imgui.h>

#include "components/MeshComponent.h"
#include "components/NameComponent.h"
#include "components/SpriteRendererComponent.h"
#include "components/TransformComponent.h"

namespace Spectre
{
	void InspectorPanel::onImGuiRender()
	{
		ImGui::Begin("Inspector");
		if (!m_Scene->getEntities().empty()) {
			entt::entity entity = (entt::entity)m_HierarchyPanel->getSelected();

			// Name Component
			showNameComponent();

			// Transform Component
			showTransformComponent();

			// Sprite Renderer Component
			showSpriteRendererComponent();

			// Mesh Component
			showMeshComponent();

			static bool isComponentPresent = false;

			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 100.0f);

			if (ImGui::Button("Add Component", ImVec2(200, 0))) {
				ImGui::OpenPopup("ComponentSelect");
			}

			if (ImGui::BeginPopup("ComponentSelect")) {
				const char* componentTypes[] = { "Transform", "SpriteRenderer", "Mesh" };
				static int selectedComponent = -1;

				ImGui::Text("Component Type");
				ImGui::Separator();

				for (int i = 0; i < 3; i++) {
					if (ImGui::Selectable(componentTypes[i])) {
						selectedComponent = i;
					}
				}

				switch ((ComponentType)selectedComponent) {
					case ComponentType::TransformComponent:
					{
						if (m_Scene->getEntities().all_of<TransformComponent>(entity)) {
							isComponentPresent = true;
						}
						else {
							m_Scene->getEntities().emplace<TransformComponent>(entity, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
						}
					}
					break;

					case ComponentType::SpriteRendererComponent:
					{
						if (m_Scene->getEntities().all_of<SpriteRendererComponent>(entity)) {
							isComponentPresent = true;
						}
						else {
							m_Scene->getEntities().emplace<SpriteRendererComponent>(entity, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
						}
					}
					break;

					case ComponentType::MeshComponent:
					{
						if (m_Scene->getEntities().all_of<MeshComponent>(entity)) {
							isComponentPresent = true;
						}
						else {
							m_Scene->getEntities().emplace<MeshComponent>(entity, "assets/meshes/cube.obj");
						}
					}
					break;
				}
				selectedComponent = -1;

				ImGui::EndPopup();
			}

			if (isComponentPresent) {
				ImGui::OpenPopup("ExistingComponent");
			}

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("ExistingComponent", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::Text("This entity already has this component");
					
				ImGui::Spacing();
					
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 60.0f);

				if (ImGui::Button("OK", ImVec2(120, 0))) {
					ImGui::CloseCurrentPopup();
					isComponentPresent = false;
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void InspectorPanel::setHierarchyPanel(HierarchyPanel* hierarchyPanel)
	{
		m_HierarchyPanel = hierarchyPanel;
	}

	void InspectorPanel::setScene(const std::shared_ptr<Scene>& scene)
	{
		m_Scene = scene;
	}

	void InspectorPanel::showNameComponent()
	{
		entt::entity entity = (entt::entity)m_HierarchyPanel->getSelected();

		if (m_Scene->getEntities().all_of<NameComponent>(entity)) {
			NameComponent& name = m_Scene->getEntities().get<NameComponent>(entity);
			
			char buffer[256];
			memcpy(buffer, name.name.c_str(), name.name.size() + 1);
			
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Name");
			ImGui::SameLine();
			ImGui::InputText("##HiddenName", buffer, 256);
			
			name = std::string(buffer);

			ImGui::Dummy(ImVec2(0.0f, 5.0f));
		}
	}

	void InspectorPanel::showTransformComponent()
	{
		entt::entity entity = (entt::entity)m_HierarchyPanel->getSelected();

		if (m_Scene->getEntities().all_of<TransformComponent>(entity)) {
			if (ImGui::CollapsingHeader("Transform Component")) {
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				TransformComponent& transform = m_Scene->getEntities().get<TransformComponent>(entity);

				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.85f);

				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X");
				ImGui::SameLine();
				ImGui::SliderFloat("##HiddenTransformX", &transform.position.x, -5.0f, 5.0f);

				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y");
				ImGui::SameLine();
				ImGui::SliderFloat("##HiddenTransformY", &transform.position.y, -5.0f, 5.0f);

				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Z");
				ImGui::SameLine();
				ImGui::SliderFloat("##HiddenTransformZ", &transform.position.z, -20.0f, 20.0f);

				ImGui::PopItemWidth();
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));
		}
	}

	void InspectorPanel::showSpriteRendererComponent()
	{
		entt::entity entity = (entt::entity)m_HierarchyPanel->getSelected();

		if (m_Scene->getEntities().all_of<SpriteRendererComponent>(entity)) {
			if (ImGui::CollapsingHeader("Sprite Renderer Component")) {
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				SpriteRendererComponent& spriteRenderer = m_Scene->getEntities().get<SpriteRendererComponent>(entity);

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Color");
				ImGui::SameLine();
				ImGui::ColorEdit4("##HiddenColor", &spriteRenderer.color.r, ImGuiColorEditFlags_NoInputs);
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));
		}
	}

	void InspectorPanel::showMeshComponent()
	{
		entt::entity entity = (entt::entity)m_HierarchyPanel->getSelected();

		if (m_Scene->getEntities().all_of<MeshComponent>(entity)) {
			if (ImGui::CollapsingHeader("Mesh Component")) {
				ImGui::Dummy(ImVec2(0.0f, 5.0f));

				MeshComponent& mesh = m_Scene->getEntities().get<MeshComponent>(entity);

				std::string path = "assets/meshes";
				for (const auto& entry : std::filesystem::directory_iterator(path)) {
					std::stringstream path, filename;
					path << "assets/meshes/" << entry.path().filename().string();
					filename << entry.path().filename().string();

					if (ImGui::Button(filename.str().c_str())) {
						m_Scene->getEntities().replace<MeshComponent>(entity, path.str());
					}
				}
			}

			ImGui::Dummy(ImVec2(0.0f, 5.0f));
		}
	}
}
