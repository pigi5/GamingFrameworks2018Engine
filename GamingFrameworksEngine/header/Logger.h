#pragma once

#include <fstream>
#include <string>

typedef std::ostream& (*ostream_manipulator)(std::ostream&);

class Logger
{
private:
    std::ofstream stream;
    std::string filename;

public:
    Logger(){};
    Logger(std::string filename);

    ~Logger();
    
    std::string now();

    friend Logger& operator<<(Logger&, const std::string&);
    friend Logger& operator<<(Logger& os, ostream_manipulator pf);
};