#include <sstream>
#include <stdio.h>

#include "resource.h"
#include "debug.h"


using namespace std;
using namespace refactor;



////
//  Resource
////


////
//  File
////
refactor::File::File(const std::string& name)
    :m_name(name)
{
}


refactor::File::~File()
{
}

std::string File::asString() const
{
    std::stringstream ss;
    ERROR("Not yet imeplemeted.")
    return ss.str();
}

int File::load(const string& filename)
{
    ERROR("Not yet implemented.")
    return 0;
}

int File::unload(const string& filename)
{
    ERROR("Not yet implemented.")
    return 0;
}


bool File::exists(const string& filename)
{
    if( FILE *file = fopen(filename.c_str(), "r"))
    {
        fclose(file);
        return true;
    }

    return false;
}


////
//  Project
////
Project::Project()
    : m_logger(NULL)
{
    m_logger = new StdLogger();
}

//
Project::Project(const std::string dir)
{
    ERROR("Not yet imeplemeted.")
}

//
Project::~Project()
{
    // we have probably some logger there
    if( m_logger )
    {
        delete m_logger;
        m_logger = NULL;
    }

    // dispose all files
    for(set<File*>::iterator it = m_sources.begin(), e = m_sources.end(); it != e; ++it)
    {
        delete *it;
    }
    m_sources.clear();
}

//
int Project::load(const string& filename)
{
    if( File::exists(filename) )
    {
        m_sources.insert(new File(filename));
        if( m_logger )
            m_logger->info("File \"" + filename + "\" added to project.");
    }
    else
    {
        if( m_logger )
            m_logger->error("File \"" + filename + "\" not found.");
    }
    return 0;
}

//
int Project::unload(const string& filename)
{
    ERROR("Not yet imeplemented.")
    return 0;
}

//
string Project::asString() const
{
    stringstream ss;

    if( m_sources.empty() )
    {
        ss << "Project: empty" << endl;
    }
    else
    {
        ss << "Project:" << endl;

        for(set<File*>::const_iterator it = m_sources.begin(), e = m_sources.end(); it != e; ++it)
            ss << "\t" << (*it)->getName() << endl;
    }
    return ss.str();
}
