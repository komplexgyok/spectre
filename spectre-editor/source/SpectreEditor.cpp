#include <Spectre.h>

class SpectreEditor : public Spectre::Application
{
public:
	SpectreEditor()
	{}

	~SpectreEditor() = default;
};

int main(int argc, char* argv[])
{
	auto editor = new SpectreEditor();
	editor->run();
	delete editor;
}
