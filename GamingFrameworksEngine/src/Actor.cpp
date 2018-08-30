#include "../header/Actor.h"

Actor::Actor()
{

}

Actor::~Actor()
{
}

void Actor::move(double deltaTime, std::list<Actor*>& actors)
{
    xSpeed += xAcceleration;
    ySpeed += yAcceleration;

    for (const Actor* other : actors)
    {
        if (willCollideX(other))
        {
            xSpeed = getHitboxDistanceX(other);
        }
        if (willCollideY(other))
        {
            xSpeed = getHitboxDistanceY(other);
        }
    }

    xPosition += xSpeed;
    yPosition += ySpeed;
}

void Actor::draw(double deltaTime)
{

}


bool Actor::isCollidable() const
{
    return sprite != NULL && sprite->getHitbox() != NULL;
}

bool Actor::willCollideX(const Actor* other) const
{
    if (!isCollidable() || !other->isCollidable())
    {
        return false;
    }
    
    return sprite->getHitbox()->willCollideX(other->getSprite()->getHitbox(), xSpeed);
}

bool Actor::willCollideY(const Actor* other) const
{
    if (!isCollidable() || !other->isCollidable())
    {
        return false;
    }
    
    return sprite->getHitbox()->willCollideY(other->getSprite()->getHitbox(), ySpeed);
}

float Actor::getHitboxDistanceX(const Actor* other) const
{
    if (!isCollidable() || !other->isCollidable())
    {
        return -1.0f;
    }
    
    return sprite->getHitbox()->getDistanceX(other->getSprite()->getHitbox());
}

float Actor::getHitboxDistanceY(const Actor* other) const
{
    if (!isCollidable() || !other->isCollidable())
    {
        return -1.0f;
    }
    
    return sprite->getHitbox()->getDistanceY(other->getSprite()->getHitbox());
}


Sprite* Actor::getSprite() const
{
    return sprite;
}