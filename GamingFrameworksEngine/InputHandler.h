#pragma once
#include <map>
#include <SFML/Window.hpp>


// this should probably be moved to an appropriate file (watching circular dependencies)
struct ButtonInputType
{
	short id;
	bool state = false;

	bool operator<(const ButtonInputType& other) const
	{
		return id * 2 + state < other.id * 2 + other.state;
	}
};

class InputHandler {

private:
	// map key codes to their state 
	std::map <sf::Keyboard::Key, ButtonInputType> keyMapping;
	// Keep track of what id number to use.
	short currId = 0;

	// make sure a given key is in the map, and if its not, add it
	void checkInMap(sf::Keyboard::Key k) {
		auto search = keyMapping.find(k);
		if (search != keyMapping.end() && k != -1) {
			ButtonInputType b;
			b.id = currId;
			keyMapping.insert(std::pair< sf::Keyboard::Key, ButtonInputType>(k,b));
			currId++;

		}
	}

public:

	void setState(sf::Keyboard::Key k, bool state) {
		checkInMap(k);
		auto search = keyMapping.find(k);
		search->second.state = state;
	}

	std::map <sf::Keyboard::Key, ButtonInputType> getKeyMap() {
		return keyMapping;
	}
};