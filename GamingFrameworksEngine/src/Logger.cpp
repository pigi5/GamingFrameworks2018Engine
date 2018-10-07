#include "../header/Logger.h"
#include <ctime>
#include <chrono>


Logger::Logger(std::string filename)
{
    this->filename = filename;
    
    stream.open(filename);
    *this << now() << std::endl;
}

Logger::~Logger() 
{
    stream.close();
}

std::string Logger::now()
{
    std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(p);
    char str[26];
    ctime_s(str, sizeof str, &t);
    return str;
}


Logger& operator<<(Logger& logger, const std::string& message)
{
    logger.stream << message;
    return logger;
}

Logger& operator<<(Logger& logger, const long val)
{
    logger.stream << std::to_string(val);
    return logger;
}

Logger& operator<<(Logger& logger, const int val)
{
    logger.stream << std::to_string(val);
    return logger;
}

Logger& operator<<(Logger& logger, const short val)
{
    logger.stream << std::to_string(val);
    return logger;
}

Logger& operator<<(Logger& logger, const double val)
{
    logger.stream << std::to_string(val);
    return logger;
}

Logger& operator<<(Logger& logger, const float val)
{
    logger.stream << std::to_string(val);
    return logger;
}

/*
Logger& operator<<(Logger& logger, const bool val)
{
    logger.stream << (val ? "true" : "false");
    return logger;
}
*/

Logger& operator<<(Logger& logger, ostream_manipulator pf)
{
   pf(logger.stream);
   return logger;
}