#pragma once

#include <SFML\Audio.hpp>
#include <map>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "../header/Configurable.h"
#include "../header/Trigger.h"
#include "../header/Action.h"
#include "../header/dirent.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"

class Audio
{
private:

public:
	static const std::string DIR_NAME;

	static std::map<const std::string, Audio*> audioMap;
	
	//triggers/actions
	std::map<const Trigger*, std::list<Action*>> actionMap;

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
};