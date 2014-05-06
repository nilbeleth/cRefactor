#include "renamer.h"
#include "debug.h"
#include "utils.h"

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

#include <clang/ASTMatchers/ASTMatchFinder.h>       // TODO: pokus
#include <clang/ASTMatchers/ASTMatchers.h>

#include <iostream>
#include <sstream>


using namespace std;
using namespace clang;
//using namespace clang::driver;
using namespace clang::tooling;
//using namespace clang::ast_matchers;
using namespace refactor;



/*****************************
 *       Class Mutator       *
 *****************************/
class RenamingMutator : public ASTConsumer, public RecursiveASTVisitor<RenamingMutator>
{
    public:
        explicit RenamingMutator(const Renamer* renamer)
            : _astContext(NULL), _renamer(renamer)
        { }

        // set new context and traverse AST
        virtual void HandleTranslationUnit(ASTContext &context);

        virtual bool VisitNamedDecl(NamedDecl* decl);
/*        {
            string name = decl->getNameAsString();
            string oldSymbol = _renamer->getOrigSymbol();

            // destructor has a ~ before, so we need to know if it's a destructor or not
            if(isa<CXXDestructorDecl>(decl))
            {
                if(string("~" + oldSymbol) != name)
                    return true;
            }
            else
                if( name != oldSymbol )
                    return true;

            // some built-in declarations
            SourceLocation loc = decl->getLocation();
            if( !loc.isValid() )
                return true;

            // Ignore symbols from system headers
            if( _astContext->getSourceManager().isInSystemHeader(loc) )
                return true;

            cout << "Declaration of (" << oldSymbol << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;

            return true;
        }
*/

        virtual bool VisitVarDecl(VarDecl* decl);
        //
        virtual bool VisitDeclRefExpr(DeclRefExpr* expr)
        {
            // is this somewhere declared?
            NamedDecl* decl = expr->getFoundDecl();
            string oldSymbol = _renamer->getOrigSymbol();
            if( decl )
            {
                string name = expr->getNameInfo().getAsString();
                SourceLocation loc = expr->getLocation();

                // does the name match?
                if( CXXDestructorDecl::classof(decl) )
                {
                    if( string("~" + oldSymbol) != name )
                        return true;
                }
                else
                    if( oldSymbol != name )
                        return true;

                // Ignore symbols from system headers
                if( _astContext->getSourceManager().isInSystemHeader(loc) )
                    return true;


                cout << "Reference of (" << name << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
            }

            return true;
        }

    protected:
    private:
        ASTContext* _astContext;
        const Renamer* _renamer;
};

//
void RenamingMutator::HandleTranslationUnit(ASTContext& context)
{
    _astContext = &context;

    TranslationUnitDecl* D = context.getTranslationUnitDecl();
    TraverseDecl(D);

    _astContext = NULL;
}

//
bool RenamingMutator::VisitNamedDecl(NamedDecl* decl)
{
    SourceLocation loc = decl->getLocation();
    string name = decl->getNameAsString();

    if( !loc.isValid() )
        return true;

    if( _astContext->getSourceManager().isInSystemHeader(loc) )
        return true;

    // priamo deklaracia s tymto nazvom
    if( name == _renamer->getOrigSymbol() )
    {
        cout << "Declaration of (" << _renamer->getOrigSymbol() << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
        //string file = _astContext->getSourceManager().getFilename(loc);
        //cout << file << endl;
    }
    //
    //if( !EnumConstantDecl::classof(decl) )
    //    return true;
    //decl->printQualifiedName(llvm::outs());
    return true;
}

bool RenamingMutator::VisitVarDecl(VarDecl* decl)
{
    //string name = decl->getNameAsString();
    //QualType type = decl->getType();
    //SourceLocation loc = decl->getLocation();   // typeloc cez decl->getTypeSourceInfo()->getTypeLoc()

    QualType typeQT = decl->getType();
    const Type* type = typeQT.getTypePtrOrNull();
    SourceLocation loc = decl->getTypeSpecStartLoc();


    if( !loc.isValid() )
        return true;

    if( _astContext->getSourceManager().isInSystemHeader(loc) )
        return true;

    string name = typeQT.getAsString();
    size_t pos = name.find(" " + _renamer->getOrigSymbol());
    if( pos != string::npos )
    {
        cout << "Type usage of (" << _renamer->getOrigSymbol() << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
    }

    //cout << name << endl;

    /*
    if( type != NULL )
    {
        cout << "\tEnumeration: " << (type->isEnumeralType() ? "true" : "false") << endl;
    }*/
    return true;
}



/**********************************
 *   Class FrontendActionFactory  *
 **********************************/
class RenamingActionFactory
{
    const Renamer* _renamer;

    public:
        RenamingActionFactory(const Renamer* renamer)
            : _renamer(renamer)
        { }

        virtual clang::ASTConsumer *newASTConsumer()
        {
            return new RenamingMutator(_renamer);
        }
};



/**
 * @brief
 *
 *
 */
class RenamingStrategy
{
    public:
        RenamingStrategy();
        ~RenamingStrategy();
    protected:

    private:

};

/*****************************
 *       Class Renamer       *
 *****************************/
Renamer::Renamer(Resource* resource, const string& origSymbol, const string& newSymbol)
    : _resource(resource), _origSymbol(origSymbol), _newSymbol(newSymbol), m_type(I_All)
{
    analyze();
}


//
Renamer::Renamer(Resource* resource, const Location& loc, const string& newSymbol)
    : _resource(resource), _newSymbol(newSymbol), m_type(I_All)
{
    (void) resource;
    (void) loc;
    (void) newSymbol;
    ERROR("Not yet imeplemented.")
}


//
Renamer::~Renamer()
{

}


//
int Renamer::analyze()
{
    // ensure compilation DB is present
    string buildPath = "./";            // TODO (nilbeleth#1#): porozmyslaj nad dakou dynamickou cestou k buildPath
    if( File::exists(buildPath + COMPILE_DB_FILE) )
        WARNING("Compilation DB already present... overwriting.")
    _resource->generateJSONDatabase();

    //
    string errMsg;
    llvm::OwningPtr<CompilationDatabase> compilations(CompilationDatabase::loadFromDirectory(buildPath, errMsg));
    if( !compilations )
        WARNING(errMsg);

    RefactoringTool tool(*compilations, _resource->getSources());

    //
    RenamingActionFactory finder(this);
    return tool.run(newFrontendActionFactory(&finder));
}


//
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
