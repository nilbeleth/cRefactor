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

        static Task* construct(const Resource& resource, std::string varName);
        virtual void getChanges() const;
        virtual void commit();

    private:
        /** Hide default constructor */
        Renamer() {}


        Renamer* m_impl;
};

}   // end namespace refactor
#endif // RENAMER_H
