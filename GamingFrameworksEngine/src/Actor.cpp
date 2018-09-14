#include <algorithm>
#include <iostream>
#include "../header/Actor.h"
#include "../header/Constants.h"
#include "../header/Utils.h"

Actor::Actor(State startState)
{
    static int _id = 0;
    id = _id++;
    // Set the default yAcceleration to gravity
    yAcceleration = engine_constant::GRAVITY;
	maxXSpeed = -1;
	maxYSpeed = -1;
    previousState = startState;
    nextState = startState;
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
    if (maxXSpeed >= 0 && abs(xSpeed) >= maxXSpeed)
    {
        xSpeed = engine_util::sign(xSpeed) * maxXSpeed;
    }
    ySpeed += yAcceleration;
    if (maxYSpeed >= 0 && abs(ySpeed) >= maxYSpeed)
    {
        ySpeed = engine_util::sign(ySpeed) * maxYSpeed;
    }
    
    // Collision detection logic (look-ahead style)
    bool collidesX;
    bool collidesY;
    for (Actor* other : actors)
    {
        // Don't perform collisions with self
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
                    ySpeed = engine_util::sign(ySpeed) * std::max(0.0f, abs(ySpeed) - other->getMaterial()->getFriction());
                }
            } 
            else if (collidesY)
            {
                // Set speed such that this object will go up to the colliding object but not past
                ySpeed = engine_util::sign(ySpeed) * getHitboxDistanceY(*other);
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
	previousState = nextState;
    nextState.xPosition += xSpeed;
    nextState.yPosition += ySpeed;
    // Move hitbox with actor
    if (isCollidable())
    {
        hitbox->x = nextState.xPosition + xSpriteOffset;
        hitbox->y = nextState.yPosition + ySpriteOffset;
    }
}

// Does a linear interpolation of the actors current state and its previous
// state based on the given progress value
// params:
//   blend - the amount to interpolate; must be between 0 and 1, where
//              1 indicates to use the previous state and 0 indicates to
//              use the next state
void Actor::interpolateState(float blend)
{
	currentState = nextState - nextState * blend + previousState * blend;
    // Move shape with actor
    if (shape != NULL)
    {
	    shape->setPosition(currentState.xPosition, currentState.yPosition);
    }
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

void Actor::fireTrigger(Trigger* trigger)
{
    for (Action* action : actionMap[trigger])
    {
        action->run(this);
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

void Actor::setXAcceleration(float xAcceleration)
{
	this->xAcceleration = xAcceleration;
}

void Actor::setYAcceleration(float yAcceleration) 
{
	this->yAcceleration = yAcceleration;
}

void Actor::setXSpeed(float xSpeed)
{
	this->xSpeed = xSpeed;
}

void Actor::setYSpeed(float ySpeed) 
{
	this->ySpeed = ySpeed;
}


bool Actor::operator<(const Actor& other) const
{
    return id < other.id;
}