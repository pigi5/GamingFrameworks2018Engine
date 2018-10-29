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
    std::list<Overlay*> overlays;
    
    std::set<Actor*> actorDeleteQueue;
    std::set<Overlay*> overlayDeleteQueue;

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
    void addOverlay(Overlay*);
    void deleteOverlay(Overlay*);

    std::list<Actor*>* getActors();
	std::list<Overlay*>* getOverlays();

    Actor* getFollowedActor() const;
	void setFollowedActor(Actor* actor);

    friend std::ostream& operator<<(std::ostream&, const Room&);
    
    void setEngine(Engine*);
    Engine* getEngine() const;


};