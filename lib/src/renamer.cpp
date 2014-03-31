#include "renamer.h"
#include "debug.h"
#include "preprocessorcontext.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"


using namespace std;
using namespace clang;
using namespace refactor;



/**
 *
 */
class RenamingVisitor/* : public clang::RecursiveASTVisitor<RenamingVisitor>*/
{
    public:
        RenamingVisitor() {}
        virtual ~RenamingVisitor() {}

        bool VisitFunctionDecl(FunctionDecl *f);
    protected:
    private:
};


bool RenamingVisitor::VisitFunctionDecl(FunctionDecl *f)
{
    DEBUG("Hooray!!!")
    return true;
}


/**
 * @brief
 */
class RenamingConsumer : public ASTConsumer
{
    public:
        RenamingConsumer() {}
        virtual ~RenamingConsumer() {}

        // just traverse all top declarations
        bool HandleTopLevelDecl(DeclGroupRef d);

        //void setNewSymbolName(const string& name) { m_newName; }
    protected:
    private:
        //string m_oldName;
        //string m_newName;
};


bool RenamingConsumer::HandleTopLevelDecl(DeclGroupRef d)
{
    RenamingVisitor rv;
    for(clang::DeclGroupRef::iterator it = d.begin(), e = d.end(); it != e; ++it)
    {
        //rv.TraverseDecl(*it);
    }

    return true;
}



/*****************************
 *       Class Renamer       *
 *****************************/
Renamer::~Renamer()
{
    delete m_impl;
    m_impl = NULL;
}

Task* Renamer::construct(const Resource& resource, const string& symbol)
{
    return NULL;
}
