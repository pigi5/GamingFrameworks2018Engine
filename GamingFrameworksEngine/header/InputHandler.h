#pragma once
#include <map>
#include <SFML/Window.hpp>
#include "..\header\TriggerPresets.h"


struct ButtonInputType 
{
	short id;
	bool state = false;

	bool operator<(const ButtonInputType& other) const
	{
		return id * 2 + state < other.id * 2 + other.state;
	}

    ButtonInputType() {}

    ButtonInputType(const YAML::Node& config)
    {
        id = config["id"].as<short>();
        state = parseState(config["state"].as<std::string>());
    }
    
    static bool parseState(std::string state)
    {
        if (state != "up" && state != "down")
        {
            throw ConfigurationError("Button Input Type state must be \"down\" or \"up\"");
        }
        return state == "up";
    }
};

class InputHandler {

private:
	// map key codes to their state 
	std::map <short, sf::Keyboard::Key> keyMapping;
	// Keep track of what id number to use.
	short currId = 0;

	// make sure a given key is in the map, and if its not, add it
	void checkInMap(sf::Keyboard::Key k) {
		auto search = keyMapping.find(k);
		if (search != keyMapping.end() && k != -1) {
			keyMapping.insert(std::pair<short, sf::Keyboard::Key>(currId, k));
			currId++;

		}
	}

public:

	void handlePress(sf::Keyboard::Key k, bool state) {
		checkInMap(k);
		ButtonInputType b;
		b.id = keyMapping.find(k)->first;
		b.state = state;
		trigger_preset::ButtonInput trigger(&b);
	}

	void changeMapping(sf::Keyboard::Key k) {
		auto search = keyMapping.find(k);
		if (search != keyMapping.end() && k != -1) {
			search->second = k;
		}
	}

	std::map <short, sf::Keyboard::Key> getKeyMap() {
		return keyMapping;
	}
};