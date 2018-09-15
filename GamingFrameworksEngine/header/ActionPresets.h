#pragma once

#include "Action.h"
#include "Actor.h"

namespace action_preset
{
    class ApplyForceX : Action
    {
    private:
        int acceleration;
    public:
        ApplyForceX(int acceleration)
        {
            this->acceleration = acceleration;
        }
    
        void run(Actor* actor) const
        {
            actor->setXAcceleration(acceleration);
        }
    };
    
    class MoveTo : Action
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
    };
    
    class Move : Action
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
    
        void run(Actor* actor) const
        {
            actor->offset(xOffset, yOffset);
        }
    };
}