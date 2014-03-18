#include "preprocessorcontext.h"

#include <iostream>

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"


using namespace refactor;

using clang::CompilerInstance;
using clang::TargetOptions;
using clang::TargetInfo;
using clang::FileEntry;
//using clang::Token;
using clang::ASTContext;
using clang::ASTConsumer;
using clang::Parser;
using clang::DiagnosticOptions;
using clang::TextDiagnosticPrinter;
using clang::Preprocessor;



//
PreprocessorContext::PreprocessorContext()
{
    //ctor
}

//
PreprocessorContext::~PreprocessorContext()
{
    //dtor
}

//
void PreprocessorContext::initialize()
{
    m_ci = new CompilerInstance();
    DiagnosticOptions diagOpts;
    m_ci->createDiagnostics();

    llvm::IntrusiveRefCntPtr<TargetOptions> pto( new TargetOptions() );
    pto->Triple = llvm::sys::getDefaultTargetTriple();
    TargetInfo *pti = TargetInfo::CreateTargetInfo(m_ci->getDiagnostics(), pto.getPtr());
    m_ci->setTarget(pti);

    m_ci->createFileManager();
    m_ci->createSourceManager(m_ci->getFileManager());
    m_ci->createPreprocessor();
    m_ci->getPreprocessor();
    m_ci->getPreprocessorOpts().UsePredefines = false;
}

//
void PreprocessorContext::parse(const Resource& resource, clang::ASTConsumer consumer)
{
    m_ci->setASTConsumer(&consumer);

    m_ci->createASTContext();

    const FileEntry *pFile = m_ci->getFileManager().getFile("pica.cpp");
    m_ci->getSourceManager().createMainFileID(pFile);
    m_ci->getDiagnosticClient().BeginSourceFile(m_ci->getLangOpts(), &m_ci->getPreprocessor());
    clang::ParseAST(m_ci->getPreprocessor(), &consumer, m_ci->getASTContext());
    m_ci->getDiagnosticClient().EndSourceFile();

    return;
}
