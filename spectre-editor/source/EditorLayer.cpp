#include "EditorLayer.h"

#include <filesystem>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui/imgui.h>
#include <imgui/imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/imgui/backends/imgui_impl_glfw.cpp>

#include "components/MeshComponent.h"
#include "components/NameComponent.h"
#include "components/SpriteRendererComponent.h"
#include "components/TransformComponent.h"

namespace Spectre
{
	EditorLayer::EditorLayer()
		: m_BackgroundColor(glm::vec3(0.0f, 0.0f, 0.0f))
	{}

	EditorLayer::~EditorLayer()
	{}

	void EditorLayer::onAttach()
	{
		ResourceManager::getShader("mesh")->use();
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));
		ResourceManager::getShader("mesh")->setUniformMat4("u_View", view);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		ResourceManager::getShader("mesh")->setUniformMat4("u_Projection", projection);

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

		//Mesh mesh("assets/meshes/cube.obj");
		//mesh.loadOBJ("assets/meshes/cube_t.obj");
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
		//Mesh mesh;

		glEnable(GL_DEPTH_TEST);
		m_Framebuffer.bind();

		glClearColor(m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//m_Renderer.beginScene();

		//m_Renderer.drawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(100.0f, 100.0f), ResourceManager::getTexture("grass-top"));

		//for (int i = 1; i <= m_GridSize; i++) {
		//	for (int j = 1; j <= m_GridSize; j++) {
		//		//m_Renderer.drawQuad(glm::vec2((float)(j * 100), (float)(i * 100)), glm::vec2(90.0f, 90.0f), ResourceManager::getTexture("grass-top"));
		//		m_Renderer.drawQuad(glm::vec2((float)(j * 100), (float)(i * 100)), glm::vec2(90.0f, 90.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		//	}
		//}

		auto group = m_Scene.getEntities().group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group) {
			auto [transform, mesh] = group.get(entity);
			transform.position;
			mesh.mesh;
			m_Renderer.renderMesh(transform.position, mesh.mesh, ResourceManager::getShader("mesh"));
		}

		/*auto view = m_Scene.getEntities().view<TransformComponent>();
		for (auto entity : view) {
			TransformComponent& transform = view.get<TransformComponent>(entity);

			if (m_Scene.getEntities().all_of<SpriteRendererComponent>(entity)) {
				SpriteRendererComponent& spriteRenderer = m_Scene.getEntities().get<SpriteRendererComponent>(entity);
				m_Renderer.drawQuad(glm::vec2(transform.position.x, transform.position.y), glm::vec2(90.0f, 90.0f), spriteRenderer.color);
			}
			else {
				m_Renderer.drawQuad(glm::vec2(transform.position.x, transform.position.y), glm::vec2(90.0f, 90.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
			}
		}*/

		//m_Renderer.endScene();

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

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Quit")) {
					Application::get().close();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		// Scene Hierarchy
		ImGui::Begin("Scene Hierarchy");
		
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;
		static uint32_t nodeSelected = 0;

		if (!m_Scene.getEntities().empty()) {
			m_Scene.getEntities().each([&](entt::entity entity) {
				if (nodeSelected == (uint32_t)entity) {
					treeNodeFlags |= ImGuiTreeNodeFlags_Selected;
				}
				else {
					treeNodeFlags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_SpanAvailWidth;
				}
				NameComponent& name = m_Scene.getEntities().get<NameComponent>(entity);
				if (ImGui::TreeNodeEx((void*)(uint32_t)entity, treeNodeFlags, name.name.c_str())) {
				//if (ImGui::TreeNodeEx((void*)(uint32_t)entity, treeNodeFlags, "Test")) {
					if (ImGui::IsItemClicked()) {
						nodeSelected = (uint32_t)entity;
						std::cout << nodeSelected << std::endl;
					}
					ImGui::TreePop();
				}
			});
		}
		
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
		static bool isWireframe = false;
		ImGui::Checkbox("Wireframe", &isWireframe);

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
		/*ImGui::Text("Draw call count: %d", m_Renderer.getDrawCallCount());
		ImGui::Text("Quad count: %d", m_Renderer.getQuadCount());
		ImGui::Text("Vertex count: %d", m_Renderer.getVertexCount());*/
		ImGui::End();

		// Inspector
		ImGui::Begin("Inspector");
		if (!m_Scene.getEntities().empty()) {
			ImGui::Text("Entity Id: %d", nodeSelected);

			entt::entity entity = (entt::entity)nodeSelected;

			// NameComponent
			if (m_Scene.getEntities().all_of<NameComponent>(entity)) {
				NameComponent& name = m_Scene.getEntities().get<NameComponent>(entity);
				char buffer[256];
				memcpy(buffer, name.name.c_str(), name.name.size()+1);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Name");
				ImGui::SameLine();
				ImGui::InputText("##Hidden", buffer, 256);
				name = std::string(buffer);
			}

			// TransformComponent
			if (m_Scene.getEntities().all_of<TransformComponent>(entity)) {
				ImGui::Separator();
				ImGui::Text("Transform Component");

				TransformComponent& transform = m_Scene.getEntities().get<TransformComponent>(entity);
				
				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "X");
				ImGui::SameLine();
				ImGui::SliderFloat("##HiddenTransformX", &transform.position.x, 0.0f, 10.0f);
				
				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Y");
				ImGui::SameLine();
				ImGui::SliderFloat("##HiddenTransformY", &transform.position.y, 0.0f, 10.0f);
				
				ImGui::AlignTextToFramePadding();
				ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "Z");
				ImGui::SameLine();
				ImGui::SliderFloat("##HiddenTransformZ", &transform.position.z, 0.0f, 100.0f);
			}

			// SpriteRendererComponent
			if (m_Scene.getEntities().all_of<SpriteRendererComponent>(entity)) {
				ImGui::Separator();
				ImGui::Text("Sprite Renderer Component");

				SpriteRendererComponent& spriteRenderer = m_Scene.getEntities().get<SpriteRendererComponent>(entity);

				ImGui::AlignTextToFramePadding();
				ImGui::Text("Color");
				ImGui::SameLine();
				ImGui::ColorEdit4("##HiddenColor", &spriteRenderer.color.r, ImGuiColorEditFlags_NoInputs);
			}

			static bool isButton1Clicked = false;
			if (ImGui::Button("Add Transform Component")) {
				isButton1Clicked = true;
			}

			if (isButton1Clicked) {
				m_Scene.getEntities().emplace<TransformComponent>(entity, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				isButton1Clicked = false;
			}

			static bool isButton2Clicked = false;
			if (ImGui::Button("Add Sprite Renderer Component")) {
				isButton2Clicked = true;
			}

			if (isButton2Clicked) {
				m_Scene.getEntities().emplace<SpriteRendererComponent>(entity, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
				isButton2Clicked = false;
			}

			static bool isButton3Clicked = false;
			if (ImGui::Button("Add Mesh Component")) {
				isButton3Clicked = true;
			}

			if (isButton3Clicked) {
				m_Scene.getEntities().emplace<MeshComponent>(entity, "assets/meshes/cube.obj");
				isButton3Clicked = false;
			}
		}
		
		ImGui::End();

		// Assets
		ImGui::Begin("Assets");
		/*std::string path = "assets";
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			std::cout << entry.path() << std::endl;
		}*/
		ImGui::End();

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
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
