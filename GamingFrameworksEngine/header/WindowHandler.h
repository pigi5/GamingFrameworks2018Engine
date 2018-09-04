#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics.hpp>
#include <vector>
#include <string>
using namespace std;

class WindowHandler {
private:
	int fullScreen;
	int windowWidth, windowHeight;
	string windowName;
	sf::VideoMode currentMode = sf::VideoMode(800,600);
	vector<sf::VideoMode> fullValid = currentMode.getFullscreenModes();
	sf::RenderWindow window;
public:
	WindowHandler();
	WindowHandler(string name, int width, int height);
	WindowHandler(string name, bool fullscreen);

	int getWinWidth();
	int getWinHeight();
	string getWinName();
	void setWinWidth(int width);
	void setWinHeight(int height);
	void setWinName(string name);
	void toggleFullScreen();
	void openWindow(vector<sf::Drawable> toDraw);
};