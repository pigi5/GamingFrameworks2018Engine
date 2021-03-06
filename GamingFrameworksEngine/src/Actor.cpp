#include <algorithm>
#include <iostream>
#include "../header/Actor.h"
#include "../header/Constants.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"
#include "../header/Utils.h"

Actor::Actor(Room* room, const ActorType* type, State& startState)
{
    static int _id = 0;
    id = _id++;
    this->room = room;
    this->type = type;
    xScale = type->xScale;
    yScale = type->yScale;
    sprite = type->sprite;

    if (xScale != 0 && yScale != 0)
    {
        float width = sprite->getRecommendedWidth() * xScale;
        float height = sprite->getRecommendedHeight() * yScale;

        if (type->collidable)
        {
            this->hitbox = new Hitbox(startState.xPosition, startState.yPosition, width, height);
        }
    }
    friction = type->friction;
    maxXSpeed = type->maxXSpeed;
    maxYSpeed = type->maxYSpeed;
	imageFrame = 0.0f;
    imageSpeed = type->imageSpeed;
    imageAngle = 0.0f;
    depth = type->depth;
    gravitous = type->gravitous;
    // Set the default yAcceleration to gravity
    if (gravitous)
    {
        yAcceleration = engine_constant::GRAVITY;
    }
    this->startState = startState;
    previousState = startState;
    nextState = startState;
    // set default attribute values
    for (auto pair : type->attributes)
    {
        attributes[pair.first] = pair.second;
    }
    
    // fire create trigger
    ActorTypeWrapper wrapper(type);
    trigger_preset::Create trigger(&wrapper);
    fireTrigger(&trigger);
}

Actor::~Actor()
{
    if (hitbox != NULL) 
    {
        delete hitbox;
    }
}

YAML::Emitter& operator<<(YAML::Emitter& out, const Actor& obj)
{
    out << YAML::Key << "type" << YAML::Value << obj.type->name;
    out << YAML::Key << "startX" << YAML::Value << obj.startState.xPosition;
    out << YAML::Key << "startY" << YAML::Value << obj.startState.yPosition;
    return out;
}

void Actor::step()
{
    // decrement counters
    for (auto& pair : timers)
    {
        pair.second -= engine_constant::PHYSICS_DELTA_TIME;
        if (pair.second <= 0)
        {
            Index wrapper(pair.first);
            trigger_preset::Timer trigger(&wrapper);
            fireTrigger(&trigger);
        }
    }

    imageFrame += imageSpeed;
    // fire step trigger
    ActorTypeWrapper wrapper(type);
    trigger_preset::Step trigger(&wrapper);
    fireTrigger(&trigger);
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
                // Don't let speed change signs
                ySpeed = engine_util::sign(ySpeed) * max(0.0f, abs(ySpeed) - other->friction);
            } 
            else if (collidesY)
            {
                // Set speed such that this object will go up to the colliding object but not past
                ySpeed = engine_util::sign(ySpeed) * getHitboxDistanceY(*other);
                // Friction that goes against direction of motion in opposite axis
                // Don't let speed change signs
                xSpeed = engine_util::sign(xSpeed) * max(0.0f, abs(xSpeed) - other->friction);
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

    // reset acceleration
    if (gravitous)
    {
        yAcceleration = engine_constant::GRAVITY;
    }
    else
    {
        yAcceleration = 0;
    }
    xAcceleration = 0;
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
}

void Actor::offset(float xOffset, float yOffset)
{
    setPosition(nextState.xPosition + xOffset, nextState.yPosition + yOffset);
}

// Draws the shape, if there is one set, to the screen
// params:
//   window: the window to draw to
//   view: the view of the world
void Actor::draw(sf::RenderWindow* window, sf::View* view)
{
    ActorTypeWrapper wrapper(type);
    trigger_preset::Draw trigger(&wrapper);
    fireTrigger(&trigger);

    if (sprite != NULL)
    {
	    sprite->draw(window, imageFrame, currentState.xPosition, currentState.yPosition, xScale, yScale, imageAngle);
    }
}

void Actor::fireTrigger(Trigger* trigger)
{
    if (room->getActorDeleteQueue().find(this) == room->getActorDeleteQueue().end() && 
            room->getOverlayDeleteQueue().find((Overlay*)this) == room->getOverlayDeleteQueue().end())
    {
        try
        {
            engine_util::logger << *this << ": trigger - " << *trigger << " - " << std::to_string(trigger->hashCode()) << std::endl;
            // get action list for trigger if one exists
            auto actions = type->actionMap.at(trigger);
            for (Action* action : actions)
            {
                engine_util::logger << *this << ": action - " << *action << std::endl;
                action->run(this);
                // can't continue if deleted
                if (dynamic_cast<action_preset::Destroy*>(action) && action->checkConditionals(this))
                {
                    return;
                }
            }
        } 
        catch (const std::out_of_range& e) {}
    }
}

void Actor::onCollision(Actor* other)
{
    ActorTypeWrapper wrapper(other->type);
    trigger_preset::Collision trigger(&wrapper);
    fireTrigger(&trigger);
}

