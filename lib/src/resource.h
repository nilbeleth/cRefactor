#ifndef RESOURCE_H
#define RESOURCE_H
#include <string>
#include <vector>



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

        virtual int load(const std::string filename) =0;
        virtual int unload(const std::string filename) =0;

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
        File();
        ~File();

        virtual int load(const std::string filename);
        virtual int unload(const std::string filename);

        static bool exists(std::string filename);

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
        ~Project();

        virtual int load(const std::string filename);
        virtual int unload(const std::string filename);

        virtual std::string asString() const;
    protected:

    private:
        std::vector<std::string> m_sources;

};

}   // end namespace refactor

#endif // RESOURCE_H
