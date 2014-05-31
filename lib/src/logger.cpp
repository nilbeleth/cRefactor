#include <ctime>
#include <sstream>
#include <iostream>
#include "logger.h"
#include "debug.h"

#define SATISFY_LVL(X,LVL) ( (X) & (Y) ? true : false )


using namespace refactor;



std::string Logger::getTime() const
{
    std::stringstream ss;
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%d/%m/%y %T", timeinfo);

    ss << buffer;

    return ss.str();
}


////
//  StdLogger
////
void StdLogger::log(const std::string msg, const TLogLevel severity) const
{
    switch(severity)
    {
        case TLogLevel::V_Info:
            info(msg);
            break;
        case TLogLevel::V_Warning:
            warn(msg);
            break;
        case TLogLevel::V_Error:
            error(msg);
            break;
        case TLogLevel::V_Fatal:
            fatal(msg);
            break;
        default:
            WARNING("Invalid severity.")
    }
}

void StdLogger::info(const std::string msg) const
{
    std::cerr << getTime() << " INFO : " << msg << std::endl;
}

void StdLogger::warn(const std::string msg) const
{
    std::cerr << getTime() << " WARN : " << msg << std::endl;
}

void StdLogger::error(const std::string msg) const
{
    std::cerr << getTime() << " ERROR: " << msg << std::endl;
}

void StdLogger::fatal(const std::string msg) const
{
    std::cerr << getTime() << " FATAL: " << msg << std::endl;
}
