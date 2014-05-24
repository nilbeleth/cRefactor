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
    : m_rewriter(NULL), m_SM(NULL), m_FM(NULL)
{
    if( !initializeRewriter() )
    {
        m_rewriter = NULL;
        m_SM = NULL;
        m_FM = NULL;
    }
}

Task::~Task()
{
    if( m_rewriter != NULL )
        delete m_rewriter;
    if( m_SM != NULL )
        delete m_SM;
    if( m_FM != NULL )
        delete m_FM;
}

int Task::applyChange(const Replacement& replace)
{
    if( m_rewriter == NULL )
    {
        if( !initializeRewriter() )
            return 1;
    }

    if( !replace.isApplicable() )
        return 0;

    SourceLocation loc = replace.getLocation().getAsSourceLocation(*m_SM);
    m_rewriter->ReplaceText(loc, replace.getOffset(), replace.getReplaceText());
    DEBUG("Replacement --> " << loc.printToString(*m_SM) << ": " << replace.getOffset() << " for " << replace.getReplaceText())
    return 0;
}

int Task::commit()
{
    if( m_rewriter == NULL )
    {
        if( !initializeRewriter() )
            return 1;
    }

    int result = 0;

    for(Replacements::const_iterator it = m_replacements.begin(), e = m_replacements.end(); it != e; ++it)
    {
        result |= applyChange(*it);
    }

    const FileEntry* entry = m_rewriter->getSourceMgr().getFileManager().getFile("test.cpp");
    if( entry == NULL )
    {
        ERROR("A je to v pici")
        return 1;
    }
    const FileID id = m_rewriter->getSourceMgr().translateFile(entry);

    const RewriteBuffer* buffer = m_rewriter->getRewriteBufferFor(id);

    cout << ">>>>> File:" << endl;
    cout << string(buffer->begin(), buffer->end());
    cout << "<<<<<< End file" << endl;
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
