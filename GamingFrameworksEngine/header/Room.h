#pragma once

#include <list>
#include "Actor.h"
#include "Overlay.h"
#include "yaml-cpp/yaml.h"
#include "Logger.h"

class Engine;

// Similar to a stage.
class Room
{
private:
    std::list<Actor*> actors;
    std::list<Actor*> overlays;
    
    std::list<Actor*> actorDeleteQueue;
    std::list<Actor*> overlayDeleteQueue;

    Actor* followedActor;

    Engine* engine;
public:
    static const std::string DIR_NAME;
    static std::map<std::string, Room*> objectMap;
    
    std::string name;
    bool is_default;
	ActorType* place;
    
    static void createRoom(std::string);

    Room(std::string);
    Room(const YAML::Node&, bool);
    
    friend YAML::Emitter& operator<<(YAML::Emitter&, const Room&);
    friend Logger& operator<<(Logger&, const Room&);

    ~Room();

    void step();
	void interpolateState(float);
    void draw(sf::RenderWindow* window, sf::View* view);
    void drawHUD(sf::RenderWindow* window, sf::View* view);
    
    void fireTrigger(Trigger* trigger);

	void allMousePress(int, int, Trigger* trigger);
	void allMouseRelease(int, int, Trigger* trigger);
    
    void addActor(Actor*);
    void deleteActor(Actor*);
    void addOverlay(Actor*);
    void deleteOverlay(Actor*);

    std::list<Actor*> getActors() const;

    Actor* getFollowedActor() const;
	void setFollowedActor(Actor* actor);

    friend std::ostream& operator<<(std::ostream&, const Room&);
    
    void setEngine(Engine*);
    Engine* getEngine() const;


};