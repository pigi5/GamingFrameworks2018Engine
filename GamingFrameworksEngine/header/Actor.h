#pragma once
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
    Sprite sprite;
    float imageSpeed;
    int imageFrame;
    int imageNum;
    float xImageOffset;
    float yImageOffset;
    float imageAngle;

public:
    Actor();
    ~Actor();

    void move(double delta_time);

    void draw(double delta_time);
};

