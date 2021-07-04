#include "EditorLayer.h"

#include <iostream>

#include <imgui/imgui/imgui.h>
#include <imgui/imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/imgui/backends/imgui_impl_glfw.cpp>

namespace Spectre
{
	EditorLayer::EditorLayer()
		: m_Generator(m_RandomDevice()), m_BackgroundColor(glm::vec3(0.0f, 0.0f, 0.0f)), m_GridSize(50)
	{}

	void EditorLayer::onAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& application = Application::get();

		ImGui_ImplGlfw_InitForOpenGL(application.getWindow().getNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void EditorLayer::onDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorLayer::onEvent(Event& event)
	{
		std::cout << event.toString();
		m_Camera.onEvent(event);
	}

	void EditorLayer::onRender()
	{
		std::uniform_real_distribution<> distribution(0, 1);

		glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Renderer.beginScene();

		for (int i = 1; i <= m_GridSize; i++) {
			for (int j = 1; j <= m_GridSize; j++) {
				float red = static_cast<float>(distribution(m_Generator));
				float green = static_cast<float>(distribution(m_Generator));
				float blue = static_cast<float>(distribution(m_Generator));
				m_Renderer.drawQuad(glm::vec2((float)(j * 100), (float)(i * 100)), glm::vec2(100.0f, 100.0f), glm::vec4(red, green, blue, 1.0f));
			}
		}

		m_Renderer.endScene();
	}

	void EditorLayer::onImGuiRender()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Quit")) {
					Application::get().close();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Background Color", &m_BackgroundColor.r);
		ImGui::DragInt("Grid size", &m_GridSize, 1, 200);
		ImGui::End();

		ImGui::Begin("Statistics");
		ImGui::Text("Frametime: %.3f ms, FPS: %.1f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Draw call count: %d", m_Renderer.getDrawCallCount());
		ImGui::Text("Quad count: %d", m_Renderer.getQuadCount());
		ImGui::Text("Vertex count: %d", m_Renderer.getVertexCount());
		ImGui::End();

		m_Renderer.resetStatistics();
	}
	
	void EditorLayer::imGuiBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	
	void EditorLayer::imGuiEnd()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(1280.0f, 720.0f);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
