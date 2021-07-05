#pragma once

#include <Spectre.h>

#include <random>

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
		Renderer2D m_Renderer;
		OrthographicCamera m_Camera;
		Framebuffer m_Framebuffer;

		glm::vec3 m_BackgroundColor;
		int m_GridSize;
	};
}
