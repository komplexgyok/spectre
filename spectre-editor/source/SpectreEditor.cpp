#include <Spectre.h>

#include <memory>

#include "EditorLayer.h"

namespace Spectre {
	class SpectreEditor : public Application
	{
	public:
		SpectreEditor()
		{
			// Load shaders
			ResourceManager::addShader("flat-color", "assets/shaders/flat-color.vert", "assets/shaders/flat-color.frag");
			ResourceManager::addShader("texture", "assets/shaders/texture.vert", "assets/shaders/texture.frag");
			ResourceManager::addShader("mesh", "assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
			ResourceManager::addShader("light", "assets/shaders/light.vert", "assets/shaders/light.frag");
			ResourceManager::addShader("model", "assets/shaders/model.vert", "assets/shaders/model.frag");
			ResourceManager::addShader("solid", "assets/shaders/solid.vert", "assets/shaders/solid.frag");

			// Load textures
			ResourceManager::addTexture("white-texture", "assets/textures/white-texture.png");
			ResourceManager::addTexture("grass-top", "assets/textures/grass-top.png");
			ResourceManager::addTexture("container", "assets/textures/container.png");
			ResourceManager::addTexture("container-specular", "assets/textures/container-specular.png");
			ResourceManager::addTexture("matrix", "assets/textures/matrix.jpg");
			ResourceManager::addTexture("folder", "assets/textures/folder.png");

			addLayer(std::make_unique<EditorLayer>());
		}
	};
}

int main(int argc, char* argv[])
{
	std::unique_ptr<Spectre::SpectreEditor> editor = std::make_unique<Spectre::SpectreEditor>();
	editor->run();
}
