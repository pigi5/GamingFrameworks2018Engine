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
	    sf::VideoMode defaultMode = sf::VideoMode(800,600);
	    sf::RenderWindow window;
	
	    window.create(defaultMode, "Game Window", sf::Style::Titlebar | sf::Style::Close);

	    Engine::getInstance().run(&window);
    }
    catch (const ConfigurationError& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}