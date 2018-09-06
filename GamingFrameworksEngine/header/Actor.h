#pragma once

#include <list>
#include "Sprite.h"
#include "Shape.h"
#include "Material.h"

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
    float xPosition;
    float yPosition;
	float xPositionPrevious;
	float yPositionPrevious;

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

public:
    Actor();
    ~Actor();
    
    // Override to perform custom object code every game loop
    virtual void step() {};

    // Implements collision and motion of the object
    void move(const std::list<Actor*>&);
	void interpolateState(float);

    // Draws the object to the screen
	virtual void draw(sf::RenderWindow*, sf::View*);
    
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

    // Setters
    void setXPosition(float);
    void setYPosition(float);
};