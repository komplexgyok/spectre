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

			addLayer(new EditorLayer());
		}

		~SpectreEditor() = default;
	};
}

int main(int argc, char* argv[])
{
	std::unique_ptr<Spectre::SpectreEditor> editor = std::make_unique<Spectre::SpectreEditor>();
	editor->run();
}
