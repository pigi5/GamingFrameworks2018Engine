#pragma once

class Material
{
public:
    float friction;

    Material(float);
    
    float getFriction() const;
};