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
class RenamingVisitor : public clang::RecursiveASTVisitor<RenamingVisitor>
{
    public:
        RenamingVisitor() {}

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
 *
 */
class RenamingConsumer : /*public Renamer,*/ public ASTConsumer
{
    public:
        RenamingConsumer() : ASTConsumer() {}
        virtual ~RenamingConsumer() {}

        // just traverse all top declarations
        virtual bool HandleTopLevelDecl(DeclGroupRef d);

        void setNewSymbolName(const string& name) { m_newName; }
    protected:
    private:
        string m_oldName;
        string m_newName;
};


bool RenamingConsumer::HandleTopLevelDecl(DeclGroupRef d)
{
    RenamingVisitor rv;
    for(clang::DeclGroupRef::iterator it = d.begin(), e = d.end(); it != e; ++it)
    {
        rv.TraverseDecl(*it);
    }

    return true;
}
/**
 * @brief
 *
 *
 */


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
