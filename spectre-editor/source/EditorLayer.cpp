#include "EditorLayer.h"

#include <filesystem>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui/imgui.h>
#include <imgui/imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/imgui/backends/imgui_impl_glfw.cpp>

#include "components/MeshComponent.h"
#include "components/MeshRendererComponent.h"
#include "components/NameComponent.h"
#include "components/SpriteRendererComponent.h"
#include "components/TransformComponent.h"
#include "Input.h"

namespace Spectre
{
	EditorLayer::EditorLayer()
		: m_Camera(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f)
		, m_IsViewportHovered(false)
		, m_BackgroundColor(glm::vec3(34.0f / 255.0f, 34.0f / 255.0f, 34.0f / 255.0f))
	{
		m_Scene = std::make_shared<Scene>();
		m_HierarchyPanel.setScene(m_Scene);
		m_InspectorPanel.setScene(m_Scene);
		m_InspectorPanel.setHierarchyPanel(&m_HierarchyPanel);
	}

	EditorLayer::~EditorLayer()
	{}

	void EditorLayer::onAttach()
	{
		ResourceManager::getShader("mesh")->use();
		ResourceManager::getShader("mesh")->setUniformMat4("u_View", m_Camera.getView());
		ResourceManager::getShader("mesh")->setUniformMat4("u_Projection", m_Camera.getProjection());
		ResourceManager::getShader("light")->use();
		ResourceManager::getShader("light")->setUniformMat4("u_View", m_Camera.getView());
		ResourceManager::getShader("light")->setUniformMat4("u_Projection", m_Camera.getProjection());

		m_Framebuffer.create(1280, 720);

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
		m_Camera.onEvent(event);
	}

	void EditorLayer::onUpdate(float deltaTime)
	{
		if (m_IsViewportHovered) {
			m_Camera.onUpdate(deltaTime);
		}
	}

	void EditorLayer::onRender()
	{
		m_Framebuffer.bind();

		glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto group = m_Scene->getEntities().group<TransformComponent>(entt::get<MeshComponent, MeshRendererComponent>);
		
		for (auto entity : group) {
			auto [transform, mesh, meshRenderer] = group.get(entity);

			if (meshRenderer.shader->getId() == ResourceManager::getShader("mesh")->getId()) {
				meshRenderer.shader->use();
				meshRenderer.shader->setUniformVec3("u_ObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));
				meshRenderer.shader->setUniformVec3("u_ViewPosition", m_Camera.getPosition());
			}
			else if (meshRenderer.shader->getId() == ResourceManager::getShader("light")->getId()) {
				//ResourceManager::getShader("light")->use();
				//ResourceManager::getShader("light")->setUniformVec3("u_ObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));
				ResourceManager::getShader("mesh")->use();
				ResourceManager::getShader("mesh")->setUniformVec3("u_LightPosition", transform.position);
			}

			m_Renderer.renderMesh(transform, mesh.mesh, meshRenderer.shader);
		}

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

		// Main menu
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Quit")) {
					Application::get().close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);

		// Hierarchy
		m_HierarchyPanel.onImGuiRender();
		
		// Settings
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Background Color", &m_BackgroundColor.r, ImGuiColorEditFlags_NoInputs);
		static bool isWireframe = false;
		ImGui::Checkbox("Wireframe", &isWireframe);
		ImGui::Checkbox("V-Sync", &Application::get().getWindow().m_IsVsync);

		if (isWireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		ImGui::End();

		// Statistics
		ImGui::Begin("Statistics");
		ImGui::Text("Frametime: %.3f ms, FPS: %.1f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		// Inspector
		m_InspectorPanel.onImGuiRender();

		// Assets
		ImGui::Begin("Assets");
		ImGui::End();

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		m_IsViewportHovered = ImGui::IsWindowHovered();
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)m_Framebuffer.getColorAttachment(), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{0, 1}, ImVec2{1, 0});
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

		//m_Renderer.resetStatistics();
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
