#pragma once

#include <list>
#include "Actor.h"
#include "Overlay.h"
#include "yaml-cpp/yaml.h"

// Similar to a stage.
class Room
{
public:
    static const int NUM_TIMERS = 10;
private:
    std::list<Actor*> actors;
    std::list<Actor*> overlays;
    
    std::list<Actor*> actorDeleteQueue;
    std::list<Actor*> overlayDeleteQueue;

    float timers[NUM_TIMERS];
public:
    static const std::string DIR_NAME;
    static std::map<const std::string, Room*> objectMap;
    
    std::string name;
    bool is_default;
    
    Room();
    Room(const YAML::Node&, bool);

    friend YAML::Emitter& operator<<(YAML::Emitter&, const Room&);

    ~Room();

    void step();
	void interpolateState(float);
    void draw(sf::RenderWindow* window, sf::View* view);
    void drawHUD(sf::RenderWindow* window, sf::View* view);
    
    void fireTrigger(const Trigger&);
    
    void addActor(Actor*);
    void deleteActor(Actor*);
    void addOverlay(Actor*);
    void deleteOverlay(Actor*);

    void startTimer(int, float);

    std::list<Actor*> getActors() const;

    friend std::ostream& operator<<(std::ostream&, const Room&);
};