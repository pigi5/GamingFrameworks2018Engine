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
	std::string name;
	std::string fileName;

	Audio(std::string name, std::string filename);
	Audio(const YAML::Node&, bool);
	~Audio();
    

	virtual bool load() = 0;
	virtual void play() = 0;
	virtual void stop() = 0;

	bool operator<(const Audio& other) const;
	friend YAML::Emitter& operator<<(YAML::Emitter& out, const Audio& obj);
	friend Logger& operator<<(Logger&, const Audio&);
};