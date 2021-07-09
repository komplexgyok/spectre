#pragma once

#include <Spectre.h>

#include <random>

#include "EditorCamera.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "panels/HierarchyPanel.h"
#include "panels/InspectorPanel.h"

namespace Spectre
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onEvent(Event& event) override;
		virtual void onUpdate(float deltaTime) override;
		virtual void onRender() override;
		virtual void onImGuiRender() override;

		virtual void imGuiBegin() override;
		virtual void imGuiEnd() override;

	private:
		std::shared_ptr<Scene> m_Scene;
		Renderer3D m_Renderer;
		Framebuffer m_Framebuffer;
		EditorCamera m_Camera;
		
		HierarchyPanel m_HierarchyPanel;
		InspectorPanel m_InspectorPanel;

		bool m_IsViewportHovered;

		// ImGui settings
		glm::vec3 m_BackgroundColor;
	};
}
