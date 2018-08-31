#include "../header/Material.h"

Material::Material(float friction)
{
    this->friction = friction;
}
    
float Material::getFriction() const
{
    return friction;
}