#include <cmath>
#include <utility>
#include "../header/Utils.h"
#include "../header/Actor.h"

Logger engine_util::logger("debug.log");

// determines the distance between two actors
float engine_util::distance(Actor* actor1, Actor* actor2)
{
    State distanceVector = actor1->getNextState() - actor2->getNextState();
    return sqrt(distanceVector.xPosition * distanceVector.xPosition + 
            distanceVector.yPosition * distanceVector.yPosition);
}

// finds the nearest actor of the given type in the list of actors to the source actor
// uses a simple O(n) search
// returns a pair: first is the nearest actor pointer, second is the distance
std::pair<Actor*, float> engine_util::findNearest(Actor* source, 
        ActorType* targetType, const std::list<Actor*>& actors)
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

        if (actor->getType() == targetType)
        {
            float distance = engine_util::distance(actor, source);
            if (nearest == NULL || distance < nearestDistance)
            {
                nearest = actor;
                nearestDistance = distance;
            }
        }
    }
    return std::make_pair(nearest, nearestDistance);
}