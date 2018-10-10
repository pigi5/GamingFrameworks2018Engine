#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "Logger.h"

class Audio
{
private:

public:
	static const std::string DIR_NAME;

	static std::unordered_map<std::string, Audio*> objectMap;

	static void createAudio(std::string);

	std::string name;
	std::string fileName;
	std::string audioType;

	Audio();
	Audio(std::string name);
	Audio(const YAML::Node&, bool);
	~Audio();
	bool operator<(const Audio& other) const;
	friend YAML::Emitter& operator<<(YAML::Emitter& out, const Audio& obj);
	friend Logger& operator<<(Logger&, const Audio&);
};