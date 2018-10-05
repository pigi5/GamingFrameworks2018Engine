#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include "Hitbox.h"
#include "yaml-cpp/yaml.h"

using namespace std;

// Holds an animation and a hitbox for objects to use.
class Sprite
{
private:
	Hitbox* hitbox;
	
	sf::Sprite s;
	vector<sf::Texture*> textures;
	vector<string> textrFiles;
	int xSize, ySize;
	int xPos, yPos;
	int numFrames;

public:
	static const std::string DIR_NAME;
	string name;
	static std::map<const std::string, Sprite*> objectMap;
	static void createSprite(std::string);

	Sprite();
	Sprite(string name);
    Sprite(int xPos, int yPos);
	Sprite(const YAML::Node&, bool);
    ~Sprite();

    Hitbox* getHitbox() const;

	void changeTexture(int pos, string filename);
	void setPosition(int xPos, int yPos);
	void setSprite(int pos);
	void move(int dx, int dy);
	void rotate(int angle);
	void setName(string name);
	void setSize(int sizeX, int sizeY);
	void addTexture(string filename);
	void setPartialTexture(string filename, int xSize, int ySize);
	vector<sf::Sprite> parseSpriteSheet(string filename, int xSize, int ySize, int num);
	void draw(sf::RenderWindow* window);
	friend YAML::Emitter& operator<<(YAML::Emitter& out, const Sprite& obj);
};