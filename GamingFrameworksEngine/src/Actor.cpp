#include <algorithm>
#include "../header/Actor.h"
#include "../header/Constants.h"

Actor::Actor()
{
    yAcceleration = GRAVITY;
}

Actor::~Actor()
{
}

void Actor::move(double deltaTime, std::list<Actor*>& actors)
{
    // Increment speeds
    xSpeed += xAcceleration;
    ySpeed += yAcceleration;
    
    // Collision detection logic (look-ahead style)
    bool collides;
    for (Actor* other : actors)
    {
        // Assume no collision
        collides = false;
        // Test collision on the x axis
        if (willCollideX(other))
        {
            // Set speed such that this object will go up to the colliding object but not past
            xSpeed = getHitboxDistanceX(other);
            collides = true;
        }
        // Test collision on the y axis
        if (willCollideY(other))
        {
            // Set speed such that this object will go up to the colliding object but not past
            ySpeed = getHitboxDistanceY(other);
            collides = true;
        }
        // Possible that the object will still collide if it moves in both directions (i.e. on a corner)
        if (!collides) 
        {
            if (willCollide(other))
            {
                // Prefer the faster direction to continue in
                if (ySpeed > xSpeed)
                {
                    xSpeed = getHitboxDistanceX(other);
                }
                else
                {
                    ySpeed = getHitboxDistanceY(other);
                }
                collides = true;
            }
        }
        // Fire collision event
        if (collides)
        {
            onCollision(other);
            other->onCollision(this);
        }
    }
    
    // Increment positions
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

bool Actor::willCollide(const Actor* other) const
{
    if (!isCollidable() || !other->isCollidable())
    {
        return false;
    }
    
    return sprite->getHitbox()->willCollide(other->getSprite()->getHitbox(), xSpeed, ySpeed);
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