// Tests if the actor has a hitbox.
// returns: if the object has a hitbox
bool Actor::isCollidable() const
{
    return sprite != NULL && hitbox != NULL;
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

const ActorType* Actor::getType() const
{
    return type;
}

int Actor::getId() const
{
	return id;
}

State Actor::getNextState() const
{
    return nextState;
}

State Actor::getStartState() const
{
	return startState;
}

State Actor::getCurrentState() const
{
    return currentState;
}

Hitbox* Actor::getHitbox() const
{
    return hitbox;
}

Room* Actor::getRoom() const
{
    return room;
}

// checks the local actor type attribute list for the given attribute,
// then checks the global attributes list. Throws an error if the given
// key is not in either list
int Actor::getAttribute(std::string key) const
{
    auto val = attributes.find(key);
    if (val == attributes.end())
    {
        val = room->getEngine()->globalAttributes.find(key);
        if (val == attributes.end())
        {
            std::stringstream errorMessage;
            errorMessage << "Attribute " << key << " does not exist.";
            throw ConfigurationError(errorMessage.str());
        }
    }
    return val->second;
}

void Actor::setStartState(State start)
{
	this->startState = start;
}

void Actor::setPosition(float xPosition, float yPosition)
{
    nextState.xPosition = xPosition;
    nextState.yPosition = yPosition;

    // Move hitbox with actor
    if (isCollidable())
    {
        hitbox->x = nextState.xPosition + xSpriteOffset;
        hitbox->y = nextState.yPosition + ySpriteOffset;
    }
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

void Actor::setCollidable(bool collidable)
{
    if (this->hitbox != NULL)
    {
        delete hitbox;
    }
    if (collidable)
    {
        float width = sprite->getRecommendedWidth() * xScale;
        float height = sprite->getRecommendedHeight() * yScale;
        this->hitbox = new Hitbox(nextState.xPosition + xSpriteOffset, 
            nextState.yPosition + ySpriteOffset, width, height);
    }
    else
    {
        this->hitbox = NULL;
    }
}
void Actor::setGravitous(bool gravitous)
{
    this->gravitous = gravitous;
}
void Actor::setAnimationSpeed(float imageSpeed)
{
    this->imageSpeed = imageSpeed;
}
void Actor::setAnimationFrame(float imageFrame)
{
    this->imageFrame = imageFrame;
}
void Actor::setDepth(float depth)
{
    this->depth = depth;
}
void Actor::setXScale(float xScale)
{
    this->xScale = xScale;
    // reset hitbox
    setCollidable(this->hitbox != NULL);
}
void Actor::setYScale(float yScale)
{
    this->yScale = yScale;
    // reset hitbox
    setCollidable(this->hitbox != NULL);
}
void Actor::setFriction(float friction)
{
    this->friction = friction;
}
void Actor::setMaxXSpeed(float maxXSpeed)
{
    this->maxXSpeed = maxXSpeed;
}
void Actor::setMaxYSpeed(float maxYSpeed)
{
    this->maxYSpeed = maxYSpeed;
}
void Actor::setSprite(Sprite* sprite)
{
    this->sprite = sprite;
    // reset hitbox
    setCollidable(this->hitbox != NULL);
}

void Actor::startTimer(int index, float time)
{
    timers[index] = time;
}

void Actor::setAttribute(std::string key, int value)
{
    try
    {
        attributes.at(key) = value;
    } 
    catch (const std::out_of_range& e)
    {
        try
        {
            room->getEngine()->globalAttributes.at(key) = value;
        }
        catch (const std::out_of_range& e)
        {
            throw ConfigurationError("Attribute named " + key + " no longer exists.");
        }
    }
}

void Actor::changeAttribute(std::string key, int value)
{
    try
    {
        attributes.at(key) += value;
    } 
    catch (const std::out_of_range& e)
    {
        try
        {
            room->getEngine()->globalAttributes.at(key) += value;
        }
        catch (const std::out_of_range& e)
        {
            throw ConfigurationError("Attribute named " + key + " no longer exists.");
        }
    }
}

Logger& operator<<(Logger& logger, const Actor& obj)
{
    logger << obj.type->name << " " << std::to_string(obj.id);
    return logger;
}

// Mouse Functions

bool Actor::checkPressOn(int clickX, int clickY)
{
	bool on = false;
	if (isCollidable() && hitbox->isClickedOn(clickX, clickY)) 
	{
		MouseInputType m;
		m.state = MouseState::PRESS_ON;
		trigger_preset::MouseInput trigger(&m);
		this->fireTrigger(&trigger);
		on = true;
	}
	return on;
}

bool Actor::checkReleaseOn(int clickX, int clickY)
{
	bool on = false;
	if (isCollidable() && hitbox->isClickedOn(clickX, clickY))
	{
		MouseInputType m;
		m.state = MouseState::RELEASE_ON;
		trigger_preset::MouseInput trigger(&m);
		this->fireTrigger(&trigger);
		on = true;
	}
	return on;
}

// reverse ordering for containers
bool Actor::compareDepth(const Actor* obj1, const Actor* obj2)
{
    return obj1->depth > obj2->depth;
}