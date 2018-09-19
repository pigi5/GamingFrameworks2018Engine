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
	std::map <sf::Keyboard::Key, ButtonInputType> keyMapping;
	short currId = 0;

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
	// map key codes to their state 


	void setState(sf::Keyboard::Key k, bool state) {
		checkInMap(k);
		auto search = keyMapping.find(k);
		search->second.state = state;
	}
};