#pragma once
#include <map>
#include <SFML/Window.hpp>
#include "..\GamingFrameworksEngine\header\TriggerPresets.h"




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