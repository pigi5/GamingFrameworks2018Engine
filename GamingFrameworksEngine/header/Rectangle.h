#pragma once

class Rectangle
{
public:
    float x;
    float y;
    float width;
    float height;

    Rectangle(float, float, float, float);
    
    bool collides(const Rectangle*) const;
    bool willCollideX(const Rectangle*, float) const;
    bool willCollideY(const Rectangle*, float) const;
    float getDistanceX(const Rectangle*) const;
    float getDistanceY(const Rectangle*) const;
};