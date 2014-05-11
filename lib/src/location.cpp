#include "location.h"
#include "debug.h"

#include <iostream>
#include <sstream>



using namespace std;
using namespace refactor;



/*****************************
 *       Class Location      *
 *****************************/
Location::Location(const std::string filePath, const int position)
    : m_filePath(filePath), m_position(position), m_valid(false)
{
    //if( file )
        m_valid = true;
}

Location::Location(const std::string filePath, const int line, const int column)
{

}

bool Location::operator==(const Location& rhs)
{
    if( m_valid && rhs.isValid() )
    {
        if( m_position == rhs.m_position )
            return true;
    }

    return false;
}

bool Location::operator!=(const Location& rhs)
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

