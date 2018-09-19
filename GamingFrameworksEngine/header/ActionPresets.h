#pragma once

#include "Action.h"
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
    
        void run(Actor* actor) const
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
    
        void run(Actor* actor) const
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
    
        void run(Actor* actor) const
        {
            actor->offset(xOffset, yOffset);
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
        else
        {
            std::stringstream errorMessage;
            errorMessage << "Action " << typeName << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }
    }
}