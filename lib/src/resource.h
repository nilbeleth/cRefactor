#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>
#include <set>

#include "logger.h"



namespace refactor
{


/**
 *
 */
enum eModify
{
    mDirect,
    mFile,
    mDirectory
};


/**
 *
 */
enum FileStatus
{
    E_Ok,
    E_Absent,
    E_ReadOnly,
    E_Binary,
    E_Directory
};


/**
 * @brief
 *
 */
class Resource
{
    public:
        /** Default constructor */
        Resource() {}
        /** Default destructor */
        virtual ~Resource() {}

        virtual int load(const std::string& filename) =0;
        virtual int unload(const std::string& filename) =0;

        //virtual bool isDirty() const =0;

        virtual std::string asString() const =0;
    protected:
    private:

};


/**
 * @brief
 *
 *
 */
class File : public Resource
{
    public:
        File(const std::string& name);
        ~File();

        std::string getName() const { return m_name; }
        //std::string getPath() const;
        //std::string getAbsPath() const;

        virtual int load(const std::string& filename);
        virtual int unload(const std::string& filename);

        static inline bool exists(const std::string& filename);

        virtual std::string asString() const;
    protected:

    private:
        std::string m_name;
};


/**
 * @brief
 *
 *
 */
class Project : public Resource
{
    public:
        Project();
        Project(const std::string dir);
        ~Project();

        virtual int load(const std::string& filename);
        virtual int unload(const std::string& filename);

        // TODO (nilbeleth#1#): daky pretazenie na + alebo porovnavanie

        Logger* getLogger() const { return m_logger; }

        //bool validate(const std::string dir);
        //addObserver();
        //removeObserver();
        //getFile(const std::string filename)



        virtual std::string asString() const;
    protected:

    private:
        std::set<File*> m_sources;
        Logger* m_logger;

};

}   // end namespace refactor

#endif // RESOURCE_H
