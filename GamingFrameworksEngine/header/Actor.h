#pragma once

#include <list>
#include <map>
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

    Material* material;

    // drawing
    //Sprite* sprite;
    Rectangle* hitbox;
	Shape* shape;
    float imageSpeed;
    int imageFrame;
    int imageNum;
    float xSpriteOffset;
    float ySpriteOffset;
    float imageAngle;

    // triggers/actions
    map<Trigger*, list<Action*>> actionMap;

public:
    Actor(State startState);
    ~Actor();
    
    // Override to perform custom object code every game loop
    virtual void step() {};

    // Implements collision and motion of the object
    void move(const std::list<Actor*>&);
	void interpolateState(float);

    // Draws the object to the screen
	virtual void draw(sf::RenderWindow*, sf::View*);

    // Performs actions given a trigger
    void fireTrigger(Trigger*);
    
    // Collision functions
    bool isCollidable() const;
    bool willCollideX(const Actor&) const;
    bool willCollideY(const Actor&) const;
    bool willCollide(const Actor&) const;
    float getHitboxDistanceX(const Actor&) const;
    float getHitboxDistanceY(const Actor&) const;
    // Override to perform custom object code on collision with another actor
    virtual void onCollision(Actor*) {};
    
    // Getters
	std::string getName() const;
	int getId() const;
    Rectangle* getHitbox() const;
    Material* getMaterial() const;
    
	// Setters
	void setXSpeed(float);
	void setYSpeed(float);
	void setXAcceleration(float);
	void setYAcceleration(float);
};