#pragma once

#include "Action.h"
#include "Room.h"
#include "Actor.h"
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
    
    /*class MoveTo : public Action
    {
    private:
        Actor* other;
    public:
        MoveTo(Actor* other)
        {
            this->other = other;
        }
    
        void run(Actor* actor) const
        {
            actor->setPosition(other->getState().xPosition, other->getState().yPosition);
        }
    };*/
    
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
            this->actorType = (*mapItem).second;

            this->startState = State(node);
        }
    
        void run(Actor* actor)
        {
            Actor* newActor = new Actor(actorType, startState);
            actor->getRoom()->addActor(newActor);
        }
    };

    // abstract factory
    static Action* createAction(const std::string typeName, const YAML::Node& node)
    {
        if (typeName == "ApplyForceX")
        {
            return new ApplyForceX(node);
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