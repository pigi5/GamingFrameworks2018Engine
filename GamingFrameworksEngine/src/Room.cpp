
#include <algorithm>
#include "../header/Room.h"
#include "../header/Constants.h"
#include "../header/TriggerPresets.h"
#include "../header/Configurable.h"

const std::string Room::DIR_NAME = "rooms";
std::map<std::string, Room*> Room::objectMap;

void Room::createRoom(std::string name)
{
    Room* newType = new Room(name);

    if (objectMap.empty())
    {
        newType->is_default = true;
    }
    
    // add room to map, keyed by name
    if (!objectMap.emplace(newType->name, newType).second)
    {
        // if key already existed, throw error
        throw ConfigurationError("Room name \"" + newType->name + "\" is not unique.");
    }
}

Room::Room(std::string name)
{
    this->name = name;
    is_default = false;
}

Room::Room(const YAML::Node& config, bool shallow)
{
    name = config["name"].as<std::string>();
    is_default = config["default"].as<bool>();

    YAML::Node actorsNode = config["actors"];
    for (YAML::Node actor : actorsNode)
    {
        std::string typeName = actor["type"].as<std::string>();
            
        auto mapItem = ActorType::objectMap.find(typeName);
        if (mapItem == ActorType::objectMap.end())
        {
            throw ConfigurationError("Actor Type " + typeName + " does not exist.");
        }

        State startState(actor["startX"].as<float>(), actor["startY"].as<float>());

        bool followed = actor["follow"].as<bool>();
        startActors.push_back(std::make_tuple(typeName, startState, followed));
    }

    YAML::Node overlaysNode = config["overlays"];
    for (YAML::Node overlay : overlaysNode)
    {
        std::string typeName = overlay["type"].as<std::string>();
            
        auto mapItem = OverlayType::objectMap.find(typeName);
        if (mapItem == OverlayType::objectMap.end())
        {
            throw ConfigurationError("Overlay Type " + typeName + " does not exist.");
        }
        
        State startState(overlay["startX"].as<float>(), overlay["startY"].as<float>());
        startOverlays.push_back(std::make_tuple(typeName, startState));
    }
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Room& obj)
{
    out << YAML::Key << "name" << YAML::Value << obj.name;
    out << YAML::Key << "default" << YAML::Value << obj.is_default;

    out << YAML::Key << "actors" << YAML::Value << YAML::BeginSeq;
    for (auto tup : obj.startActors)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "type" << YAML::Value << std::get<std::string>(tup);
        out << YAML::Key << "startX" << YAML::Value << std::get<State>(tup).xPosition;
        out << YAML::Key << "startY" << YAML::Value << std::get<State>(tup).yPosition;
        out << YAML::Key << "follow" << YAML::Value << std::get<bool>(tup);
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    
    out << YAML::Key << "overlays" << YAML::Value << YAML::BeginSeq;
    for (auto tup : obj.startOverlays)
    {
        out << YAML::BeginMap;
        out << YAML::Key << "type" << YAML::Value << std::get<std::string>(tup);
        out << YAML::Key << "startX" << YAML::Value << std::get<State>(tup).xPosition;
        out << YAML::Key << "startY" << YAML::Value << std::get<State>(tup).yPosition;
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    return out;
}

Logger& operator<<(Logger& logger, const Room& obj) 
{
    logger << obj.name;
    return logger;
}

Room::~Room()
{
    for (Actor* actor : actors)
    {
        delete actor;
    }
    actors.clear();
    for (Overlay* overlay : overlays)
    {
        delete overlay;
    }
    overlays.clear();
}

void Room::reset()
{
    for (Actor* actor : actors)
    {
        delete actor;
    }
    actors.clear();
    for (Overlay* overlay : overlays)
    {
        delete overlay;
    }
    overlays.clear();
    for (auto& tup : startActors)
    {
        auto mapItem = ActorType::objectMap.find(std::get<std::string>(tup));
        if (mapItem == ActorType::objectMap.end())
        {
            throw ConfigurationError("Actor Type " + std::get<std::string>(tup) + " does not exist.");
        }

        Actor* actor = new Actor(this, mapItem->second, std::get<State>(tup));

        actors.push_back(actor);

        if (std::get<bool>(tup))
        {
            followedActor = actor;
        }
    }
    for (auto& tup : startOverlays)
    {
        auto mapItem = OverlayType::objectMap.find(std::get<std::string>(tup));
        if (mapItem == OverlayType::objectMap.end())
        {
            throw ConfigurationError("Overlay Type " + std::get<std::string>(tup) + " does not exist.");
        }

        Overlay* actor = new Overlay(this, mapItem->second, std::get<State>(tup));

        overlays.push_back(actor);
    }
}

void Room::step()
{
    // perform step triggers
    for (Actor* actor : actors)
    {
        actor->step();
    }
    for (Overlay* overlay : overlays)
    {
        overlay->step();
    }

    // perform physics
    for (Actor* actor : actors)
    {
        actor->move(actors);
    }

    // delete actors queued for deletion
    for (Actor* actor : actorDeleteQueue)
    {
        actors.remove(actor);
        delete actor;
    }
    actorDeleteQueue.clear();
    for (Overlay* overlay : overlayDeleteQueue)
    {
        overlays.remove(overlay);
        delete overlay;
    }
    overlayDeleteQueue.clear();
    
    // sort actors by depth
    actors.sort(Actor::compareDepth);
    overlays.sort(Actor::compareDepth);
}

void Room::interpolateState(float progress)
{
	for (Actor* actor : actors)
	{
		actor->interpolateState(progress);
	}
    for (Overlay* overlay : overlays)
    {
        overlay->interpolateState(progress);
    }
}

void Room::draw(sf::RenderWindow* window, sf::View* view)
{
    for (Actor* actor : actors)
    {
        actor->draw(window, view);
    }
}

void Room::drawHUD(sf::RenderWindow* window, sf::View* view)
{
    for (Overlay* overlay : overlays)
    {
        overlay->draw(window, view);
    }
}

void Room::fireTrigger(Trigger* trigger)
{
    for (Actor* actor : actors)
    {
        actor->fireTrigger(trigger);
    }
    for (Overlay* overlay : overlays)
    {
        overlay->fireTrigger(trigger);
    }
}

void Room::addActor(Actor* newActor)
{
    actors.push_back(newActor);
}

void Room::deleteActor(Actor* actor)
{
    // fire destroy trigger
    ActorTypeWrapper wrapper(actor->getType());
    trigger_preset::Destroy trigger(&wrapper);
    actor->fireTrigger(&trigger);
    actorDeleteQueue.insert(actor);
}

void Room::addOverlay(Overlay* newOverlay)
{
    overlays.push_back(newOverlay);
}

void Room::deleteOverlay(Overlay* overlay)
{
    // fire destroy trigger
    ActorTypeWrapper wrapper(overlay->getType());
    trigger_preset::Destroy trigger(&wrapper);
    overlay->fireTrigger(&trigger);
    overlayDeleteQueue.insert(overlay);
}

const std::set<Actor*>& Room::getActorDeleteQueue()
{
    return actorDeleteQueue;
}
const std::set<Overlay*>& Room::getOverlayDeleteQueue()
{
    return overlayDeleteQueue;
}

std::list<Actor*>* Room::getActors()
{
    return &actors;
}

std::list<Overlay*>* Room::getOverlays()
{
	return &overlays;
}

std::list<std::tuple<std::string, State, bool>>* Room::getStartActors()
{
	return &startActors;
}

std::list<std::tuple<std::string, State>>* Room::getStartOverlays()
{
	return &startOverlays;
}

Actor* Room::getFollowedActor() const
{
    return followedActor;
}

void Room::setFollowedActor(Actor* actor)
{
	this->followedActor = actor;
}

std::ostream& operator<<(std::ostream& output, const Room& object)
{
    output << " name: " << object.name;
   
    for (Actor* actor : object.actors)
    {
        output << " actor: " << actor->getType() << ", ";
    }
    return output;
}

void Room::setEngine(Engine* engine)
{
    this->engine = engine;
}

Engine* Room::getEngine() const
{
    return engine;
}

void Room::allMousePress(int x, int y, Trigger* trigger)
{
	for (Actor* actor : actors)
	{
		if(!actor->checkPressOn(x, y))
        {
			actor->fireTrigger(trigger);
        }
	}
	for (Overlay* overlay : overlays)
	{
		if(!overlay->checkPressOn(x, y))
        {
			overlay->fireTrigger(trigger);
        }
	}
}

void Room::allMouseRelease(int x, int y, Trigger* trigger)
{
	for (Actor* actor : actors)
	{
		if(!actor->checkReleaseOn(x, y))
        {
			actor->fireTrigger(trigger);
        }
	}
	for (Overlay* overlay : overlays)
	{
		if(!overlay->checkReleaseOn(x, y))
        {
			overlay->fireTrigger(trigger);
        }
	}
}