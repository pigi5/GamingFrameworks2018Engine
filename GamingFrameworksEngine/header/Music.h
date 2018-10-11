#pragma once

#include "Audio.h"

class Music : public Audio
{
private:
	sf::Music music;

public:
	static const std::string DIR_NAME;
	static std::unordered_map<std::string, Music*> objectMap;

	static void createMusic(std::string, std::string);

    Music(std::string name, std::string fileName) : Audio(name, fileName) {};
	Music(const YAML::Node& config, bool shallow) : Audio(config, shallow) {};
	~Music();
    
	bool load();
	void play();
	void stop();
};