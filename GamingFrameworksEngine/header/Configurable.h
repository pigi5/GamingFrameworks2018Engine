#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include "../header/ConfigurationError.h"
#include "../header/dirent.h"
#include "yaml-cpp/yaml.h"

template <typename T>
static void loadAll(std::string projectDir, bool shallow = false)
{
    std::map<const std::string, T*> objectMap;

    std::stringstream directoryPath;
    directoryPath << projectDir << "/" << T::DIR_NAME;

    DIR *dir;
    struct dirent *file;
    char* loc;
    if ((dir = opendir(directoryPath.str().c_str())) != NULL) 
    {
        // iterate all files in the given directory
        while ((file = readdir(dir)) != NULL)
        {
            if ((loc = strstr(file->d_name, ".yml")) != NULL)
            {
                // create new actor type
                std::stringstream relativePath;
                relativePath << directoryPath.str() << "/" << file->d_name;
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
        errorMessage << "Directory " << directoryPath.str() << " could not be opened.";
        throw ConfigurationError(errorMessage.str());
    }

    // via https://stackoverflow.com/questions/3639741/merge-two-stl-maps
    objectMap.insert(T::objectMap.begin(), T::objectMap.end());
    std::swap(objectMap, T::objectMap);
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

template <typename T>
static void saveAll(std::string projectDir)
{
    std::stringstream directoryPath;
    directoryPath << projectDir << "/" << T::DIR_NAME;
 
    // iterate all objects of type T in memory
    for (const auto& pair : T::objectMap)
    {
        // write object to YAML emitter
        YAML::Emitter emitter;
        emitter << *(pair.second);

        std::stringstream relativePath;
        relativePath << directoryPath.str() << "/" << pair.first;

        // write YAML to file
        std::ofstream fout(relativePath.str());
        fout << emitter.c_str();
        fout.close();
    }
}