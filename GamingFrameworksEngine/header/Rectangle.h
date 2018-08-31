#pragma once

// Simple 
class Rectangle
{
public:
    float x;
    float y;
    float width;
    float height;

    Rectangle(float, float, float, float);
    
    // Collision functions
    bool collides(const Rectangle*) const;
    bool willCollideX(const Rectangle*, float) const;
    bool willCollideY(const Rectangle*, float) const;
    bool willCollide(const Rectangle*, float, float) const;
    float getDistanceX(const Rectangle*) const;
    float getDistanceY(const Rectangle*) const;
};