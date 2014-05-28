#include <sstream>
#include <stdio.h>
#include <fstream>
//#include <stdexcept>
#ifndef __WIN
    #include <limits.h>
    #include <unistd.h>
    #include <stdlib.h>
#else
    #include <windows.h>
#endif // __WIN

#include "resource.h"
#include "utils.h"
#include "debug.h"


using namespace std;
using namespace refactor;



/*****************************
 *       Class Resource      *
 *****************************/


/*****************************
 *       Class File          *
 *****************************/
File::File(const std::string& name)
    : Resource()
{
    m_path = getPath(name);
    m_name = getFilename(name);

    // distinguish between header and source file
    size_t pos = m_name.find(".");
    if( pos != string::npos )
    {
        // There is a distinct possibility that file ends with ".".
        // In this case I've got std::out_of_range exception here.
        // Otherwise this should work just fine.
        if( m_name.at(pos+1) == 'c' )
            m_type = T_Source;
        else if( m_name.at(pos+1) == 'h' )
            m_type = T_Header;
        else
            m_type = T_Plain;
    }
    else
        m_type = T_Plain;
}


File::~File()
{
}

std::string File::asString() const
{
    std::stringstream ss;
    ss << m_path << "/" << m_name;
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


bool File::operator==(const File& rhs)
{
    return getFullName() == rhs.getFullName();
}


bool File::operator!=(const File& rhs)
{
    return getFullName() != rhs.getFullName();
}


bool File::exists(const string& filename)
{
    // TODO (nilbeleth#1#): ak by som to spravil platform-dependent existuje podstatne efektivnejsie riesenie
    if( FILE *file = fopen(filename.c_str(), "r"))
    {
        fclose(file);
        return true;
    }

    return false;
}

bool File::isHeaderFile(const string& filename)
{
    ERROR("Not yet implemented.")
    return false;
}

bool File::isSourceFile(const string& filename)
{
    ERROR("Not yet implemented.")
    return false;
}


string File::getPath(const string& name)
{
    // FIXME (nilbeleth#1#): platform-dependant
    char buffer[PATH_MAX];
    if( NULL == realpath(name.c_str(), buffer) )
        ERROR("Can't get correct absolute path.")

    // remove the file from path
    string path = buffer;
    return path.substr(0, path.find_last_of("/"));
}


string File::getFilename(const string& name)
{
    return name.substr(name.find_last_of("/") +1, string::npos);
}

int File::generateJSONDatabase(const string path) const
{
    string filePath = path + COMPILE_DB_FILE;
    ofstream out(filePath.c_str());
    if( !out.is_open() )
    {
        ERROR("Unable to open compilation database for writing.")
        return 1;
    }


    out << "[" << endl;

    out << "  {" << endl;
    out << "    \"directory\": \"" << m_path << "\"," << endl;
    out << "    \"command\": \"" << escapeString(m_compileCommand) << " " << m_name << "\"," << endl;
    out << "    \"file\": \"" << m_name << "\"" << endl;
    out << "  }" << endl;

    out << "]" << endl;

    out.close();
    return 0;
}

vector<string> File::getSources() const
{
    vector<string> sources;
    if( m_type == T_Source )
        sources.push_back(m_name);
    return sources;
}


/*****************************
 *       Class Project       *
 *****************************/
Project::Project()
    : Resource(), m_logger(NULL)
{
    m_logger = new StdLogger();

    // TODO (nilbeleth#1#): defaultne kvoli tomu stdarg.h (implicitne to byva s clangom)
    m_libPaths.push_back("/usr/lib64/clang/3.3/include/");
}

//
Project::Project(const std::string dir)
{
    ERROR("Not yet implemeted.")
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
    for(vector<File*>::iterator it = m_sources.begin(), e = m_sources.end(); it != e; ++it)
    {
        delete *it;
        *it = NULL;
    }
    m_sources.clear();
}

//
int Project::load(const string& filename)
{
    if( File::exists(filename) )
    {
        m_sources.push_back(new File(filename));
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


int Project::generateJSONDatabase(const string path) const
{
    string filePath = path + COMPILE_DB_FILE;
    ofstream out(filePath.c_str());
    if( !out.is_open() )
    {
        ERROR("Unable to open compilation database for writing.")
        return 1;
    }

    // include all search paths for libraries
    string libPaths;
    for(vector<string>::const_iterator it = m_libPaths.begin(), e = m_libPaths.end(); it != e; ++it)
        libPaths += "-I" + *it + " ";

    // create record for each file
    out << "[" << endl;

    for(vector<File*>::const_iterator it = m_sources.begin(), e = m_sources.end(); it != e; ++it)
    {
        out << "  {" << endl;
        out << "    \"directory\": \"" << (*it)->getAbsPath() << "\"," << endl;
        out << "    \"command\": \"" << escapeString(m_compileCommand + " " + libPaths) << " " << (*it)->getName() << "\"," << endl;
        out << "    \"file\": \"" << (*it)->getName() << "\"" << endl;
        out << "  }";

        if( it+1 != e )
            out << ",";
        out << endl;
    }
    out << "]" << endl;

    out.close();
    return 0;
}


vector<string> Project::getSources() const
{
    vector<string> sources;

    for(vector<File*>::const_iterator it = m_sources.begin(), e = m_sources.end(); it != e; ++it)
    {
        if( (*it)->getType() == T_Source )
            sources.push_back((*it)->getAbsPath() + "/" +(*it)->getName());
    }

    return sources;
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

        for(vector<File*>::const_iterator it = m_sources.begin(), e = m_sources.end(); it != e; ++it)
            ss << "\t" << (*it)->asString() << endl;
    }

    return ss.str();
}


string Project::getWorkingDirectory()
{
    string result = "";

    // sadly c++ standard don't say a word about file access so this needs to be solved separetly for each platform
#ifndef __WIN
    char buffer[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", buffer, PATH_MAX );
    result = string(buffer, (count > 0) ? count : 0);

    // now get the file out
    result = result.substr(0, result.find_last_of("/"));
#else
    // TODO (nilbeleth#1#): na Windowse to nemusi vracat len adresar ale aj exac (samotna appka)
    char buffer[PATH_MAX];
    result = string(buffer, GetModuleFilename(NULL, buffer, MAX_PATH))
#endif // __WIN

    return result;
}
