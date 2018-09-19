#pragma once

// Macro for cleaner trigger definitions
// Note that name is NOT in quotes
// type is the type of the id pointer in TriggerType
#define CREATE_TRIGGER(name, type)          \
class name : public TriggerType<type>       \
{                                           \
public:                                     \
    name(type* id) : TriggerType(id){};     \
};                                          \

#include "Trigger.h"
#include "Actor.h"

// this should probably be moved to an appropriate file (watching circular dependencies)
struct ButtonInputType
{
	short id;
	bool state = false;

	bool operator<(const ButtonInputType& other) const
	{
		return id * 2 + state < other.id * 2 + other.state;
	}
};

// All trigger presets are defined here
namespace trigger_preset
{
    CREATE_TRIGGER(Collision, Actor);
    CREATE_TRIGGER(ButtonInput, ButtonInputType);
    CREATE_TRIGGER(Create, Actor);
    CREATE_TRIGGER(Step, Actor);
    CREATE_TRIGGER(Draw, Actor);
    CREATE_TRIGGER(Destroy, Actor);
}

#undef create_trigger