#include <cstdlib>
#include <iostream>
#include <getopt.h>
#include <vector>
#include "resource.h"
#include "logger.h"
#include "renamer.h"



using namespace std;
using namespace refactor;


enum Mode
{
    M_Unknown,
    M_Renaming,
    M_Formatting,
    M_Extracting
};


Mode mode = M_Unknown;
string oldSymbol = "";
string newSymbol = "";
vector<string> files;
IdentType type = I_Unknown;


void printHelp()
{
    cout << "Simple tester for librefactor." << endl;
    cout << endl;
    cout << "Usage:" << endl;
    cout << "tester [options] [files]" << endl;
}


int parseArgs(int argc, char* argv[])
{
    opterr = 0;     // disable getopt error msg

    // implicit options

    // definitions of allowed options
    const char *shortOption = "hRo:n:FX";

    const struct option longOption[] =
    {
	{ "help",    no_argument,       NULL, 'h' },
	{ "rename",  no_argument,       NULL, 'R' },
	{ "old",     required_argument, NULL, 'o' },
	{ "new",     required_argument, NULL, 'n' },
	{ "format",  no_argument,       NULL, 'F' },
	{ "extract", no_argument,       NULL, 'X' },
	{ 0,        0,                 0,    0 }
    };


    // while there is something to parse
    int next = 0;
    while( (next = getopt_long(argc, argv, shortOption, longOption, NULL)) != -1 )
    {
        switch(next)
        {
            case 'h':
                printHelp();
            exit(0);
            case 'R':
                mode = M_Renaming;
                break;
            case 'o':
                oldSymbol = optarg;
                break;
            case 'n':
                newSymbol = optarg;
                break;
            case 'F':
                mode = M_Formatting;
                break;
            case 'X':
                mode = M_Extracting;
                break;
            case ':':
                cerr << "ERROR: Missing argument: -" << (char)optopt << endl;
            return 1;
            case '?':
                cerr << "ERROR: Unrecognized option: -" << (char)optopt << endl;
            return 1;
            default:
                cerr << "ERROR: Default option while parsing arguments." << endl;
            printHelp();
            exit(1);
        }
    }

    // read input files as well
    while( optind < argc )
        files.push_back(argv[optind++]);

    switch( mode )
    {
        case M_Unknown:
            cerr << "ERROR: No refactoring task selected." << endl;
            return 1;
        case M_Renaming:
            if( newSymbol.empty() )
            {
                cerr << "ERROR: New name not presented." << endl;
                return 1;
            }

            if( oldSymbol.empty() )     // TODO: pridat lokaciu ked bude fungovat
            {
                cerr << "ERROR: Old name not presented." << endl;
                return 1;
            }
            break;
        case M_Formatting:
        case M_Extracting:
        default:
            cerr << "ERROR: Refactoring task not supported." << endl;
            return 1;
    }

    return 0;
}


//
int main(int argc, char* argv[])
{
    // 0. step: parse arguments
    if( parseArgs(argc, argv) != 0 )
        return EXIT_FAILURE;


    // 1. step: creating new project
    Project testing;
    for(vector<string>::const_iterator it = files.begin(), e = files.end(); it != e; ++it )
    {
        testing.load(*it);
    }


    // 2. step: creating a new renaming task
    Task* renamer = new Renamer(&testing, oldSymbol, newSymbol);


    // 3. step: run analysis
    int res = renamer->analyze();


    // 4. step: some feedback about changes
    Replacements changes = renamer->getChanges();
    for( Replacements::const_iterator it = changes.begin(), e = changes.end(); it != e; ++it )
    {
        cout << (*it).asString() << endl;
    }


    // 5. step: commit changes
    renamer->commit();


    // 6. step: clean up
    delete renamer;


    // TESTING
    return EXIT_SUCCESS;
}
