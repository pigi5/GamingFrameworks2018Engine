#pragma once

#include <string>
#include <sstream>
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "yaml-cpp/yaml.h"

template <typename T>
static void loadAll(std::string directoryName)
{
    DIR *dir;
    struct dirent *file;
    char* loc;
    if ((dir = opendir(directoryName.c_str())) != NULL) 
    {
        /* print all the files and directories within directory */
        while ((file = readdir(dir)) != NULL)
        {
            if ((loc = strstr(file->d_name, ".yaml")) != NULL)
            {
                // create new actor type
                std::stringstream relativePath;
                relativePath << directoryName << "/" << file->d_name;
                YAML::Node config = YAML::LoadFile(relativePath.str());
                T* object = new T(config);
                // add actor type to map, keyed by name
                if (!T::objectMap.emplace(object->name, object).second)
                {
                    // if key already existed, throw error
                    std::stringstream errorMessage;
                    errorMessage << "Object name \"" << object->name;
                    errorMessage << "\" (referenced in " << file->d_name << ") is not unique.";
                    throw ConfigurationError(errorMessage.str());
                }
            }
        }
        closedir(dir);
    } else {
        // could not open directory
        std::stringstream errorMessage;
        errorMessage << "Directory " << directoryName << " could not be opened.";
        throw ConfigurationError(errorMessage.str());
    }
    
    std::cout << "Loaded:" << std::endl;
    for (const auto& pair : T::objectMap)
    {
        std::cout << pair.first << ": " << *pair.second << std::endl;
    }
}

// must be called to avoid memory leak of actor type pointers
template <typename T>
static void unloadAll()
{
    for (const auto& pair : T::objectMap)
    {
        delete pair.second;
    }
}