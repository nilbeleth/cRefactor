#include "task.h"


using namespace refactor;



Task::~Task()
{
    //dtor
}

int Task::applyChange(const Replacement& replace) const
{
    return 0;
}

int Task::commit() const
{
    return 0;
}
