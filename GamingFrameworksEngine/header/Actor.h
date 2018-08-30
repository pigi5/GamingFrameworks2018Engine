#pragma once

#include <list>
#include "Sprite.h"

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

    // drawing
    Sprite* sprite;
    float imageSpeed;
    int imageFrame;
    int imageNum;
    float xImageOffset;
    float yImageOffset;
    float imageAngle;

public:
    Actor();
    ~Actor();
    
    virtual void step(double) = 0;

    void move(double, std::list<Actor*>&);

    void draw(double);
    
    bool isCollidable() const;
    bool willCollideX(const Actor* actor) const;
    bool willCollideY(const Actor* actor) const;
    float getHitboxDistanceX(const Actor* other) const;
    float getHitboxDistanceY(const Actor* other) const;

    Sprite* getSprite() const;
};