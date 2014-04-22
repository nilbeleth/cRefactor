#include "location.h"
#include "debug.h"

#include <iostream>
#include <sstream>



using namespace std;
using namespace refactor;



/*****************************
 *       Class Location      *
 *****************************/
Location::Location(File* file, const int position)
    : m_file(file), m_position(position), m_valid(false)
{
    //if( file )
        m_valid = true;
}

Location::Location(File* file, const int line, const int column)
{

}

int Location::getLine() const
{
    return 1;
}

int Location::getColumn() const
{
    return 1;
}

std::string Location::asString() const
{
    stringstream ss;

    return ss.str();
}

