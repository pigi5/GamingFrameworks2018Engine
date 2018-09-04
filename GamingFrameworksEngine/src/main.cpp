#include "../header/Engine.h"
#include <SFML\Graphics\RenderWindow.hpp>

int main()
{
	sf::VideoMode defaultMode = sf::VideoMode(800,600);
	sf::RenderWindow* window = new sf::RenderWindow();
	
	window->create(defaultMode, "Game Window", sf::Style::Titlebar | sf::Style::Close);
    Engine engine;
    engine.run(window);

    return 0;
}