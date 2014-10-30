#include "replacement.h"

#include <sstream>
#include <iostream>


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
    cout << endl << "Location: " << endl;
    cout << "\t" << m_location.asString() << endl;
    cout << "\t" << rhs.m_location.asString() << endl;

    if( m_location != rhs.m_location )
        return false;

    cout << "Offset: " << m_offset << " <-> " << rhs.m_offset << endl;
    if( m_offset != rhs.m_offset )
        return false;

    cout << "ReplaceText: " << m_replaceText << " <-> " << rhs.m_replaceText << endl;
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


/*****************************
 *       Class Replacements  *
 *****************************/
int Replacements::add(Replacement item)
{
    cout << item.asString() << ": ";
    for( auto replacement : _data )
    {
        if( item == replacement )
        {
            cout << "rejected as opposed to: " << replacement.asString() << "\n";
            return 0;
        }
    }

    cout << "accepted\n";
    _data.push_back(item);
    return 1;
}

int Replacements::remove(const Replacement& item)
{
    (void) item;
    return 0;
}

/*
bool Replacements::operator==(const Replacements& rhs)
{
    if( _data.size() != rhs.size() )
        return false;

    for( auto item : _data )
    {
        bool found = false;

        for( auto item2 : rhs )
        {
            if( item == item2 )
            {
                found = true;
                break;
            }
        }

        if( found == false )
            return false;
    }

    return true;
}

bool Replacements::operator!=(const Replacement& rhs)
{
    return !(*this == rhs);
}*/

int Replacements::operator+(const Replacements& rhs)
{
    (void) rhs;
    return 0;
}

string Replacements::asString() const
{
    stringstream ss;

    ss << "Replacements:" << endl;
    for( auto item : _data )
    {
        ss << item.asString() << endl;
    }
    return ss.str();
}
