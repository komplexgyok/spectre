#include "EditorLayer.h"

#include <filesystem>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui/imgui.h>
#include <imgui/imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/imgui/backends/imgui_impl_glfw.cpp>
#include <stb_image/stb_image.h>

#include "components/CameraComponent.h"
#include "components/MeshComponent.h"
#include "components/MeshRendererComponent.h"
#include "components/NameComponent.h"
#include "components/SpriteRendererComponent.h"
#include "components/TransformComponent.h"
#include "Input.h"

namespace Spectre
{
	EditorLayer::EditorLayer()
		: m_Camera(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
		, m_IsViewportHovered(false)
		, m_BackgroundColor(glm::vec3(0.133f, 0.133f, 0.133f)) // 34, 34, 34
	{}

	EditorLayer::~EditorLayer()
	{}

	void EditorLayer::onAttach()
	{
		m_Scene = std::make_shared<Scene>();
		m_Scene->createCamera();
		
		m_HierarchyPanel.setScene(m_Scene);
		m_InspectorPanel.setScene(m_Scene);
		m_InspectorPanel.setHierarchyPanel(&m_HierarchyPanel);

		m_Scene->loadScene("imports/scene-01.fbx");

		ResourceManager::getShader("model")->use();
		ResourceManager::getShader("model")->setUniformMat4("u_View", m_Camera.getView());
		ResourceManager::getShader("model")->setUniformMat4("u_Projection", m_Camera.getProjection());
		ResourceManager::getShader("light")->use();
		ResourceManager::getShader("light")->setUniformMat4("u_View", m_Camera.getView());
		ResourceManager::getShader("light")->setUniformMat4("u_Projection", m_Camera.getProjection());
		ResourceManager::getShader("solid")->use();
		ResourceManager::getShader("solid")->setUniformMat4("u_View", m_Camera.getView());
		ResourceManager::getShader("solid")->setUniformMat4("u_Projection", m_Camera.getProjection());

		m_Framebuffer.create(1280, 720);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.Fonts->AddFontFromFileTTF("assets/fonts/SourceCodePro-Regular.ttf", 16);

		ImGui::StyleColorsDark();

		Application& application = Application::get();

		ImGui_ImplGlfw_InitForOpenGL(application.getWindow().getNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450");

		stbi_set_flip_vertically_on_load(true);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		auto group = m_Scene->getEntities().group<>(entt::get<TransformComponent, MeshComponent, MeshRendererComponent>);

		for (auto entity : group) {
			auto [transform, mesh, meshRenderer] = group.get(entity);

			m_Renderer.renderMesh(transform, mesh, meshRenderer, m_Camera, false);
		}

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		for (auto entity : group) {
			auto [transform, mesh, meshRenderer] = group.get(entity);

			if (entity == (entt::entity)m_HierarchyPanel.getSelected()) {
				m_Renderer.renderMesh(transform, mesh, meshRenderer, m_Camera, true);
			}
		}

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		m_Framebuffer.unbind();

		//auto group = m_Scene->getEntities().group<TransformComponent>(entt::get<MeshComponent, MeshRendererComponent>);
		//
		//for (auto entity : group) {
		//	auto [transform, mesh, meshRenderer] = group.get(entity);

		//	if (meshRenderer.shader->getId() == ResourceManager::getShader("mesh")->getId()) {
		//		meshRenderer.shader->use();
		//		
		//		//meshRenderer.shader->setUniformVec3("u_Material.ambient", meshRenderer.materialAmbient);
		//		//meshRenderer.shader->setUniformVec3("u_Material.diffuse", meshRenderer.materialDiffuse);
		//		
		//		glActiveTexture(GL_TEXTURE0);
		//		glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture("container")->getId());

		//		glActiveTexture(GL_TEXTURE1);
		//		glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture("container-specular")->getId());

		//		glActiveTexture(GL_TEXTURE2);
		//		glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture("matrix")->getId());

		//		meshRenderer.shader->setUniformVec3("u_Material.specular", meshRenderer.materialSpecular);
		//		meshRenderer.shader->setUniformFloat("u_Material.shininess", meshRenderer.materialShininess);

		//		meshRenderer.shader->setUniformVec3("u_ViewPosition", m_Camera.getPosition());
		//		meshRenderer.shader->setUniformFloat("u_Time", static_cast<float>(glfwGetTime()));
		//	}
		//	else if (meshRenderer.shader->getId() == ResourceManager::getShader("light")->getId()) {
		//		ResourceManager::getShader("mesh")->use();
		//		ResourceManager::getShader("mesh")->setUniformVec3("u_Light.position", transform.position);
		//	}

		//	ResourceManager::getShader("model")->use();
		//	ResourceManager::getShader("model")->setUniformMat4("u_View", m_Camera.getView());
		//	ResourceManager::getShader("light")->use();
		//	ResourceManager::getShader("light")->setUniformMat4("u_View", m_Camera.getView());

		//	//m_Renderer.renderMesh(transform, mesh.mesh, meshRenderer.shader);
		//	glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//	glStencilMask(0xFF);
		//	m_Renderer.renderModel(transform, mesh.model, meshRenderer.shader);

		//	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//	glStencilMask(0x00);
		//	glDisable(GL_DEPTH_TEST);
		//	ResourceManager::getShader("light")->use();
		//	transform.scale = glm::vec3(1.5f, 1.5f, 1.5f);
		//	m_Renderer.renderModel(transform, mesh.model, ResourceManager::getShader("light"));
		//	glStencilMask(0xFF);
		//	glStencilFunc(GL_ALWAYS, 0, 0xFF);
		//	glEnable(GL_DEPTH_TEST);
		//}

		//m_Framebuffer.unbind();

		//// Iterate through scene cameras
		//auto cameraGroup = m_Scene->getEntities().group<>(entt::get<TransformComponent, CameraComponent>);

		//for (auto cameraEntity : cameraGroup) {
		//	auto [cameraTransform, camera] = cameraGroup.get(cameraEntity);

		//	camera.camera.getFramebuffer().bind();

		//	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	auto group = m_Scene->getEntities().group<>(entt::get<TransformComponent, MeshComponent, MeshRendererComponent>);

		//	for (auto entity : group) {
		//		auto [transform, mesh, meshRenderer] = group.get<TransformComponent, MeshComponent, MeshRendererComponent>(entity);

		//		if (meshRenderer.shader->getId() == ResourceManager::getShader("mesh")->getId()) {
		//			meshRenderer.shader->use();

		//			glActiveTexture(GL_TEXTURE0);
		//			glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture("container")->getId());

		//			glActiveTexture(GL_TEXTURE1);
		//			glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture("container-specular")->getId());

		//			glActiveTexture(GL_TEXTURE2);
		//			glBindTexture(GL_TEXTURE_2D, ResourceManager::getTexture("matrix")->getId());

		//			meshRenderer.shader->setUniformVec3("u_Material.specular", meshRenderer.materialSpecular);
		//			meshRenderer.shader->setUniformFloat("u_Material.shininess", meshRenderer.materialShininess);

		//			meshRenderer.shader->setUniformVec3("u_ViewPosition", m_Camera.getPosition());
		//			meshRenderer.shader->setUniformFloat("u_Time", static_cast<float>(glfwGetTime()));
		//		}
		//		else if (meshRenderer.shader->getId() == ResourceManager::getShader("light")->getId()) {
		//			ResourceManager::getShader("mesh")->use();
		//			ResourceManager::getShader("mesh")->setUniformVec3("u_Light.position", transform.position);
		//		}

		//		/*glm::mat4 view = glm::lookAt(transform.position, transform.position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//		ResourceManager::getShader("mesh")->use();
		//		ResourceManager::getShader("mesh")->setUniformMat4("u_View", view);
		//		ResourceManager::getShader("mesh")->setUniformMat4("u_Projection", camera.camera.getProjection());
		//		ResourceManager::getShader("light")->use();
		//		ResourceManager::getShader("light")->setUniformMat4("u_View", view);
		//		ResourceManager::getShader("light")->setUniformMat4("u_Projection", camera.camera.getProjection());*/

		//		ResourceManager::getShader("model")->use();
		//		ResourceManager::getShader("model")->setUniformMat4("u_View", camera.camera.getView());
		//		ResourceManager::getShader("light")->use();
		//		ResourceManager::getShader("light")->setUniformMat4("u_View", camera.camera.getView());

		//		//m_Renderer.renderMesh(transform, mesh.mesh, meshRenderer.shader);
		//		m_Renderer.renderModel(transform, mesh.model, meshRenderer.shader);
		//	}

		//	camera.camera.getFramebuffer().unbind();
		//}
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

		ImGuiStyle& style = ImGui::GetStyle();
		style.TabRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.WindowMenuButtonPosition = ImGuiDir_None;

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

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

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
		ImGui::Text("Scene node counter: %d", m_Scene->getStatistics().nodeCounter);
		ImGui::Text("Scene mesh counter: %d", m_Scene->getStatistics().meshCounter);
		ImGui::Text("Scene vertex counter: %d", m_Scene->getStatistics().vertexCounter);
		ImGui::End();

		// Inspector
		m_InspectorPanel.onImGuiRender();

		// Assets
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
		ImGui::Begin("Assets");

		//{
		//	ImGui::BeginChild("Assets Left", ImVec2(150.0f, 0.0f), true);

		//	uint32_t index = 100;
		//	std::string path = "assets";
		//	for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
		//		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_None;

		//		//std::cout << entry << std::endl;

		//		if (entry.is_directory()) {
		//			treeNodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
		//		}
		//		else {
		//			treeNodeFlags |= ImGuiTreeNodeFlags_Leaf;
		//		}

		//		std::stringstream label;
		//		label << entry.path().filename().string();

		//		if (ImGui::TreeNodeEx((void*)index, treeNodeFlags, label.str().c_str())) {
		//			if (ImGui::IsItemClicked()) {
		//				std::cout << "Clicked: " << index << std::endl;
		//			}
		//			ImGui::TreePop();
		//		}

		//		index++;

		//		/*std::stringstream path, filename;
		//		path << "assets/meshes/" << entry.path().filename().string();
		//		filename << entry.path().filename().string();

		//		if (ImGui::Button(filename.str().c_str())) {
		//			m_Scene->getEntities().replace<MeshComponent>(entity, path.str());
		//		}*/
		//		ImGui::Image((ImTextureID)ResourceManager::getTexture("folder")->getId(), ImVec2(84.0f, 74.0f), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		//	}
		//	ImGui::EndChild();
		//}

		//ImGui::SameLine();

		//{
		//	ImGui::BeginChild("Assets Right", ImVec2(0.0f, 0.0f));
		//	ImGui::Text("Right");

		//	bool isPopup = false;

		//	if (ImGui::BeginPopupContextWindow()) {
		//		if (ImGui::Selectable("Import asset")) {
		//			isPopup = true;
		//		}
		//		ImGui::EndPopup();
		//	}

		//	if (isPopup) {
		//		ImGui::OpenPopup("Import asset");
		//	}

		//	if (ImGui::BeginPopupModal("Import asset")) {
		//		ImGui::Text("File browser");
		//		ImGui::EndPopup();
		//	}
		//	

		//	ImGui::EndChild();
		//}
		
		ImGui::End();
		ImGui::PopStyleVar();

		/*ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
ImGui::Begin("Test 3226");
ImGui::PopStyleVar();
ImGui::BeginChild("blah");
ImGui::EndChild();
ImGui::End();*/

		// Scene
		if (!m_Scene->getEntities().empty()) {
			entt::entity selectedEntity = (entt::entity)m_HierarchyPanel.getSelected();

			if (m_Scene->getEntities().all_of<CameraComponent>(selectedEntity)) {
				CameraComponent& camera = m_Scene->getEntities().get<CameraComponent>(selectedEntity);

				ImGui::Begin("Camera");
				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				ImGui::Image((ImTextureID)camera.camera.getFramebuffer().getColorAttachment(), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				ImGui::End();
			}
			else {
				auto cameraGroup = m_Scene->getEntities().group<>(entt::get<TransformComponent, CameraComponent>);

				for (auto cameraEntity : cameraGroup) {
					auto [transform, camera] = cameraGroup.get(cameraEntity);

					if (camera.camera.isPrimary()) {
						ImGui::Begin("Main Camera");
						ImVec2 viewportSize = ImGui::GetContentRegionAvail();
						ImGui::Image((ImTextureID)camera.camera.getFramebuffer().getColorAttachment(), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
						ImGui::End();
					}
				}
			}

			auto cameraGroup = m_Scene->getEntities().group<>(entt::get<TransformComponent, CameraComponent>);

			for (auto cameraEntity : cameraGroup) {
				auto [transform, camera] = cameraGroup.get(cameraEntity);

				if (camera.camera.isPrimary()) {
					ImGui::Begin("Game");
					ImVec2 viewportSize = ImGui::GetContentRegionAvail();
					ImGui::Image((ImTextureID)camera.camera.getFramebuffer().getColorAttachment(), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
					ImGui::End();
				}
			}
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene");
		m_IsViewportHovered = ImGui::IsWindowHovered();
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		if (viewportSize.x != m_Framebuffer.getWidth() || viewportSize.y != m_Framebuffer.getHeight()) {
			m_Framebuffer.recreate(viewportSize.x, viewportSize.y);
		}

		m_Camera.setAspectRatio(viewportSize.x / viewportSize.y);

		ImGui::Image((ImTextureID)m_Framebuffer.getColorAttachment(), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
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
