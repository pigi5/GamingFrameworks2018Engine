#include "../header/Logger.h"
#include <ctime>
#include <chrono>


Logger::Logger(std::string filename)
{
    this->filename = filename;
    active = true;
    
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

void Logger::setActive(bool active)
{
    this->active = active;
}

Logger& operator<<(Logger& logger, const std::string& message)
{
    if (logger.active)
    {
        logger.stream << message;
    }
    return logger;
}

Logger& operator<<(Logger& logger, const long val)
{
    if (logger.active)
    {
        logger.stream << std::to_string(val);
    }
    return logger;
}

Logger& operator<<(Logger& logger, const int val)
{
    if (logger.active)
    {
        logger.stream << std::to_string(val);
    }
    return logger;
}

Logger& operator<<(Logger& logger, const short val)
{
    if (logger.active)
    {
        logger.stream << std::to_string(val);
    }
    return logger;
}

Logger& operator<<(Logger& logger, const double val)
{
    if (logger.active)
    {
        logger.stream << std::to_string(val);
    }
    return logger;
}

Logger& operator<<(Logger& logger, const float val)
{
    if (logger.active)
    {
        logger.stream << std::to_string(val);
    }
    return logger;
}

Logger& operator<<(Logger& logger, ostream_manipulator pf)
{
    if (logger.active)
    {
        pf(logger.stream);
    }
    return logger;
}