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
    std::list<std::tuple<std::string, State, bool>> startActors;
    std::list<Overlay*> overlays;
    std::list<std::tuple<std::string, State>> startOverlays;
    
    std::set<Actor*> actorDeleteQueue;
    std::set<Overlay*> overlayDeleteQueue;

    Actor* followedActor;

    const YAML::Node* config;

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
    
    void reset();
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

    const std::set<Actor*>& getActorDeleteQueue();
    const std::set<Overlay*>& getOverlayDeleteQueue();
    
    std::list<Actor*>* getActors();
	std::list<Overlay*>* getOverlays();
    std::list<std::tuple<std::string, State, bool>>* getStartActors();
	std::list<std::tuple<std::string, State>>* getStartOverlays();

    Actor* getFollowedActor() const;
	void setFollowedActor(Actor* actor);

    friend std::ostream& operator<<(std::ostream&, const Room&);
    
    void setEngine(Engine*);
    Engine* getEngine() const;


};