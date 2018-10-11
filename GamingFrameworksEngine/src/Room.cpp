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

        Actor* newActor = new Actor(this, mapItem->second, startState);
        if (actor["follow"].as<bool>())
        {
            followedActor = newActor;
        }
        actors.push_back(newActor);
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
        overlays.push_back(new Overlay(this, mapItem->second, startState));
    }
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Room& obj)
{
    out << YAML::Key << "name" << YAML::Value << obj.name;
    out << YAML::Key << "default" << YAML::Value << obj.is_default;

    out << YAML::Key << "actors" << YAML::Value << YAML::BeginSeq;
    for (Actor* actor : obj.actors)
    {
        out << YAML::BeginMap;
        out << *actor;
        out << YAML::Key << "follow" << YAML::Value << (actor == obj.followedActor);
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    
    out << YAML::Key << "overlays" << YAML::Value << YAML::BeginSeq;
    for (Actor* overlay : obj.overlays)
    {
        out << YAML::BeginMap;
        out << *overlay;
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
    for (Actor* overlay : overlays)
    {
        delete overlay;
    }
}

void Room::step()
{
    // perform step triggers
    for (Actor* actor : actors)
    {
        actor->step();
    }
    for (Actor* overlay : overlays)
    {
        overlay->step();
    }

    // decrement counters
    for (int i = 0; i < Room::NUM_TIMERS; i++)
    {
        if (timers[i] > 0)
        {
            timers[i] -= engine_constant::PHYSICS_DELTA_TIME;
            if (timers[i] <= 0)
            {
                Index wrapper(i);
                trigger_preset::Timer trigger(&wrapper);
                fireTrigger(&trigger);
            }
        }
    }

    // perform physics
    for (Actor* actor : actors)
    {
        actor->move(actors);
    }
    for (Actor* overlay : overlays)
    {
        overlay->move(overlays);
    }

    // delete actors queued for deletion
    for (Actor* actor : actorDeleteQueue)
    {
        actors.remove(actor);
        delete actor;
    }
    actorDeleteQueue.clear();
    for (Actor* overlay : overlayDeleteQueue)
    {
        overlays.remove(overlay);
        delete overlay;
    }
    actorDeleteQueue.clear();
}

void Room::interpolateState(float progress)
{
	for (Actor* actor : actors)
	{
		actor->interpolateState(progress);
	}
    for (Actor* overlay : overlays)
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
    for (Actor* overlay : overlays)
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
    for (Actor* overlay : overlays)
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
    actorDeleteQueue.push_back(actor);
}

void Room::addOverlay(Actor* newOverlay)
{
    overlays.push_back(newOverlay);
}

void Room::deleteOverlay(Actor* overlay)
{
    overlayDeleteQueue.push_back(overlay);
}

void Room::startTimer(int index, float time)
{
    timers[index] = time;
}

std::list<Actor*> Room::getActors() const
{
    return actors;
}

Actor* Room::getFollowedActor() const
{
    return followedActor;
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
		actor->fireTrigger(trigger);
		actor->checkPressOn(x, y);
	}
}

void Room::allMouseRelease(int x, int y, Trigger* trigger)
{
	for (Actor* actor : actors)
	{
		actor->fireTrigger(trigger);
		actor->checkReleaseOn(x, y);
	}
}