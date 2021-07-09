#pragma once

#include "HierarchyPanel.h"
#include "Scene.h"

namespace Spectre
{
	enum class ComponentType
	{
		TransformComponent,
		SpriteRendererComponent,
		MeshComponent,
		MeshRendererComponent
	};

	class InspectorPanel
	{
	public:
		void onImGuiRender();

		void setHierarchyPanel(HierarchyPanel* hierarchyPanel);
		void setScene(const std::shared_ptr<Scene>& scene);

	private:
		void showNameComponent();
		void showTransformComponent();
		void showSpriteRendererComponent();
		void showMeshComponent();
		void showMeshRendererComponent();

		std::shared_ptr<Scene> m_Scene;
		HierarchyPanel* m_HierarchyPanel;
	};
}
