#include "renamer.h"
#include "debug.h"
#include "utils.h"
#include "renamingstrategy.h"

#include <clang/Driver/Options.h>
#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Refactoring.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Rewrite/Core/Rewriter.h>

#include <iostream>
#include <sstream>


using namespace std;
using namespace clang;
using namespace clang::tooling;
using namespace refactor;



/*****************************
 *       Class Renamer       *
 *****************************/
Renamer::Renamer(Resource* resource, const string& origSymbol, const string& newSymbol)
{
    m_impl = new RenamerByName(resource, origSymbol, newSymbol);
}


//
Renamer::Renamer(Resource* resource, const Location& loc, const string& newSymbol)
{
    (void) resource;
    (void) loc;
    (void) newSymbol;
    ERROR("Not yet imeplemented.")
}


//
Renamer::~Renamer()
{
    delete m_impl;
    m_impl = NULL;
}


//
int Renamer::analyze()
{
    INFO("Running analysis for renaming identifiers...")
    int result = m_impl->analyze();
    m_replacements = m_impl->getChanges();

    return result;
}


/*
refactor::Replacements Renamer::getChanges() const
{
    ERROR("Not yet implemented.")
    return vector<Replacement>();
}


int Renamer::applyChange(const Replacement& replace) const
{
    ERROR("Not yet implemented.")
    return 0;
}

//
int Renamer::commit()
{
    ERROR("Not yet implemented.")
    return 0;
}
*/
