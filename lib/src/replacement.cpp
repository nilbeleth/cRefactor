#include "replacement.h"

#include <sstream>


using namespace std;
using namespace refactor;


/*****************************
 *       Class Replacement   *
 *****************************/
Replacement::Replacement()
    : m_location(Location()), m_offset(0), m_replaceText("")
{

}

Replacement::Replacement(const Location& loc, const int offset, const std::string& toReplace )
    : m_location(loc), m_offset(offset), m_replaceText(toReplace)
{

}

Replacement::~Replacement()
{
    //dtor
}


bool Replacement::isApplicable() const
{
    if( m_location.isValid() )
        return true;

    return false;
}

bool Replacement::operator==(const Replacement& rhs)
{
    if( m_location != rhs.m_location )
        return false;

    if( m_offset != rhs.m_offset )
        return false;

    if( m_replaceText != rhs.m_replaceText )
        return false;

    return true;
}

bool Replacement::operator!=(const Replacement& rhs)
{
    return !(*this == rhs);
}

string Replacement::asString() const
{
    stringstream ss;
    ss << m_location.asString() << ":+" << m_offset << ":\"" << m_replaceText << "\"";
    return ss.str();
}
