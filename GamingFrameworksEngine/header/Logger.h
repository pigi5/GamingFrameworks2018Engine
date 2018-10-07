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
    friend Logger& operator<<(Logger&, const long);
    friend Logger& operator<<(Logger&, const int);
    friend Logger& operator<<(Logger&, const short);
    friend Logger& operator<<(Logger&, const double);
    friend Logger& operator<<(Logger&, const float);
    //friend Logger& operator<<(Logger&, const bool);
    friend Logger& operator<<(Logger& os, ostream_manipulator pf);
};