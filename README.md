# Spectre Game Engine

It's a 2D game engine written in C++. It only supports Visual Studio 2019 on Windows 64-bit at the moment.

Setup/usage:
1. Clone the github repository:
```
git clone https://github.com/komplexgyok/spectre <path>
```
2. Run the generate.bat script in the scripts folder, that will generate and configure the required VS solution and all the projects.

3. Use the included Sandbox application or create your own game/application class by inheriting from the Spectre Application class:
```c++
#include <Spectre.h>

class Game : public Spectre::Application
{
public:
    Game();
    ~Game();
}

int main(int argc, char* argv[])
{
    auto game = new Game();
    game->run();
    delete game;
}
```
