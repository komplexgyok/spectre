#include "EditorLayer.h"

#include <filesystem>
#include <iostream>

#include <imgui/imgui/imgui.h>
#include <imgui/imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/imgui/backends/imgui_impl_glfw.cpp>

#include "components/NameComponent.h"

namespace Spectre
{
	EditorLayer::EditorLayer()
		: m_BackgroundColor(glm::vec3(0.0f, 0.0f, 0.0f)), m_GridSize(50)
	{}

	EditorLayer::~EditorLayer()
	{}

	void EditorLayer::onAttach()
	{
		m_Framebuffer.create(1280, 720);

		m_Scene.addEntity();
		m_Scene.addEntity();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

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
		//std::cout << event.toString();
		m_Camera.onEvent(event);
	}

	void EditorLayer::onRender()
	{
		glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Framebuffer.bind();

		glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Renderer.beginScene();

		m_Renderer.drawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f), ResourceManager::getTexture("grass-top"));
		//m_Renderer.drawQuad(glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f), "grass-top");
		//m_Renderer.drawQuad(glm::vec2(200.0f, 200.0f), glm::vec2(100.0f, 100.0f), "grass-top");
		//m_Renderer.drawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		//m_Renderer.drawQuad(glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//m_Renderer.drawQuad(glm::vec2(200.0f, 200.0f), glm::vec2(100.0f, 100.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

		//for (int i = 1; i <= m_GridSize; i++) {
		//	for (int j = 1; j <= m_GridSize; j++) {
		//		float red = 1.0f;
		//		float green = 1.0f;
		//		float blue = 0.0f;
		//		m_Renderer.drawQuad(glm::vec2((float)(j * 100), (float)(i * 100)), glm::vec2(90.0f, 90.0f), glm::vec4(red, green, blue, 1.0f));
		//		//m_Renderer.drawQuad(glm::vec2((float)(j * 200), (float)(i * 200) - 1000.0f), glm::vec2(100.0f, 100.0f), "grass-top");
		//	}
		//}

		m_Renderer.endScene();

		/*std::string path = "assets";
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			std::cout << entry.path() << std::endl;
		}*/

		m_Framebuffer.unbind();
	}

	void EditorLayer::onImGuiRender()
	{
		// Set up docking
		static bool isDockingEnabled = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
			window_flags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding) {
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		}
		
		ImGui::Begin("DockSpace", &isDockingEnabled, window_flags);
		
		if (!opt_padding) {
			ImGui::PopStyleVar();
		}

		if (opt_fullscreen) {
			ImGui::PopStyleVar(2);
		}

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

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

		// Scene Hierarchy
		ImGui::Begin("Scene Hierarchy");
		
		m_Scene.getEntities().each([&](auto entity) {
			NameComponent& name = m_Scene.getEntities().get<NameComponent>(entity);
			if (ImGui::TreeNode(name.name.c_str())) {
				ImGui::TreePop();
			}
		});
		
		if (ImGui::BeginPopupContextWindow()) {
			if (ImGui::Selectable("Create Entity")) {
				m_Scene.addEntity();
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);

		// Settings
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Background Color", &m_BackgroundColor.r, ImGuiColorEditFlags_NoInputs);
		//ImGui::DragInt("Grid size", &m_GridSize, 1, 200);
		ImGui::End();

		// Statistics
		ImGui::Begin("Statistics");
		ImGui::Text("Frametime: %.3f ms, FPS: %.1f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Draw call count: %d", m_Renderer.getDrawCallCount());
		ImGui::Text("Quad count: %d", m_Renderer.getQuadCount());
		ImGui::Text("Vertex count: %d", m_Renderer.getVertexCount());
		ImGui::End();

		// Inspector
		ImGui::Begin("Inspector");
		ImGui::End();

		// Assets
		ImGui::Begin("Assets");
		ImGui::End();

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)m_Framebuffer.getColorAttachment(), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End();
		ImGui::PopStyleVar();

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
