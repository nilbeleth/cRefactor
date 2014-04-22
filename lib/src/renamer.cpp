#include "renamer.h"
#include "debug.h"

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
using namespace clang::driver;
using namespace clang::tooling;
using namespace refactor;


// TODO (nilbeleth#1#): to predavanie by nemalo ist cez globalne premenne, co ked budeme mat viac instancii naraz
string oldSymbol;
string newSymbol;



/*****************************
 *       Class Mutator       *
 *****************************/
class RenamingMutator : public ASTConsumer, public RecursiveASTVisitor<RenamingMutator>
{
    public:
        explicit RenamingMutator()
            : _astContext(NULL)
        { }

        virtual void HandleTranslationUnit(ASTContext &context)
        {
            _astContext = &context;

            TranslationUnitDecl *D = context.getTranslationUnitDecl();
            // Run Recursive AST Visitor
            TraverseDecl(D);

            _astContext = NULL;
        }

        virtual bool VisitNamedDecl(NamedDecl* decl)
        {
            string name = decl->getNameAsString();

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

        virtual bool VisitDeclRefExpr(DeclRefExpr* expr)
        {
            // is this somewhere declared?
            NamedDecl* decl = expr->getFoundDecl();
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
};



/**********************************
 *   Class FrontendActionFactory  *
 **********************************/
class RenamingActionFactory
{
    public:
        virtual clang::ASTConsumer *newASTConsumer()
        {
            return new RenamingMutator();
        }
};



/*****************************
 *       Class Renamer       *
 *****************************/
Renamer::Renamer(Resource* resource, const string& origSymbol, const string& newSymbol)
    : _resource(resource), _origSymbol(origSymbol), _newSymbol(newSymbol)
{
    analyze();
}


//
Renamer::Renamer(Resource* resource, const Location& loc, const string& newSymbol)
    : _resource(resource), _newSymbol(newSymbol)
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
    // set global variables
    // TODO (nilbeleth#1#): lepsie riesenie ako zdielat tieto nazvy
    oldSymbol = _origSymbol;
    newSymbol = _newSymbol;

    //
    int argc = 3;
    const char* argv[] = { "app", "test.cpp", "--"};
    RenamingActionFactory factory;


    // create new DB file
    _resource->generateJSONDatabase();

    string buildDir = ".";
    string errMsg = "Databaza v riti.";
    CompilationDatabase* compilations(CompilationDatabase::loadFromDirectory(buildDir, errMsg));

    // TODO (nilbeleth#1#): daky lepsi system ako spustit analyzu
    // parse the command-line args passed to your code
    //CommonOptionsParser op(argc, argv);
    //string msg = "Blba databaza.";
    //llvm::OwningPtr<CompilationDatabase> compilations(CompilationDatabase::loadFromDirectory(".", msg));


    /*vector<CompileCommand> commands = op.getCompilations().getCompileCommands("test.cpp");

    for(int i = 0; i < commands.size();++i)
    {
        cout << "Directory: " << commands.at(i).Directory << endl;

        cout << "Command: ";
        for(int j = 0; j < commands.at(i).CommandLine.size(); ++j)
        {
            cout << commands.at(i).CommandLine.at(j) << " ";
        }
        cout << endl;
    }*/

    // create a new Clang Tool instance (a LibTooling environment)
    //ClangTool Tool(op.getCompilations(), op.getSourcePathList());
    RefactoringTool tool(*compilations, _resource->getSources());
    //vector<string> files;
    //files.push_back("test.cpp");
    //ClangTool Tool(*compilations.get(), files);


    // run the Clang Tool, creating a new FrontendAction (explained below)
    int result = tool.run(newFrontendActionFactory(&factory));


    return result;
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
