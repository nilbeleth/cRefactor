#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>
#include <vector>

#include "logger.h"
#include "defines.h"



namespace refactor
{

class RenamerByName;


/**
 * Defines the type of file (header/source).
 */
enum eFileType
{
    T_Unknown,      /**< For error purposes. */
    T_Source,       /**< A source file. */
    T_Header,       /**< Header file. */
    T_Plain,        /**< Plantext file. */
    T_Binary,       /**< A binary file. */
    T_Directory     /**< Directory. */
};


/**
 *
 */
enum FileStatus
{
    E_Absent,
    E_ReadOnly,
    E_Write
};


/**
 * @brief
 *
 */
class Resource
{
    friend class RenamerByName;

    public:
        /** Default constructor */
        Resource()
            : m_compileCommand(DEFAULT_COMPILE_COMMAND)
        {}

        /** Default destructor */
        virtual ~Resource() {}

        virtual int load(const std::string& filename) =0;
        virtual int unload(const std::string& filename) =0;

        //virtual bool isDirty() const =0;

        /** @brief Set a new compile command. */
        void setCompileCommand(const std::string& command) { m_compileCommand = command; }

        /** @brief Get a compile command for resource. */
        std::string getCompileCommand() const { return m_compileCommand; }

        /** @brief Add some switch to compile command line. */
        void addCompileCommand(const std::string command)
        {
            m_compileCommand += " " + command;
        }


        /** List all sources in resource. */
        virtual std::vector<std::string> getSources() const =0;


        /** @brief Pretty print */
        virtual std::string asString() const =0;


    protected:
        /**
         * @brief Generate JSON compilation database.
         *
         * Generate a file with compilation database for Clang.
         * Currently is supported only "compile_commands.json".
         * @param path  A directory to store new database.
         * @return      Returns 1 if fails, 0 otherwise.
         */
        virtual int generateJSONDatabase(const std::string path = "./") const =0;


        std::string m_compileCommand;
    private:

};


/**
 * @brief A header/source file.
 *
 *
 */
class File : public Resource
{
    friend class RenamerByName;
    friend class Project;
    friend class Location;

    public:
        File(const std::string& name);
        ~File();

        /** @brief Get the file name with absolute path. */
        std::string getFullName() const { return m_path + "/" + m_name; }

        /** @brief Get the file name. */
        std::string getName() const { return m_name; }

        /** @brief Get the absolute path to directory where this file is stored. */
        std::string getAbsPath() const { return m_path; }


        /** @warning Do not use this. */
        virtual int load(const std::string& filename) override;

        /** @warning Do not use this. */
        virtual int unload(const std::string& filename) override;


        bool operator==(const File& rhs);
        bool operator!=(const File& rhs);


        //bool isDirty() const { return m_dirty; }
        //void setDirty(const bool flag = true) { m_dirty = flag; }

        /**
         * @brief Get the type of file.
         *
         * This attribute distinguish between source file and headers.
         * So far, this cannot be set and its determined from filename
         * in class constructor.
         */
        eFileType getType() const { return m_type; }


        /**
         * @brief Get the names of all source files.
         */
        std::vector<std::string> getSources() const override;


        /** @brief Pretty print. */
        virtual std::string asString() const override;


        /**
         * Just find out if file with this name really exists or not.
         * @param filename  Filename
         * @return          Return true if file exists and it could be open for reading.
         */
        static bool exists(const std::string& filename);


        /**
         *
         */
        static bool isHeaderFile(const std::string& filename);

        /**
         *
         */
        static bool isSourceFile(const std::string& filename);


    protected:
        /**
         * @brief Generate JSON compilation database.
         *
         * Generate a file with compilation database for Clang.
         * Currently is supported only "compile_commands.json".
         * @param path  A directory to store new database.
         * @return      Returns 1 if fails, 0 otherwise.
         */
        int generateJSONDatabase(const std::string path = "./") const override;


    private:
        /**
         *  Find an absolute path and filename from relative path to file.
         * @note So far working only for linux.
         * @param name  A relative path to file.
         * @return      Absolute path to file (without file itself).
         */
        std::string getPath(const std::string& name);

        /**
         * Get file name without its path.
         * @note So far working only for linux.
         * @param name  The file with relative path.
         * @return      The name of file.
         */
        std::string getFilename(const std::string& name);


        std::string m_name;     /**< The name of specific file. */
        std::string m_path;     /**< Absolute path to this file. */
        eFileType m_type;       /**< Type of file (header/source). */
        bool m_dirty;           /**< Flag. @warning Not used so far. */
};


/**
 * @brief A project.
 *
 *
 */
class Project final : public Resource
{
    friend class RenamerByName;

    public:
        Project();
        Project(const std::string dir);
        ~Project();

        virtual int load(const std::string& filename);
        virtual int unload(const std::string& filename);

        // TODO (nilbeleth#1#): daky pretazenie na + alebo porovnavanie

        Logger* getLogger() const { return m_logger; }

        //bool validate(const std::string dir);
        //getFile(const std::string filename)


        /** @brief Get the names of all source files. */
        std::vector<std::string> getSources() const override;


        /** @brief Add a new library search path. */
        void addLibPath(const std::string libPath) { m_libPaths.push_back(libPath); }


        /** @brief Pretty print. */
        virtual std::string asString() const override;


        /**
         *
         */
        static std::string getWorkingDirectory();


    protected:
        /**
         * @brief Generate JSON compilation database.
         *
         * Generate a file with compilation database for Clang.
         * Currently is supported only "compile_commands.json".
         * @param path  A directory to store new database.
         * @return      Returns 1 if fails, 0 otherwise.
         */
        int generateJSONDatabase(const std::string path = "./") const override;

        /**
         *
         */
        std::vector<std::string> getLibPaths() const { return m_libPaths; }

    private:
        std::vector<File*> m_sources;
        std::vector<std::string> m_libPaths;
        Logger* m_logger;
};

}   // end namespace refactor

#endif // RESOURCE_H
