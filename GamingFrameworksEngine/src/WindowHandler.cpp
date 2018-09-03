#include "../header/WindowHandler.h"
using namespace std;

WindowHandler::WindowHandler() {
	this->windowName = "Game Window";
	this->windowWidth = 800;
	this->windowHeight = 600;
	this->fullScreen = -1;
}
WindowHandler::WindowHandler(string name, int width, int height) {
	this->setWinName(name);
	this->setWinWidth(width);
	this->setWinHeight(height);
	this->fullScreen = -1;
	this->currentMode = sf::VideoMode(windowWidth, windowHeight);
}
WindowHandler::WindowHandler(string name, bool fullscreen) {
	this->setWinName(name);
	this->fullScreen = 1;
}

int WindowHandler::getWinWidth() {
	return windowWidth;
}
int WindowHandler::getWinHeight() {
	return windowHeight;
}
string WindowHandler::getWinName() {
	return windowName;
}
void WindowHandler::setWinWidth(int width) {
	this->windowWidth = width;
	this->currentMode = sf::VideoMode(windowWidth, windowHeight);
}
void WindowHandler::setWinHeight(int height) {
	this->windowHeight = height;
	this->currentMode = sf::VideoMode(windowWidth, windowHeight);
}
void WindowHandler::setWinName(string name) {
	this->windowName = name;
}
void WindowHandler::toggleFullScreen() {
	this->fullScreen *= -1;
}
void WindowHandler::openWindow(void* function) {
	if (fullScreen) {
		window.create(fullValid[0], windowName, sf::Style::Fullscreen);
	}
	else {
		window.create(currentMode, windowName, sf::Style::Titlebar | sf::Style::Close);
	}

	while (window.isOpen()) {
		function;
	}
}