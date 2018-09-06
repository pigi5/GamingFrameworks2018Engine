#include <algorithm>
#include <iostream>
#include "../header/Actor.h"
#include "../header/Constants.h"
#include "../header/Utils.h"

Actor::Actor()
{
    // Set the default yAcceleration to gravity
    yAcceleration = engine_constant::GRAVITY;
}

Actor::~Actor()
{
    if (shape != NULL) 
    {
        delete shape;
    }
    if (shape != NULL) 
    {
        delete hitbox;
    }
    if (shape != NULL) 
    {
        delete material;
    }
}

// Moves the object based on its physics attributes and performs collision detection
// params:
//   deltaTime - the time in seconds between this frame and the previous frame
//   actors - other actors in the room
void Actor::move(const std::list<Actor*>& actors)
{
    // Increment speeds
    xSpeed += xAcceleration;
    ySpeed += yAcceleration;
    
    // Collision detection logic (look-ahead style)
    bool collidesX;
    bool collidesY;
    for (Actor* other : actors)
    {
        if (other == this)
        {
            continue;
        }

        // Test collision on the x axis
        collidesX = willCollideX(*other);
        // Test collision on the y axis
        collidesY = willCollideY(*other);

        // Possible that the object will still collide if it moves in both directions (i.e. on a corner)
        if (!collidesX && !collidesY) 
        {
            if (willCollide(*other))
            {
                // Prefer the faster direction to continue in
                collidesX = ySpeed > xSpeed;
                collidesY = !collidesX;
            }
        }

        // Perform collision actions
        if (collidesX || collidesY)
        {
            // The "else" is an optimization based on the fact that we cannot collide in both directions
            // with a single object with a rectangular hitbox 
            if (collidesX)
            {
                // Set speed such that this object will go up to the colliding object but not past
                xSpeed = engine_util::sign(xSpeed) * getHitboxDistanceX(*other);
                // Friction that goes against direction of motion in opposite axis
                Material* otherMaterial = other->getMaterial();
                if (otherMaterial != NULL)
                {
                    // Don't let speed change signs
                    ySpeed = engine_util::sign(ySpeed) * std::min(0.0f, abs(ySpeed) - other->getMaterial()->getFriction());
                }
            } 
            else if (collidesY)
            {
                // Set speed such that this object will go up to the colliding object but not past
                ySpeed = 0;
                //ySpeed = engine_util::sign(ySpeed) * getHitboxDistanceY(other);
                // Friction that goes against direction of motion in opposite axis
                Material* otherMaterial = other->getMaterial();
                if (otherMaterial != NULL)
                {
                    // Don't let speed change signs
                    xSpeed = engine_util::sign(xSpeed) * std::max(0.0f, abs(xSpeed) - other->getMaterial()->getFriction());
                }
            }
            // Fire collision event
            onCollision(other);
            other->onCollision(this);
        }
    }
    
    // Increment positions
	xPositionPrevious = xPosition;
	yPositionPrevious = yPosition;
    setXPosition(xPosition + xSpeed);
    setYPosition(yPosition + ySpeed);
}

// Does a linear interpolation of the actors current state and its previous
// state based on the given progress value
// params:
//   progress - the amount to interpolate; must be between 0 and 1, where
//              0 indicates to use the previous state and 1 indicates to
//              use the current state
void Actor::interpolateState(float progress)
{
	setXPosition(xPositionPrevious + (xPositionPrevious - xPosition) * progress);
	setYPosition(yPositionPrevious + (yPositionPrevious - yPosition) * progress);
}

// Draws the shape, if there is one set, to the screen
// params:
//   window: the window to draw to
//   view: the view of the world
void Actor::draw(sf::RenderWindow* window, sf::View* view)
{
    if (shape != NULL)
    {
	    shape->draw(window);
    }
}

// Tests if the actor has a hitbox.
// returns: if the object has a hitbox
bool Actor::isCollidable() const
{
    return hitbox != NULL;
}

// Tests if this actor will collide with the given actor after its new position is
// calculated based on its speed.
// params:
//   other - actor to test collision against
// returns: whether this actor will collide with the given actor
bool Actor::willCollide(const Actor& other) const
{
    if (!isCollidable() || !other.isCollidable())
    {
        return false;
    }
    
    return hitbox->willCollide(other.getHitbox(), xSpeed, ySpeed);
}

// Tests if this actor will collide with the given actor in the x-axis after its new 
// position is calculated based on its speed.
// params:
//   other - actor to test collision against
// returns: whether this actor will collide with the given actor
bool Actor::willCollideX(const Actor& other) const
{
    if (!isCollidable() || !other.isCollidable())
    {
        return false;
    }
    
    return hitbox->willCollideX(other.getHitbox(), xSpeed);
}

// Tests if this actor will collide with the given actor in the y-axis after its new 
// position is calculated based on its speed.
// params:
//   other - actor to test collision against
// returns: whether this actor will collide with the given actor
bool Actor::willCollideY(const Actor& other) const
{
    if (!isCollidable() || !other.isCollidable())
    {
        return false;
    }
    
    return hitbox->willCollideY(other.getHitbox(), ySpeed);
}

// Calculate the distance along the x-axis to the given object.
// params:
//   other - actor to test distance against
// returns: x-axis distance to the given object
float Actor::getHitboxDistanceX(const Actor& other) const
{
    if (!isCollidable() || !other.isCollidable())
    {
        return -1.0f;
    }
    
    return hitbox->getDistanceX(other.getHitbox());
}

// Calculate the distance along the y-axis to the given object.
// params:
//   other - actor to test distance against
// returns: y-axis distance to the given object
float Actor::getHitboxDistanceY(const Actor& other) const
{
    if (!isCollidable() || !other.isCollidable())
    {
        return -1.0f;
    }
    
    return hitbox->getDistanceY(other.getHitbox());
}


// Getters/Setters
std::string Actor::getName() const
{
	return this->objName;
}

int Actor::getId() const
{
	return this->id;
}

Rectangle* Actor::getHitbox() const
{
    return hitbox;
}

Material* Actor::getMaterial() const
{
    return material;
}

void Actor::setXSpeed(float xSpeed)
{
	this->xSpeed = xSpeed;
}

void Actor::setYSpeed(float ySpeed) 
{
	this->ySpeed = ySpeed;
}


// Sets the current x-axis position of actor.
// params:
//   xPosition - new x-axis position
void Actor::setXPosition(float xPosition)
{
    this->xPosition = xPosition;
    // Move hitbox with actor
    if (isCollidable())
    {
        hitbox->x = xPosition + xSpriteOffset;
    }
    // Move shape with actor
    if (shape != NULL)
    {
	    shape->setXPosition(xPosition);
    }
}

// Sets the current y-axis position of actor.
// params:
//   xPosition - new y-axis position
void Actor::setYPosition(float yPosition)
{
    this->yPosition = yPosition;
    // Move hitbox with actor
    if (isCollidable())
    {
        hitbox->y = yPosition + ySpriteOffset;
    }
    // Move shape with actor
    if (shape != NULL)
    {
	    shape->setYPosition(yPosition);
    }
}