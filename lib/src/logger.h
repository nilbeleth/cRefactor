#ifndef LOGGER_H
#define LOGGER_H
#include <string>


namespace refactor
{


/**
 * Describes different types of messages.
 */
enum class TLogLevel
{
    V_Ignored   = 0,
    V_Info      = 1,
    V_Warning   = 2,
    V_Error     = 4,
    V_Fatal     = 8,
    V_All       = 15
};


/**
 * @brief An interface for logging messages.
 *
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
        virtual void log(const std::string msg, const TLogLevel severity = TLogLevel::V_Info) const =0;

        /**
         * Log new message as an informative one.
         * @param msg
         */
        virtual void info(const std::string msg) const =0;

        /**
         * Log new message as a warning.
         * @param msg
         */
        virtual void warn(const std::string msg) const =0;

        /**
         * Log new message as an error.
         * @param msg
         */
        virtual void error(const std::string msg) const =0;

        /**
         * Log new message as a fatal error.
         * @param msg
         */
        virtual void fatal(const std::string msg) const =0;

    protected:
        unsigned m_verbosity;
        std::string getTime() const;
};



/**
 * @brief A logger writing to stdout.
 *
 *
 */
class StdLogger final : public Logger
{
    public:
        StdLogger() {}
        virtual ~StdLogger() {}

        /**
         * Log a new message.
         * @param msg
         * @param severity
         */
        virtual void log(const std::string msg, const TLogLevel severity = TLogLevel::V_Info) const override;

        /**
         * Log new message as an informative one.
         * @param msg
         */
        virtual void info(const std::string msg) const override;

        /**
         * Log new message as a warning.
         * @param msg
         */
        virtual void warn(const std::string msg) const override;

        /**
         * Log new message as an error.
         * @param msg
         */
        virtual void error(const std::string msg) const override;

        /**
         * Log new message as a fatal error.
         * @param msg
         */
        virtual void fatal(const std::string msg) const override;
    protected:
    private:
};

}   // end namespace refactor
#endif // LOGGER_H
