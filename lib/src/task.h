#ifndef TASK_H
#define TASK_H
#include <string>

#include "resource.h"



namespace refactor
{


/**
 *
 */
class Task
{
    public:
        /** Default destructor */
        virtual ~Task();

        static Task* construct(const Resource& resource, const std::string& symbol);
        // some location of desired change
        static Task* construct(const Resource& resource, const int loc);

        virtual void getChanges() const =0;
        virtual void commit() =0;
    protected:
        /** Hide default constructor */
        Task() {}
    private:
};

}   // end namespace refactor
#endif // TASK_H
