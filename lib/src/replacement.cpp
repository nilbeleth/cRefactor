#include "replacement.h"

#include <sstream>


using namespace std;
using namespace refactor;


/*****************************
 *       Class Replacement   *
 *****************************/
Replacement::Replacement()
{
    //ctor
}

Replacement::~Replacement()
{
    //dtor
}


bool Replacement::isApplicable() const
{
    return true;
}

bool Replacement::operator==(const Replacement& rhs)
{
    return true;
}

bool Replacement::operator!=(const Replacement& rhs)
{
    return false;
}

string Replacement::asString() const
{
    stringstream ss;

    return ss.str();
}
