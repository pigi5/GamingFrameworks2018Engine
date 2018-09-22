#include <list>
#include "../header/OverlayType.h"
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "../header/TriggerPresets.h"
#include "../header/ActionPresets.h"

std::map<const std::string, OverlayType*> OverlayType::objectMap;

bool OverlayType::operator<(const OverlayType& other) const
{
    return name < other.name;
}

std::ostream& operator<<(std::ostream& output, const OverlayType& object)
{
    output << " name: " << object.name;
    if (object.material == NULL)
    {
        output << " material: NULL";
    }
    else
    {
         output << " material: " << object.material;
    }
   
    for (const auto& pair : object.actionMap)
    {
        output << " trigger: ";
        for (const Action* action : pair.second)
        {
            output << " action ";
        }
    }
    return output;
}