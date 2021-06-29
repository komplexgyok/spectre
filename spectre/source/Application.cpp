#include "Application.h"

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Renderer2D.h"
#include "ResourceManager.h"

namespace Spectre
{
	Application::Application()
	{
		m_Window = std::make_unique<Window>(1280, 720, "Spectre Engine");

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

		ImGui_ImplGlfw_InitForOpenGL(m_Window->getNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Load shaders
		ResourceManager::addShader("basic", "../spectre/resources/shaders/basic.vert", "../spectre/resources/shaders/basic.frag");
		ResourceManager::addShader("3d", "../spectre/resources/shaders/3d.vert", "../spectre/resources/shaders/3d.frag");

		// Load textures
		ResourceManager::addTexture("grass-top", "../spectre/resources/textures/grass-top.png");
		ResourceManager::addTexture("grass-side", "../spectre/resources/textures/grass-side.png");
		ResourceManager::addTexture("grass-bottom", "../spectre/resources/textures/grass-bottom.png");

		float vertices[] = {
			// Front face
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 2.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 2.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 2.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 2.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 2.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 2.0f,

			// Back face
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 2.0f,
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 2.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 2.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 2.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 2.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 2.0f,

			// Left face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 2.0f,
			-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 2.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 2.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 2.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 2.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 2.0f,

			// Right face
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 2.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 2.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 2.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 2.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 2.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 2.0f,

			// Top face
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glCreateVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glCreateBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

		//glCreateBuffers(1, &indexBuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		ResourceManager::getShader("3d")->use();
		ResourceManager::getShader("3d")->setUniformInt("uTexture1", 0);
		ResourceManager::getShader("3d")->setUniformInt("uTexture2", 1);
		ResourceManager::getShader("3d")->setUniformInt("uTexture3", 2);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		ResourceManager::getShader("3d")->setUniformMat4("uProjection", projection);

		backgroundColor = glm::vec4(0.2f, 0.3f, 0.8f, 1.0f);
	}

	Application::~Application()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::run()
	{
		while (!glfwWindowShouldClose(m_Window->getNativeWindow())) {
			// Handle input
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Test Panel");
			ImGui::Text("Hello from Spectre Engine");
			ImGui::ColorEdit4("Background Color", &backgroundColor.r);

			ImGui::End();

			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(m_Window->getNativeWindow(), &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);

			// Render
			glm::vec3 cubePositions[] = {
				glm::vec3(0.0f,  0.0f,  0.0f),
				glm::vec3(2.0f,  5.0f, -15.0f),
				glm::vec3(-1.5f, -2.2f, -2.5f),
				glm::vec3(-3.8f, -2.0f, -12.3f),
				glm::vec3(2.4f, -0.4f, -3.5f),
				glm::vec3(-1.7f,  3.0f, -7.5f),
				glm::vec3(1.3f, -2.0f, -2.5f),
				glm::vec3(1.5f,  2.0f, -2.5f),
				glm::vec3(1.5f,  0.2f, -1.5f),
				glm::vec3(-1.3f,  1.0f, -1.5f)
			};

			ResourceManager::getShader("3d")->use();
			glBindTextureUnit(0, ResourceManager::getTexture("grass-top")->getId());
			glBindTextureUnit(1, ResourceManager::getTexture("grass-side")->getId());
			glBindTextureUnit(2, ResourceManager::getTexture("grass-bottom")->getId());
			glBindVertexArray(vertexArray);

			const float radius = 10.0f;
			float cameraX = sin(glfwGetTime()) * radius;
			float cameraZ = cos(glfwGetTime()) * radius;

			glm::mat4 view = glm::mat4(1.0f);
			view = glm::lookAt(glm::vec3(cameraX, 0.0f, cameraZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			ResourceManager::getShader("3d")->setUniformMat4("uView", view);

			for (int i = 0; i < 10; i++) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f * (i+1)), glm::vec3(0.5f, 0.3f, 0.5f));
				
				ResourceManager::getShader("3d")->setUniformMat4("uModel", model);
				
				//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			
			// Swap buffers
			glfwSwapBuffers(m_Window->getNativeWindow());
		}
	}
}
