#include <cmath>
#include <utility>
#include "../header/Utils.h"

// determines the distance between two actors
float engine_util::distance(Actor* actor1, Actor* actor2)
{
    State distanceVector = actor1->getState() - actor2->getState();
    return sqrt(distanceVector.xPosition * distanceVector.xPosition + 
            distanceVector.yPosition * distanceVector.yPosition);
}

// finds the nearest actor in the list of actors to the source actor
// uses a simple O(n) search
// returns a pair: first is the nearest actor pointer, second is the distance
std::pair<Actor*, float> engine_util::findNearest(Actor* source, const std::list<Actor*>& actors)
{
    Actor* nearest = NULL;
    float nearestDistance;
    for (Actor* actor : actors)
    {
        // don't compare to self
        if (actor == source)
        {
            continue;
        }

        float distance = engine_util::distance(actor, source);
        if (nearest == NULL || distance < nearestDistance)
        {
            nearest = actor;
            nearestDistance = distance;
        }
    }
    return std::make_pair(nearest, nearestDistance);
}