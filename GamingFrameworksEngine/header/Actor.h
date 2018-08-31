#pragma once

#include <list>
#include "Sprite.h"
#include "Material.h"

// A "world object" that can move and be drawn to the screen.
class Actor
{
private:
    // physics
    float xAcceleration;
    float yAcceleration;
    float xSpeed;
    float ySpeed;
    float xPosition;
    float yPosition;

    Material* material;

    // drawing
    Sprite* sprite;
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
    virtual void step(double) = 0;

    // Implements collision and motion of the object
    void move(double, std::list<Actor*>&);

    // Draws the object to the screen
    void draw(double);
    
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