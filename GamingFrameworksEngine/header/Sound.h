#pragma once

#include "Audio.h"

class Sound : public Audio
{
private:
	sf::Sound sound;
	sf::SoundBuffer buffer;

public:
	static const std::string DIR_NAME;
	static std::unordered_map<std::string, Sound*> objectMap;

	static void createSound(std::string, std::string);

    Sound(std::string name, std::string fileName);
	Sound(const YAML::Node&, bool);
	~Sound();

	bool load();
	void play();
	void stop();
};
