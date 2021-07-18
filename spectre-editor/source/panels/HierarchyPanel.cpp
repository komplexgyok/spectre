#include "HierarchyPanel.h"

#include <iostream>

#include <imgui/imgui/imgui.h>

#include "components/NameComponent.h"

namespace Spectre
{
	HierarchyPanel::HierarchyPanel()
		: m_Selected(0)
	{}

	void HierarchyPanel::onImGuiRender()
	{
		ImGui::Begin("Hierarchy");

		if (!m_Scene->getEntities().empty()) {
			// Sort entities by name
			m_Scene->getEntities().sort<NameComponent>([](const auto& a, const auto& b) {
				return a.name < b.name;
			});

			// List all entities
			m_Scene->getEntities().view<NameComponent>().each([this](auto entity, auto name) {
				ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;

				if (m_Selected == (uint32_t)entity) {
					treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
				}

				if (ImGui::TreeNodeEx((void*)(uint32_t)entity, treeNodeFlags, name.name.c_str())) {
					if (ImGui::IsItemClicked()) {
						m_Selected = (uint32_t)entity;
					}

					ImGui::TreePop();
				}
			});
		}

		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Selectable("Create Entity")) {
				m_Selected = m_Scene->createEntity();
			}
			
			if (ImGui::Selectable("Create 3D Object")) {
				m_Selected = m_Scene->create3DObject();
			}

			if (ImGui::Selectable("Create Light")) {
				m_Selected = m_Scene->createLight();
			}

			if (ImGui::Selectable("Create Camera")) {
				m_Selected = m_Scene->createCamera();
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void HierarchyPanel::setScene(const std::shared_ptr<Scene>& scene)
	{
		m_Scene = scene;
	}
}
