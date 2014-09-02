#include "task.h"
#include "debug.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include <iostream>


using namespace std;
using namespace clang;
using namespace refactor;



Task::Task()
    : m_rewriter(nullptr), m_SM(nullptr), m_FM(nullptr)
{
    if( !initializeRewriter() )
    {
        m_rewriter = nullptr;
        m_SM = nullptr;
        m_FM = nullptr;
    }
}

Task::~Task()
{
    if( m_rewriter != nullptr )
        delete m_rewriter;
    if( m_SM != nullptr )
        delete m_SM;
    if( m_FM != nullptr )
        delete m_FM;
}

int Task::applyChange(const Replacement& replace)
{
    if( m_rewriter == nullptr )
    {
        if( !initializeRewriter() )
            return 1;
    }

    if( !replace.isApplicable() )
        return 0;

    SourceLocation loc = replace.getLocation().getAsSourceLocation(*m_SM);
    m_rewriter->ReplaceText(loc, replace.getOffset(), replace.getReplaceText());

#if OVERWRITE_FILES
    m_rewriter->overwriteChangedFiles();
#endif // OVERWRITE_FILES
    return 0;
}

int Task::commit()
{
    if( m_rewriter == nullptr )
    {
        if( !initializeRewriter() )
            return 1;
    }

    int result = 0;

    for(Replacements::const_iterator it = m_replacements.begin(), e = m_replacements.end(); it != e; ++it)
    {
        result |= applyChange(*it);
    }

    //m_rewriter->overwriteChangedFiles();

/*
    const FileEntry* entry = m_rewriter->getSourceMgr().getFileManager().getFile("test.cpp");
    if( entry == nullptr )
    {
        ERROR("File not found.")
        return 1;
    }
    const FileID id = m_rewriter->getSourceMgr().translateFile(entry);
    const RewriteBuffer* buffer = m_rewriter->getRewriteBufferFor(id);
    cout << ">>>>> File:" << endl;
    cout << string(buffer->begin(), buffer->end());
    cout << "<<<<<< End file" << endl;
*/
    return result;
}

//
bool Task::initializeRewriter()
{
    DiagnosticOptions diagOpts;
    TextDiagnosticPrinter *pTextDiagnosticPrinter = new clang::TextDiagnosticPrinter(llvm::outs(), &diagOpts, true);
    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> pDiagIDs;
    DiagnosticsEngine * diagsEngine = new clang::DiagnosticsEngine(pDiagIDs, &diagOpts, pTextDiagnosticPrinter);

    FileSystemOptions fileSystemOptions;
    m_FM = new FileManager(fileSystemOptions);

    m_SM = new SourceManager(*diagsEngine, *m_FM);

    LangOptions langOpts;
    langOpts.GNUMode = 1;
    langOpts.CXXExceptions = 1;
    langOpts.RTTI = 1;
    langOpts.Bool = 1;
    langOpts.CPlusPlus = 1;

    m_rewriter = new Rewriter(*m_SM, langOpts);

    return true;
}
