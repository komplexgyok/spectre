#include <Spectre.h>

class SandboxApplication : public Spectre::Application
{
public:
	SandboxApplication()
	{}

	~SandboxApplication() = default;
};

int main(int argc, char* argv[])
{
	auto application = new SandboxApplication();
	application->run();
	delete application;
}
