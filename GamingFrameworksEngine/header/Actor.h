#pragma once

#include <list>
#include <unordered_map>
#include "ActorType.h"
#include "Sprite.h"
#include "Audio.h"
#include "Shape.h"
#include "Material.h"
#include "Trigger.h"
#include "Action.h"
#include "Hitbox.h"
#include "Logger.h"

class Room;

// The state of the object that needs to be interpolated between frames
struct State
{
    float xPosition;
    float yPosition;

    State operator*(float blend) const
    {
        State newState;
        newState.xPosition = xPosition * blend;
        newState.yPosition = yPosition * blend;
        return newState;
    }

    State operator+(const State& other) const
    {
        State newState;
        newState.xPosition = xPosition + other.xPosition;
        newState.yPosition = yPosition + other.yPosition;
        return newState;
    }

    State operator-(const State& other) const
    {
        State newState;
        newState.xPosition = xPosition - other.xPosition;
        newState.yPosition = yPosition - other.yPosition;
        return newState;
    }

    State& operator=(State other)
    {
        xPosition = other.xPosition;
        yPosition = other.yPosition;
        return *this;
    }

    State() {}

    State(float xPosition, float yPosition)
    {
        this->xPosition = xPosition;
        this->yPosition = yPosition;
    }

    State(const YAML::Node& node)
    {
        xPosition = node["xPosition"].as<float>();
        yPosition = node["yPosition"].as<float>();
    }
    
    friend YAML::Emitter& operator<<(YAML::Emitter& out, const State& obj)
    {
        out << YAML::Key << "xPosition" << YAML::Value << obj.xPosition;
        out << YAML::Key << "yPosition" << YAML::Value << obj.yPosition;
        return out;
    }
    
    const std::string& toString() const
    {
        return "xPosition: " + std::to_string(xPosition) + ", yPosition: " + std::to_string(yPosition);
    }
};


// A "world object" that can move and be drawn to the screen. Its
// position is in world or "room" coordinates.
class Actor
{
protected:
	// identifiers
	int id;

    Room* room;

    // physics
    Hitbox* hitbox;
    float xAcceleration;
    float yAcceleration;
    float xSpeed;
    float ySpeed;
    State startState;
    State currentState;
    State nextState;
    State previousState;

    // drawing
    float imageSpeed;
    float imageFrame;
    float xSpriteOffset;
    float ySpriteOffset;
    float imageAngle;
    
    // Type data
    const ActorType* type;

    std::unordered_map<std::string, int> attributes;

public:
    Actor(Room* room, const ActorType* type, State& startState);
    ~Actor();

    friend YAML::Emitter& operator<<(YAML::Emitter&, const Actor&);
    
    void step();

    // Implements collision and motion of the object
    void move(const std::list<Actor*>&);
	void interpolateState(float);

    void offset(float, float);

    // Draws the object to the screen
	void draw(sf::RenderWindow*, sf::View*);

    // Performs actions given a trigger
    void fireTrigger(Trigger* trigger);
    
    // Collision functions
    void onCollision(Actor*);
    bool isCollidable() const;
    bool willCollideX(const Actor&) const;
    bool willCollideY(const Actor&) const;
    bool willCollide(const Actor&) const;
    float getHitboxDistanceX(const Actor&) const;
    float getHitboxDistanceY(const Actor&) const;
    
    // Getters
	const ActorType* getType() const;
	int getId() const;
    State getState() const;
    Hitbox* getHitbox() const;
    Room* getRoom() const;
    int getAttribute(std::string) const;
    
	// Setters
    void setPosition(float, float);
	void setXSpeed(float);
	void setYSpeed(float);
	void setXAcceleration(float);
	void setYAcceleration(float);
    
    void setAttribute(std::string, int);
    void changeAttribute(std::string, int);

    friend Logger& operator<<(Logger&, const Actor&);
};