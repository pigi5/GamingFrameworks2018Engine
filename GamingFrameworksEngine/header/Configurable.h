#pragma once

#include <string>
#include <sstream>
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "yaml-cpp/yaml.h"

template <typename T>
static void loadAll(std::string directoryName, bool shallow = false)
{
    std::map<const std::string, T*> objectMap;

    DIR *dir;
    struct dirent *file;
    char* loc;
    if ((dir = opendir(directoryName.c_str())) != NULL) 
    {
        // iterate all files in the given directory
        while ((file = readdir(dir)) != NULL)
        {
            if ((loc = strstr(file->d_name, ".yml")) != NULL)
            {
                // create new actor type
                std::stringstream relativePath;
                relativePath << directoryName << "/" << file->d_name;
                YAML::Node config = YAML::LoadFile(relativePath.str());
                T* object = new T(config, shallow);
                // add object to map, keyed by name
                if (!objectMap.emplace(object->name, object).second)
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

    // via https://stackoverflow.com/questions/3639741/merge-two-stl-maps
    objectMap.insert(T::objectMap.begin(), T::objectMap.end());
    std::swap(objectMap, T::objectMap);
    
    /*
    std::cout << "Loaded:" << std::endl;
    for (const auto& pair : T::objectMap)
    {
        std::cout << pair.first << ": " << *pair.second << std::endl;
    }
    */
}

// must be called to avoid memory leak of actor type pointers
template <typename T>
static void unloadAll()
{
    for (const auto& pair : T::objectMap)
    {
        delete pair.second;
    }
    T::objectMap.clear();
}