#include "../header/Room.h"
#include "../header/Constants.h"
#include "../header/TriggerPresets.h"

const std::string Room::DIR_NAME = "rooms";

std::map<const std::string, Room*> Room::objectMap;

Room::Room()
{
    name = "unknown";
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
            std::stringstream errorMessage;
            errorMessage << "Actor Type " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }

        State startState(actor["startX"].as<float>(), actor["startY"].as<float>());
        actors.push_back(new Actor(this, mapItem->second, startState));
    }

    YAML::Node overlaysNode = config["overlays"];
    for (YAML::Node overlay : overlaysNode)
    {
        std::string typeName = overlay["type"].as<std::string>();
            
        auto mapItem = OverlayType::objectMap.find(typeName);
        if (mapItem == OverlayType::objectMap.end())
        {
            std::stringstream errorMessage;
            errorMessage << "Overlay Type " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }

        State startState(overlay["startX"].as<float>(), overlay["startY"].as<float>());
        overlays.push_back(new Overlay(this, mapItem->second, startState));
    }
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
                fireTrigger(trigger_preset::Timer(&Index(i)));
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

void Room::fireTrigger(const Trigger& trigger)
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

std::ostream& operator<<(std::ostream& output, const Room& object)
{
    output << " name: " << object.name;
   
    for (Actor* actor : object.actors)
    {
        output << " actor: " << actor->getType() << ", ";
    }
    return output;
}