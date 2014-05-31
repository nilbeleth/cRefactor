#include "reformatter.h"
#include "debug.h"

#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Format/Format.h>
#include <clang/Lex/Lexer.h>
//#include <clang/Rewrite/Core/Rewriter.h>
//#include <llvm/Support/Debug.h>
//#include <llvm/Support/FileSystem.h>
//#include <llvm/Support/Signals.h>
//#include <llvm/ADT/StringMap.h>
#include <llvm/Support/YAMLTraits.h>

#include <iostream>


using namespace std;
using namespace llvm;
using namespace clang;
using namespace refactor;



static int format(const string& filename)
{
    /*
    FileManager files(FileSystemOptions());
    DiagnosticsEngine diags(IntrusiveRefCntPtr<DiagnosticIDs>(new DiagnosticIDs), new DiagnosticOptions);
    SourceManager sm(diags, files);
    OwningPtr<MemoryBuffer> code;       // some sort of buffer for the file
    if(error_code ec = MemoryBuffer::getFile(filename, code) )
    {
        llvm::errs() << "ERROR: " << ec.message() << "\n";
        return 1;
    }
    if( code->getBufferSize() == 0 )
        return 0;       // empty files are formatted correctly

    FileID id = createInMemoryFile(filename, code.get(), sm, files);
    std::vector<CharSourceRange> ranges;
    if( fillRanges(Sources, ID, Code.get(), ranges) )
        return 1;
    */
    //format::FormatStyle formatStyle = format::getLLVMStyle();
    //cout << configurationAsText(formatStyle);

    return 0;
}


/*****************************
 *       Class Reformatter   *
 *****************************/
Reformatter::Reformatter(Resource* resource, CodeStyle style)
{
    (void) resource;
    (void) style;
    ERROR("Not yet implemented.")
    //ctor
}

Reformatter::~Reformatter()
{
    //dtor
}

int Reformatter::analyze()
{
    int result = ::format("test.cpp");
    return result;
}
