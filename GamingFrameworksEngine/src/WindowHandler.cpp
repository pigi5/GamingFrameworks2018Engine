#include "../header/WindowHandler.h"
using namespace std;

WindowHandler::WindowHandler() {
	this->windowName = "Game Window";
	this->windowWidth = 800;
	this->windowHeight = 600;
	this->fullScreen = -1;
	defaultFont.loadFromFile("arial.ttf");
}
WindowHandler::WindowHandler(string name, int width, int height) {
	this->setWinName(name);
	this->setViewWidth(width);
	this->setViewHeight(height);
	this->fullScreen = -1;
	this->currentMode = sf::VideoMode(windowWidth, windowHeight);
	defaultFont.loadFromFile("arial.ttf");
}
WindowHandler::WindowHandler(string name, bool fullscreen) {
	this->setWinName(name);
	this->fullScreen = 1;
	defaultFont.loadFromFile("arial.ttf");
}

int WindowHandler::getViewWidth() {
	return windowWidth;
}
int WindowHandler::getViewHeight() {
	return windowHeight;
}
int WindowHandler::getWindowWidth() {
	return fullValid[0].width;
}
int WindowHandler::getWindowHeight() {
	return fullValid[0].height;
}
string WindowHandler::getWinName() {
	return windowName;
}

void WindowHandler::setViewWidth(int width) {
	this->windowWidth = width;
	this->currentMode = sf::VideoMode(windowWidth, windowHeight);
}
void WindowHandler::setViewHeight(int height) {
	this->windowHeight = height;
	this->currentMode = sf::VideoMode(windowWidth, windowHeight);
}
void WindowHandler::setWinName(string name) {
	this->windowName = name;
}

int WindowHandler::addText(string txt, int xPos, int yPos) {
	sf::Text text = sf::Text(txt, defaultFont, txt.size);
	text.setPosition(xPos, yPos);
	toDrawText.push_back(text);
	return toDrawText.size() - 1;
}
void WindowHandler::updateText(int id, string txt) {
	toDrawText[id].setString(txt);
}
void WindowHandler::setTextPosition(int id, int xPos, int yPos) {
	toDrawText[id].setPosition(xPos, yPos);
}
void WindowHandler::moveText(int id, int dx, int dy) {
	toDrawText[id].move(dx, dy);
}
void WindowHandler::rotateText(int id, int angle) {
	toDrawText[id].rotate(angle);
}
void WindowHandler::setTextSize(int id, int size) {
	toDrawText[id].setCharacterSize(size);
}
void WindowHandler::setTextFont(int id, sf::Font font) {
	toDrawText[id].setFont(font);
}
void WindowHandler::setTextStyle(int id, char style) {
	switch (style)
	{
	case 'r': toDrawText[id].setStyle(sf::Text::Regular);
		break;
	case 'b': toDrawText[id].setStyle(sf::Text::Bold);
		break;
	case 'i': toDrawText[id].setStyle(sf::Text::Italic);
		break;
	case 's': toDrawText[id].setStyle(sf::Text::StrikeThrough);
		break;
	}
}
void WindowHandler::setTextColor(int id, sf::Color color) {
	toDrawText[id].setFillColor(color);
}

int WindowHandler::addShape(int numPoints, int xPos, int yPos) {
	if (numPoints == 4) {
		sf::RectangleShape s = sf::RectangleShape(sf::Vector2f(1, 1));
		s.setPosition(xPos, yPos);
		toDrawShape.push_back(s);
		return toDrawShape.size - 1;
	}
	else {
		sf::CircleShape s = sf::CircleShape(0.5, numPoints);
		s.setPosition(xPos, yPos);
		toDrawShape.push_back(s);
		return toDrawShape.size - 1;
	}
	
}
int WindowHandler::addCustomShape(sf::Shape& shape) {
	toDrawShape.push_back(shape);
	return toDrawShape.size - 1;
}
void WindowHandler::setShapePosition(int id, int xPos, int yPos) {
	toDrawShape[id].setPosition(xPos, yPos);
}
void WindowHandler::moveShape(int id, int dx, int dy) {
	toDrawShape[id].move(dx, dy);
}
void WindowHandler::rotateShape(int id, int angle) {
	toDrawShape[id].rotate(angle);
}
void WindowHandler::setShapeSize(int id, int sizeX, int sizeY) {
	toDrawShape[id].setScale(sizeX, sizeY);
}
void WindowHandler::setShapeColorFill(int id, sf::Color color) {
	toDrawShape[id].setFillColor(color);
}
void WindowHandler::setShapeColorOutline(int id, sf::Color color) {
	toDrawShape[id].setOutlineColor(color);
}
void WindowHandler::setShapeOutlineThickness(int id, int size) {
	toDrawShape[id].setOutlineThickness(size);
}
void WindowHandler::setShapeTexture(int id, string filename, int xSize, int ySize) {
	sf::Texture* t = new sf::Texture();
	sf::IntRect r = sf::IntRect(sf::Vector2i(toDrawShape[id].getPosition().x, toDrawShape[id].getPosition().y), sf::Vector2i(xSize, ySize));
	t->loadFromFile(filename, r);
	toDrawShape[id].setTexture(t, true);
}

