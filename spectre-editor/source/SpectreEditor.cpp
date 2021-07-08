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

			// Load textures
			ResourceManager::addTexture("white-texture", "assets/textures/white-texture.png");
			ResourceManager::addTexture("grass-top", "assets/textures/grass-top.png");

			addLayer(std::make_unique<EditorLayer>());
		}
	};
}

int main(int argc, char* argv[])
{
	std::unique_ptr<Spectre::SpectreEditor> editor = std::make_unique<Spectre::SpectreEditor>();
	editor->run();
}
