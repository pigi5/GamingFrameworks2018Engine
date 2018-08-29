#pragma once

class Moveable
{
private:
    float xGravity;
    float yGravity;
    float xSpeed;
    float ySpeed;
    float xPosition;
    float yPosition;

public:
    Moveable();
    ~Moveable();
    void move();
};

