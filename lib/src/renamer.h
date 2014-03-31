#ifndef RENAMER_H
#define RENAMER_H
#include "task.h"
#include "resource.h"
#include <string>


namespace refactor
{


/**
 *
 */
class Renamer : public Task
{
    public:
        /** Default destructor */
        virtual ~Renamer();

        static Task* construct(const Resource& resource, const std::string& symbol);
        // some location of desired change
        //static Task* construct(const Resource& resource, const int loc);

        virtual void getChanges() const {}
        virtual void commit() {}

    protected:
        /** Hide default constructor */
        Renamer() {}

    private:
        Renamer* m_impl;
};

}   // end namespace refactor
#endif // RENAMER_H
