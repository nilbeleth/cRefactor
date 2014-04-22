#include <cstdlib>
#include <iostream>
#include "src/resource.h"
#include "src/logger.h"
#include "src/renamer.h"


using namespace std;
using namespace refactor;



//
int main(int argc, char* argv[])
{

    // 1. step: creating new project
    Project testing;
    for(int i = 1; i < argc; ++i)
        testing.load(argv[i]);
    cout << testing.asString();



    // 2. step: creating a new renaming task
    string old = "x", novy = "y";
    Renamer* renamer = new Renamer(&testing, old, novy);

    // 3. step: some feedback about changes
    // 4. step: commit changes

    // 5. step: clean up
    delete renamer;


    // TESTING
    return EXIT_SUCCESS;
}
