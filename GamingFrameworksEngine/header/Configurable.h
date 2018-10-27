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
    std::map<std::string, T*> objectMap;

    std::stringstream directoryPath;
    directoryPath << projectDir << "/" << T::DIR_NAME;

    DIR* dir = opendir(directoryPath.str().c_str());
    // if that directory does not exist, create it
    if (dir == NULL && errno == ENOENT)
    {
        CreateDirectoryA(directoryPath.str().c_str(), NULL);
        dir = opendir(directoryPath.str().c_str());
    } 
    if (dir != NULL) 
    {
        engine_util::logger << "searching directory: " << directoryPath.str() << std::endl;
        // iterate all files in the given directory
        struct dirent* file;
        while ((file = readdir(dir)) != NULL)
        {
            // only consider files ending in .yml
            char* loc = strstr(file->d_name, ".yml");
            if (loc != NULL)
            {
                engine_util::logger << "reading file: " << file->d_name << std::endl;

                // create new object
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

                engine_util::logger << "obj loaded: " << *object << std::endl;
            }
        }
        closedir(dir);
    }
    else 
    {
        // could not open directory
        std::stringstream errorMessage;
        errorMessage << "Directory " << directoryPath.str() << " could not be opened.";
        throw ConfigurationError(errorMessage.str());
    }
    
    for (const auto& obj : objectMap)
    {
        T::objectMap[obj.first] = obj.second;
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
        emitter << YAML::BeginDoc;
        emitter << YAML::BeginMap << *(pair.second) << YAML::EndMap;
        emitter << YAML::EndDoc;

        // write YAML to file
        std::ofstream fout(directoryPath.str() + "/" + pair.first + ".yml");
        fout << emitter.c_str();
        fout.close();
    }
}