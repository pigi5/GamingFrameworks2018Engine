#pragma once

#include <stdexcept>
#include <iostream>

class ConfigurationError : public std::runtime_error
{
public:
    ConfigurationError(std::string message) : std::runtime_error(message) 
    {
        std::cerr << message << std::endl;
    };
};