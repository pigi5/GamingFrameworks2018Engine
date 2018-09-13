#pragma once

// Macro for cleaner trigger definitions
// Note that name is NOT in quotes
#define CREATE_TRIGGER(name, type)          \
class name : public TriggerType<type>       \
{                                           \
public:                                     \
    name(type* data) : TriggerType(data){}; \
};                                          \

#include "Trigger.h"
#include "Actor.h"

// All trigger presets are defined here
namespace trigger_preset
{
    CREATE_TRIGGER(Collision, Actor);
}

#undef create_trigger