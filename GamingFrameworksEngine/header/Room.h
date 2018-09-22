#pragma once

#include <list>
#include "Actor.h"
#include "Overlay.h"
#include "yaml-cpp/yaml.h"

// Similar to a stage.
class Room
{
private:
    std::list<Actor*> actors;
    std::list<Actor*> overlays;
public:
    static std::map<const std::string, Room*> objectMap;
    
    std::string name;
    bool is_default;

    Room(const YAML::Node&);

    void step();
	void interpolateState(float);
    void draw(sf::RenderWindow* window, sf::View* view);
    void drawHUD(sf::RenderWindow* window, sf::View* view);
    
    void addActor(Actor*);
    void addOverlay(Actor*);

    friend std::ostream& operator<<(std::ostream&, const Room&);
};