#ifndef LOGGER_H
#define LOGGER_H
#include <string>


namespace refactor
{


/**
 * Describes different types of messages
 * ....
 */
enum TVerbosityLevel
{
    V_None      = 0,
    V_Info      = 1,
    V_Warning   = 2,
    V_Fatal     = 4,
    V_All       = 7
};


/**
 *
 */
class Logger
{
    public:
        /** Default constructor */
        Logger() {}
        /** Default destructor */
        virtual ~Logger() {}


        /** Simple getter for verbosity level setting. */
        unsigned getVerbosity() const { return m_verbosity; }


        /**
         * Log a new message.
         * @param msg
         * @param severity
         */
        virtual void log(const std::string msg, const int severity) const =0;

        /**
         * Log new message as informative one.
         * @param msg
         */
        virtual void info(const std::string msg) const =0;

        /**
         * Log new message as warning.
         * @param msg
         */
        virtual void warn(const std::string msg) const =0;

        /**
         * Log new message as error.
         * @param msg
         */
        virtual void error(const std::string msg) const =0;

        /**
         * Log new message as fatal error.
         * @param msg
         */
        virtual void fatal(const std::string msg) const =0;

    protected:
        unsigned m_verbosity;
    private:
};



/**
 *
 */
class StdLogger : public Logger
{
    public:
    protected:
    private:
};

}   // end namespace refactor
#endif // LOGGER_H