int WindowHandler::addVertexList(sf::PrimitiveType type, int vertexCount, sf::Vertex vList[]) {
	sf::VertexArray va = sf::VertexArray(type, vertexCount);
	for (int i = 0; i < vertexCount; i++) {
		va[i] = vList[i];
	}
	toDrawVList.push_back(va);
	return toDrawVList.size() - 1;
}
void WindowHandler::changeVertexListType(int id, sf::PrimitiveType type) {
	toDrawVList[id].setPrimitiveType(type);
}
void WindowHandler::changeVertexPosition(int id, int index, int posX, int posY) {
	toDrawVList[id][index].position.x = posX;
	toDrawVList[id][index].position.y = posY;
}
void WindowHandler::changeVertexColor(int id, int index, sf::Color color) {
	toDrawVList[id][index].color = color;
}
void WindowHandler::addVertex(int id, sf::Vertex v) {
	toDrawVList[id].append(v);
}

int WindowHandler::addSprite(string filename, int xPos, int yPos, int xSize, int ySize) {
	sf::Texture t = sf::Texture();
	sf::IntRect r = sf::IntRect(sf::Vector2i(xPos, yPos), sf::Vector2i(xSize, ySize));
	t.loadFromFile(filename, r);
	sf::Sprite s = sf::Sprite(t, r);
	toDrawSprite.push_back(s);
	return toDrawSprite.size() - 1;
}
void WindowHandler::setSpritePosition(int id, int xPos, int yPos) {
	toDrawSprite[id].setPosition(xPos, yPos);
}
void WindowHandler::moveSprite(int id, int dx, int dy) {
	toDrawSprite[id].move(dx, dy);
}
void WindowHandler::rotateSprite(int id, int angle) {
	toDrawSprite[id].rotate(angle);
}
void WindowHandler::setSpriteSize(int id, int sizeX, int sizeY) {
	toDrawSprite[id].setScale(sizeX, sizeY);
}
void WindowHandler::setSpriteTexture(int id, string filename, int xSize, int ySize) {
	sf::Texture t = sf::Texture();
	sf::IntRect r = sf::IntRect(sf::Vector2i(toDrawShape[id].getPosition().x, toDrawShape[id].getPosition().y), sf::Vector2i(xSize, ySize));
	t.loadFromFile(filename, r);
	toDrawSprite[id].setTexture(t);
	toDrawSprite[id].setTextureRect(r);
}

void WindowHandler::toggleFullScreen() {
	this->fullScreen *= -1;
}
void WindowHandler::openWindow() {
	if (fullScreen) {
		window.create(fullValid[0], windowName, sf::Style::Fullscreen);
	}
	else {
		window.create(currentMode, windowName, sf::Style::Titlebar | sf::Style::Close);
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);

		for (int i = 0; i < toDrawText.size(); i++) {
			window.draw(toDrawText[i]);
		}
		for (int i = 0; i < toDrawSprite.size(); i++) {
			window.draw(toDrawText[i]);
		}
		for (int i = 0; i < toDrawShape.size(); i++) {
			window.draw(toDrawText[i]);
		}
		for (int i = 0; i < toDrawVList.size(); i++) {
			window.draw(toDrawText[i]);
		}

		window.display();
	}
}