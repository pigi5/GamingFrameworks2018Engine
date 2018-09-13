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
    
        void run(Actor* actor)
        {
            actor->setXAcceleration(acceleration);
        }
    };
}