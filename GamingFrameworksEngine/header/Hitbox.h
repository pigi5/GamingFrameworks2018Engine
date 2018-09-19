#pragma once

// Simple 
class Hitbox
{
public:
    float x;
    float y;
    float width;
    float height;

    Hitbox(float, float, float, float);
    
    // Collision functions
    bool collides(const Hitbox*) const;
    bool willCollideX(const Hitbox*, float) const;
    bool willCollideY(const Hitbox*, float) const;
    bool willCollide(const Hitbox*, float, float) const;
    float getDistanceX(const Hitbox*) const;
    float getDistanceY(const Hitbox*) const;
};