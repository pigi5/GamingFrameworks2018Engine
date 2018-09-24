#pragma once

#include <cmath>
#include "Action.h"
#include "Room.h"
#include "Actor.h"
#include "Utils.h"
#include "yaml-cpp/yaml.h"

namespace action_preset
{
    class ApplyForceX : public Action
    {
    private:
        int acceleration;
    public:
        ApplyForceX(int acceleration)
        {
            this->acceleration = acceleration;
        }

        ApplyForceX(const YAML::Node& node)
        {
            acceleration = node["acceleration"].as<int>();
        }
    
        void run(Actor* actor)
        {
            actor->setXAcceleration(acceleration);
        }
    };
    
    // instantly move to the position of the nearest actor of the given type
    class MoveToNearest : public Action
    {
    private:
        ActorType* type;
    public:
        MoveToNearest(ActorType* type)
        {
            this->type = type;
        }

        MoveToNearest(const YAML::Node& node)
        {
            std::string typeName = node["type"].as<std::string>();
            auto mapItem = ActorType::objectMap.find(typeName);
            if (mapItem == ActorType::objectMap.end())
            {
                std::stringstream errorMessage;
                errorMessage << "Actor Type " << typeName << " does not exist.";
                throw ConfigurationError(errorMessage.str());
            }

            this->type = mapItem->second;
        }
    
        void run(Actor* actor)
        {
            auto nearest = engine_util::findNearest(actor, actor->getRoom()->getActors());
            actor->setPosition(nearest.first->getState().xPosition, nearest.first->getState().yPosition);
        }
    };
    
    // set the speed of the actor to move at the given speed to the 
    // position of the nearest actor of the given type
    class MoveTowardNearest : public Action
    {
    private:
        ActorType* type;
        float speed;
    public:
        MoveTowardNearest(ActorType* type, float speed)
        {
            this->type = type;
            this->speed = speed;
        }

        MoveTowardNearest(const YAML::Node& node)
        {
            std::string typeName = node["type"].as<std::string>();
            auto mapItem = ActorType::objectMap.find(typeName);
            if (mapItem == ActorType::objectMap.end())
            {
                std::stringstream errorMessage;
                errorMessage << "Actor Type " << typeName << " does not exist.";
                throw ConfigurationError(errorMessage.str());
            }

            this->type = mapItem->second;
            this->speed = node["speed"].as<float>();
        }
    
        void run(Actor* actor)
        {
            auto nearest = engine_util::findNearest(actor, actor->getRoom()->getActors());
            
            State distanceVector = nearest.first->getState() - actor->getState();
            // special case to avoid divide-by-zero
            if (distanceVector.xPosition == 0)
            {
                actor->setXSpeed(0);
                actor->setYSpeed(speed);
                return;
            }
            // calculate speeds
            float angle = atan(distanceVector.yPosition / distanceVector.xPosition);
            float xSpeed = speed * cos(angle) * engine_util::sign(distanceVector.yPosition);
            float ySpeed = speed * sin(angle) * engine_util::sign(distanceVector.yPosition);

            actor->setXSpeed(xSpeed);
            actor->setYSpeed(ySpeed);
        }
    };
    
    // move the actor to the given location
    class MoveTo : public Action
    {
    private:
        float xPosition;
        float yPosition;
    public:
        MoveTo(float xPosition, float yPosition)
        {
            this->xPosition = xPosition;
            this->yPosition = yPosition;
        }

        MoveTo(const YAML::Node& node)
        {
            xPosition = node["xPosition"].as<float>();
            yPosition = node["yPosition"].as<float>();
        }
    
        void run(Actor* actor)
        {
            actor->setPosition(xPosition, yPosition);
        }
    };
    
    // move the actor a given offset amount
    class Move : public Action
    {
    private:
        float xOffset;
        float yOffset;
    public:
        Move(float xOffset, float yOffset)
        {
            this->xOffset = xOffset;
            this->yOffset = yOffset;
        }

        Move(const YAML::Node& node)
        {
            xOffset = node["xOffset"].as<float>();
            yOffset = node["yOffset"].as<float>();
        }
    
        void run(Actor* actor)
        {
            actor->offset(xOffset, yOffset);
        }
    };
    
    // spawn a new actor of the given type
    class Create : public Action
    {
    private:
        const ActorType* actorType;
        State startState;
    public:
        Create(const ActorType* actorType, State& startState)
        {
            this->actorType = actorType;
            this->startState = startState;
        }

        Create(const YAML::Node& node)
        {
            std::string typeName = node["name"].as<std::string>();
            auto mapItem = ActorType::objectMap.find(typeName);
            if (mapItem == ActorType::objectMap.end())
            {
                std::stringstream errorMessage;
                errorMessage << "Actor Type " << typeName << " does not exist.";
                throw ConfigurationError(errorMessage.str());
            }
            this->actorType = mapItem->second;

            this->startState = State(node);
        }
    
        void run(Actor* actor)
        {
            Actor* newActor = new Actor(actorType, startState);
            actor->getRoom()->addActor(newActor);
        }
    };

    // abstract factory (**add new actions here**)
    static Action* createAction(const std::string typeName, const YAML::Node& node)
    {
        if (typeName == "ApplyForceX")
        {
            return new ApplyForceX(node);
        }
        else if (typeName == "MoveToNearest")
        {
            return new MoveToNearest(node);
        }
        else if (typeName == "MoveTowardNearest")
        {
            return new MoveTowardNearest(node);
        }
        else if (typeName == "MoveTo")
        {
            return new MoveTo(node);
        }
        else if (typeName == "Move")
        {
            return new Move(node);
        }
        else if (typeName == "Create")
        {
            return new Create(node);
        }
        else
        {
            std::stringstream errorMessage;
            errorMessage << "Action " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }
    }
}