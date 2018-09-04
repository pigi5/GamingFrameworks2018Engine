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
	sf::Font defaultFont = sf::Font();
	string windowName;
	sf::VideoMode currentMode = sf::VideoMode(800,600);
	vector<sf::VideoMode> fullValid = currentMode.getFullscreenModes();
	sf::RenderWindow window;
	vector<sf::Text> toDrawText;
	vector<sf::Shape> toDrawShape;
	vector<sf::VertexArray> toDrawVList;
	vector<sf::Sprite> toDrawSprite;
public:
	WindowHandler();
	WindowHandler(string name, int width, int height);
	WindowHandler(string name, bool fullscreen);

	int getViewWidth();
	int getViewHeight();
	int getWindowWidth();
	int getWindowHeight();
	string getWinName();

	void setViewWidth(int width);
	void setViewHeight(int height);
	void setWinName(string name);

	int addText(string txt, int xPos, int yPos);
	void updateText(int id, string txt);
	void setTextPosition(int id, int xPos, int yPos);
	void moveText(int id, int dx, int dy);
	void rotateText(int id, int angle);
	void setTextSize(int id, int size);
	void setTextFont(int id, sf::Font font);
	void setTextStyle(int id, char style);
	void setTextColor(int id, sf::Color color);

	int addShape(int numPoints, int xPos, int yPos);
	int addCustomShape(sf::Shape& shape);
	void setShapePosition(int id, int xPos, int yPos);
	void moveShape(int id, int dx, int dy);
	void rotateShape(int id, int angle);
	void setShapeSize(int id, int sizeX, int sizeY);
	void setShapeColorFill(int id, sf::Color color);
	void setShapeColorOutline(int id, sf::Color color);
	void setShapeOutlineThickness(int id, int size);
	void setShapeTexture(int id, string filename, int xSize, int ySize);

	int addVertexList(sf::PrimitiveType type, int vertexCount, sf::Vertex vList[]);
	void changeVertexListType(int id, sf::PrimitiveType type);
	void changeVertexPosition(int id, int index, int posX, int posY);
	void changeVertexColor(int id, int index, sf::Color color);
	void addVertex(int id, sf::Vertex v);

	int addSprite(string filename, int xPos, int yPos, int xSize, int ySize);
	void setSpritePosition(int id, int xPos, int yPos);
	void moveSprite(int id, int dx, int dy);
	void rotateSprite(int id, int angle);
	void setSpriteSize(int id, int sizeX, int sizeY);
	void setSpriteTexture(int id, string filename, int xSize, int ySize);

	void toggleFullScreen();
	void openWindow();
};