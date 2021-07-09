#include "HierarchyPanel.h"

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

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (!m_Scene->getEntities().empty()) {
			m_Scene->getEntities().each([&](entt::entity entity) {
				if (m_Selected == (uint32_t)entity) {
					treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
				}
				else {
					treeNodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;
				}

				NameComponent& name = m_Scene->getEntities().get<NameComponent>(entity);
				
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
				m_Scene->createEntity();
			}
			
			if (ImGui::Selectable("Create 3D Object")) {
				m_Scene->create3DObject();
			}

			if (ImGui::Selectable("Create Light")) {
				m_Scene->createLight();
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
