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
    //cout << testing.asString();



    // 2. step: creating a new renaming task
    string old = "ClassBase", novy = "Foo";
    Renamer* renamer = new Renamer(&testing, old, novy);

    // 3. step: run analysis
    int res = renamer->analyze();

    // 4. step: some feedback about changes
    // 5. step: commit changes

    // 6. step: clean up
    delete renamer;


    // TESTING
    return EXIT_SUCCESS;
}
