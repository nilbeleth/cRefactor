#include "location.h"
#include "debug.h"

#include <iostream>
#include <sstream>



using namespace std;
using namespace clang;
using namespace refactor;



/*****************************
 *       Class Location      *
 *****************************/
Location::Location(const std::string filePath, const unsigned position)
    : m_filePath(""), m_line(1), m_column(1)
{
    (void) position;
    ERROR("Not yet implemented.")
}

Location::Location(const std::string filePath, const unsigned line, const unsigned column)
    : m_filePath(filePath), m_line(line), m_column(column)
{
    // TODO (nilbeleth#1#): kontrola ci neprechadza za koniec a column posunut dalej
}

Location::Location(const SourceManager& SM, const SourceLocation loc)
{
    m_line = SM.getExpansionLineNumber(loc);
    m_column = SM.getExpansionColumnNumber(loc);
    m_filePath = SM.getFilename(loc);
}

Location Location::getLocWithOffset(const int offset) const
{
    return Location(m_filePath, m_line, m_column + offset);
}

bool Location::operator==(const Location& rhs)
{
    if( isValid() && rhs.isValid() )
    {
        if( m_line != rhs.m_line )
            return false;

        if( m_column != rhs.m_column )
            return false;

        return true;
    }

    return false;
}

bool Location::operator!=(const Location& rhs)
{
    if( isValid() && rhs.isValid() )
    {
        if( m_line == rhs.m_line )
            return false;

        if( m_column == rhs.m_column )
            return false;

        return true;
    }

    return true;
}

unsigned Location::getLine() const
{
    return m_line;
}

unsigned Location::getColumn() const
{
    return m_column;
}

std::string Location::asString() const
{
    stringstream ss;
    if( isValid() )
        ss << m_filePath << ":" << m_line << ":" << m_column;
    else
        ss << "<invalid loc>";
    return ss.str();
}

SourceLocation Location::getAsSourceLocation(SourceManager& SM) const
{
    const FileEntry* entry = SM.getFileManager().getFile(m_filePath, true);
    if( entry != NULL )
    {
        FileID ID;

        SourceLocation location = SM.translateFileLineCol(entry, 1, 1);

        if( location.isValid() )
            ID = SM.getFileID(location);
        else
            ID = SM.createFileID(entry, SourceLocation(), SrcMgr::C_User);

        SourceLocation start = SM.translateLineCol(ID, m_line, m_column);

        //const SourceLocation start = SM.getLocForStartOfFile(ID);
        //DEBUG("Location translated as: " << start.printToString(SM));
        //if( location.isValid() )
            return start;
    }

    return SourceLocation();
}

Location Location::getAsThisLocation(const SourceManager& SM, const SourceLocation loc)
{
    WARNING("Location::getAsThisLocation() is deprecated. Use Location(const SourceLocation loc, const SourceManager& SM) instead!")
    PresumedLoc pLoc = SM.getPresumedLoc(loc);
    return Location(pLoc.getFilename(), pLoc.getLine(), pLoc.getColumn());
}
