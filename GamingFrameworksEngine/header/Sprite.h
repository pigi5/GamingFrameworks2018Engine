#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "yaml-cpp/yaml.h"
#include "Logger.h"

// Holds an animation and a hitbox for objects to use.
class Sprite
{
public:	
	sf::Sprite s;
	std::vector<sf::Texture*> textures;
	std::vector<std::string> textrFiles;

	static const std::string DIR_NAME;
	static std::unordered_map<std::string, Sprite*> objectMap;
	static void createSprite(std::string);

	std::string name;

	Sprite(std::string name);
	Sprite(const YAML::Node&, bool);
    ~Sprite();

    float getRecommendedWidth();
    float getRecommendedHeight();

	void draw(sf::RenderWindow* window, float index, float xPos, float yPos, float xScale, float yScale, float angle);
	friend YAML::Emitter& operator<<(YAML::Emitter& out, const Sprite& obj);
    friend Logger& operator<<(Logger&, const Sprite&);
};