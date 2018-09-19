#pragma once

#include <list>
#include <map>
#include "ActorType.h"
#include "Sprite.h"
#include "Shape.h"
#include "Material.h"
#include "Trigger.h"
#include "Action.h"

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
};


// A "world object" that can move and be drawn to the screen. Its
// position is in world or "room" coordinates.
class Actor
{
protected:
	// identifiers
	int id;
	std::string objName;

    // physics
    float xAcceleration;
    float yAcceleration;
    float xSpeed;
    float ySpeed;
    State currentState;
    State nextState;
    State previousState;
	float maxXSpeed;
	float maxYSpeed;

    // drawing
    //Sprite* sprite;
    Hitbox* hitbox;
	Shape* shape;
    float imageSpeed;
    int imageFrame;
    int imageNum;
    float xSpriteOffset;
    float ySpriteOffset;
    float imageAngle;
    
    // Type data
    ActorType* type;

public:
    Actor(ActorType* type);
    ~Actor();
    
    void step();

    // Implements collision and motion of the object
    void move(const std::list<Actor*>&);
	void interpolateState(float);

    void offset(float, float);

    // Draws the object to the screen
	void draw(sf::RenderWindow*, sf::View*);

    // Performs actions given a trigger
    void fireTrigger(const Trigger&);
    
    // Collision functions
    void onCollision(Actor*);
    bool isCollidable() const;
    bool willCollideX(const Actor&) const;
    bool willCollideY(const Actor&) const;
    bool willCollide(const Actor&) const;
    float getHitboxDistanceX(const Actor&) const;
    float getHitboxDistanceY(const Actor&) const;
    
    // Getters
	std::string getName() const;
	ActorType* getType() const;
	int getId() const;
    State getState() const;
    Hitbox* getHitbox() const;
    
	// Setters
    void setPosition(float, float);
	void setXSpeed(float);
	void setYSpeed(float);
	void setXAcceleration(float);
	void setYAcceleration(float);

    // Comparison
    bool operator<(const Actor&) const;
};