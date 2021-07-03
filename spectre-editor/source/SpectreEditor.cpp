#include <Spectre.h>

#include <iostream>
#include <memory>
#include <random>

#include <glad/include/glad/glad.h>
#include <glfw/glfw/include/GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <imgui/imgui/imgui.h>
#include <imgui/imgui/backends/imgui_impl_opengl3.cpp>
#include <imgui/imgui/backends/imgui_impl_glfw.cpp>

namespace Spectre {
	class RenderLayer : public Layer
	{
	public:
		RenderLayer()
			: generator(randomDevice())
		{}

		~RenderLayer() = default;

		void onAttach() override
		{
			/*glm::mat4 projection = glm::mat4(1.0f);
			projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
			ResourceManager::getShader("flat-color")->use();
			ResourceManager::getShader("flat-color")->setUniformMat4("uProjection", projection);*/
		}

		void onEvent(Event& event) override
		{
			std::cout << event.toString();
			camera.onEvent(event);
		}

		void onRender() override
		{
			std::uniform_real_distribution<> distribution(0, 1);

			glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/*glm::vec3 cubePositions[] = {
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

			const float radius = 10.0f;
			float cameraX = sin(glfwGetTime()) * radius;
			float cameraZ = cos(glfwGetTime()) * radius;

			glm::mat4 view = glm::mat4(1.0f);
			view = glm::lookAt(glm::vec3(cameraX, 0.0f, cameraZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			ResourceManager::getShader("3d")->setUniformMat4("uView", view);

			for (int i = 0; i < 10; i++) {
				renderer.drawCube(cubePositions[i], i);
			}*/

			for (int i = 1; i <= 50; i++) {
				for (int j = 1; j <= 50; j++) {
					float red = distribution(generator);
					float green = distribution(generator);
					float blue = distribution(generator);
					renderer.drawQuad(glm::vec2((float)(j*100), (float)(i*100)), glm::vec2(100.0f, 100.0f), glm::vec4(red, green, blue, 1.0f));
				}
			}

			//renderer.drawQuad(glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f), glm::vec4(1.0f, 0.7f, 0.2f, 1.0f));
			//renderer.drawQuad(glm::vec2(x, y), glm::vec2(200.0f, 200.0f), glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		}

	private:
		Renderer2D renderer;
		OrthographicCamera camera;

		std::random_device randomDevice;
		std::mt19937 generator;

		static glm::vec3 backgroundColor;
		static float x, y;

		friend class ImGuiLayer;
	};

	glm::vec3 RenderLayer::backgroundColor = glm::vec3(0.2f, 0.3f, 0.8f);
	float RenderLayer::x = 300.0f;
	float RenderLayer::y = 320.0f;

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() = default;
		~ImGuiLayer() = default;

		void onAttach() override
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
			ImGui_ImplOpenGL3_Init("#version 330");
		}

		void onDetach() override
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void onImGuiRender() override
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

			ImGui::Begin("Test Panel");
			ImGui::ColorEdit3("Background Color", &RenderLayer::backgroundColor.r);
			ImGui::SliderFloat("Big square X", &RenderLayer::x, 0.0f, 1280.0f);
			ImGui::SliderFloat("Big square Y", &RenderLayer::y, 0.0f, 720.0f);
			ImGui::End();

			ImGui::Begin("Statistics");
			ImGui::Text("Frametime: %.3f ms, FPS: %.1f", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			/*ImGui::Begin("Assets");
			ImGui::Image((void*)(intptr_t)ResourceManager::getTexture("grass-top")->getId(), ImVec2(64.0f, 64.0f), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)ResourceManager::getTexture("grass-side")->getId(), ImVec2(64.0f, 64.0f), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::Image((void*)(intptr_t)ResourceManager::getTexture("grass-bottom")->getId(), ImVec2(64.0f, 64.0f), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();*/
		}

		void begin()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void end()
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
	};

	class SpectreEditor : public Application
	{
	public:
		SpectreEditor()
		{
			// Load shaders
			ResourceManager::addShader("flat-color", "assets/shaders/flat-color.vert", "assets/shaders/flat-color.frag");

			addLayer(new RenderLayer());
			addLayer(new ImGuiLayer());
		}

		~SpectreEditor() = default;
	};
}

int main(int argc, char* argv[])
{
	std::unique_ptr<Spectre::SpectreEditor> editor = std::make_unique<Spectre::SpectreEditor>();
	editor->run();
}
