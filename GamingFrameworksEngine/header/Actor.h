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
    
	// Getters/Setters
	std::string getName() const;
	int getId() const;

	void setXSpeed(float speed);
	void setYSpeed(float speed);

    // Override to perform custom object code every game loop
    virtual void step() = 0;

    // Implements collision and motion of the object
    void move(std::list<Actor*>&);
	void interpolateState(float);

    // Draws the object to the screen
	virtual void draw(sf::RenderWindow* window) = 0;
    
    // Collision functions
    bool isCollidable() const;
    bool willCollideX(const Actor* actor) const;
    bool willCollideY(const Actor* actor) const;
    bool willCollide(const Actor* actor) const;
    float getHitboxDistanceX(const Actor* other) const;
    float getHitboxDistanceY(const Actor* other) const;
    // Override to perform custom object code on collision with another actor
    virtual void onCollision(const Actor* other) = 0;
    
    // Getters
    Sprite* getSprite() const;
    Material* getMaterial() const;

    // Setters
    void setXPosition(float);
    void setYPosition(float);
};