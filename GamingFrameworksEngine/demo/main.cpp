#include "../header/Engine.h"
#include <SFML\Graphics\RenderWindow.hpp>
#include <iostream>
#include <direct.h> 

int main()
{
    char a;
    // print current working directory
    char buffer[256];
    char *val = _getcwd(buffer, sizeof(buffer));
    if (val) 
    {
        std::cout << "Running from: " << buffer << std::endl;
    }

    try 
    {
	    Engine::getInstance().run();
    }
    catch (const ConfigurationError& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}