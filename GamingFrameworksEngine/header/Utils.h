#pragma once

#include "Actor.h"
#include "Logger.h"

namespace engine_util
{
    // Implements the signum function.
    // params:
    //   val - the value to check sign of
    // returns: -1 if val is negative, 0 if val is 0, 1 if val is positive
    template <typename T> int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    float distance(Actor* actor1, Actor* actor2);

    std::pair<Actor*, float> findNearest(Actor* source, const std::list<Actor*>& actors);

    extern Logger logger;
}