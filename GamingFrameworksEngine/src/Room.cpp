#include "../header/Room.h"

std::map<const std::string, Room*> Room::objectMap;

Room::Room(const YAML::Node& config)
{
    name = config["name"].as<std::string>();
    
    if (config["default"])
    {
        is_default = config["default"].as<bool>();
    }
    else
    {
        is_default = false;
    }

    if (config["actors"])
    {
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
            actors.push_back(new Actor(mapItem->second, startState));
        }
    }

    if (config["overlays"])
    {
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
            overlays.push_back(new Overlay(mapItem->second, startState));
        }
    }
}

void Room::step()
{
    for (Actor* actor : actors)
    {
        actor->step();
    }
    for (Actor* overlay : overlays)
    {
        overlay->step();
    }

    for (Actor* actor : actors)
    {
        actor->move(actors);
    }
    for (Actor* overlay : overlays)
    {
        overlay->move(overlays);
    }
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

void Room::addActor(Actor* newActor)
{
    actors.push_back(newActor);
}

void Room::addOverlay(Actor* newOverlay)
{
    overlays.push_back(newOverlay);
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