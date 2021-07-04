#pragma once

#include <Spectre.h>

#include <random>

namespace Spectre
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onEvent(Event& event) override;
		virtual void onRender() override;
		virtual void onImGuiRender() override;

		virtual void imGuiBegin() override;
		virtual void imGuiEnd() override;

	private:
		Renderer2D m_Renderer;
		OrthographicCamera m_Camera;

		std::random_device m_RandomDevice;
		std::mt19937 m_Generator;

		glm::vec3 m_BackgroundColor;
		int m_GridSize;
	};
}
