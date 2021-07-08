#pragma once

#include <Spectre.h>

#include <random>

#include "EditorCamera.h"
#include "Framebuffer.h"
#include "Scene.h"

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
		virtual void onRender() override;
		virtual void onImGuiRender() override;

		virtual void imGuiBegin() override;
		virtual void imGuiEnd() override;

	private:
		Scene m_Scene;
		Renderer3D m_Renderer;
		//OrthographicCamera m_Camera;
		Framebuffer m_Framebuffer;
		EditorCamera m_Camera;

		// ImGui settings
		glm::vec3 m_BackgroundColor;
	};
}
