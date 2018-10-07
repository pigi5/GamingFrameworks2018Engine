#pragma once
#include <map>
#include <unordered_set>
#include <SFML/Window.hpp>
#include "TriggerPresets.h"
#include "ButtonStates.h"
#include "Utils.h"

class InputHandler {

private:
	std::unordered_set<sf::Keyboard::Key> heldKeys;

public:

	void handlePress(sf::Keyboard::Key k, Room* room) {
		heldKeys.insert(k);
                    
	    ButtonInputType b;
	    b.id = k;
	    b.state = ButtonState::PRESS;
	    trigger_preset::ButtonInput trigger(&b);
        
        room->fireTrigger(trigger);
	}

	void handleRelease(sf::Keyboard::Key k, Room* room) {
		heldKeys.erase(k);
                    
	    ButtonInputType b;
	    b.id = k;
	    b.state = ButtonState::RELEASE;
	    trigger_preset::ButtonInput trigger(&b);
        
        room->fireTrigger(trigger);
	}

	void handleHolds(Room* room) {
		for (sf::Keyboard::Key k : heldKeys) {
			ButtonInputType b;
			b.id = k;
			b.state = ButtonState::HOLD;
			trigger_preset::ButtonInput trigger(&b);
        
            room->fireTrigger(trigger);
		}
    }
};