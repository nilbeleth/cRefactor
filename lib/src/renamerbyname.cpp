#include "renamingstrategy.h"
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

#include <iostream>
#include <sstream>


using namespace std;
using namespace clang;
using namespace clang::tooling;
using namespace refactor;



/*****************************
 *       Class Mutator       *
 *****************************/
// find all occurencies and remeber them
class RenamingMutator : public ASTConsumer, public RecursiveASTVisitor<RenamingMutator>
{
    public:
        explicit RenamingMutator(RenamerByName* renamer)
            : _astContext(NULL), _SM(NULL), _renamer(renamer)
        { }

        // set new context and traverse AST
        virtual void HandleTranslationUnit(ASTContext &context);

        // check any named declaration node
        virtual bool VisitNamedDecl(NamedDecl* decl);

        //
        virtual bool VisitVarDecl(VarDecl* decl);

        //
        virtual bool VisitDeclRefExpr(DeclRefExpr* expr);

        virtual bool VisitMemberExpr(MemberExpr* expr);

        virtual bool VisitCXXRecordDecl(CXXRecordDecl* decl)
        {
            if( "Class" == decl->getNameAsString() )
            {

            }
            return true;
        }

    protected:
    private:
        ASTContext* _astContext;
        SourceManager* _SM;
        RenamerByName* _renamer;

        // true if loc is in project files (has valid location
        // and is not in system libraries)
        inline bool inValidLoc(const SourceLocation loc);

        inline bool checkTypeIdent(const string decl, const string name);
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
    int offset = 0;

    // search only project files
    if( !inValidLoc(loc) )
        return true;


    //cout << name << ": (from NamedDecl)\n";
    //cout << "\tAt:\t\t" << loc.printToString(_astContext->getSourceManager()) << endl;
    //cout << "\tIs:\t\t" << Utils::identifyDecl(decl);
    //cout << "\t\t\t"; _renamer->identify(decl); cout << endl << endl;


    // destructor has a ~ before, so we need to know if it's a destructor or not
    if( isa<CXXDestructorDecl>(decl) )
    {
        if( _renamer->getOldSymbol().compare(name.substr(1)) != 0 )
            return true;

        // aplikuj offset
        ++offset;
    }
    // it's not a destructor, check for name
    else if( _renamer->getOldSymbol() != name )
    {
        return true;
    }

    // is this type we are looking for?
    if( !(_renamer->getRestrictType() & _renamer->identify(decl)) )
        return true;

    Location location = Location::getAsThisLocation(_astContext->getSourceManager(),loc.getLocWithOffset(offset));
    refactor::Replacements& replacements = _renamer->getChanges();
    replacements.push_back(refactor::Replacement(location, name.size()-offset, _renamer->getNewSymbol()));

    cout << "Declaration of (" << _renamer->getOldSymbol() << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;

    return true;
}

bool RenamingMutator::VisitVarDecl(VarDecl* decl)
{
    QualType typeQT = decl->getType();
    const Type* type = typeQT.getTypePtrOrNull();
    SourceLocation loc = decl->getTypeSpecStartLoc();


    if( !inValidLoc(loc) )
        return true;

    string name = typeQT.getAsString();

    //cout << name << ": (from VarDecl)\n";
    //cout << "\tAt:\t\t" << loc.printToString(_astContext->getSourceManager()) << endl;
    //cout << "\tIs:\t\t" << Utils::identifyDecl(decl);
    //cout << "\t\t\t"; _renamer->identify(name); cout << endl << endl;
    //if( type && !type->isBuiltinType() )
        ;//cout << "Of type: " << type->getTypeClassName() << endl;

    // is this type we are looking for?
    if( !(_renamer->getRestrictType() & _renamer->identify(decl)) )
        return true;

    // Type
    if( checkTypeIdent(name, _renamer->getOldSymbol()) )
    {
        Location location = Location::getAsThisLocation(_astContext->getSourceManager(), loc);
        refactor::Replacements& replacements = _renamer->getChanges();
        replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

        cout << "Type usage of (" << name << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
    }

    return true;
}

bool RenamingMutator::VisitDeclRefExpr(DeclRefExpr* expr)
{
    // is this somewhere declared?
    NamedDecl* decl = expr->getFoundDecl();
    string oldSymbol = _renamer->getOldSymbol();
    int offset = 0;

    if( decl )
    {
        string name = expr->getNameInfo().getAsString();
        SourceLocation loc = expr->getLocation();

        if( !inValidLoc(loc) )
            return true;

        // the qualifier could be the thing we are looking for
        if( expr->hasQualifier() )
        {
            NestedNameSpecifier* qualifier = expr->getQualifier();
            SourceLocation nLoc = expr->getQualifierLoc().getLocalBeginLoc();

            switch( qualifier->getKind() )
            {
                case NestedNameSpecifier::Namespace:
                {
                    // este by sa mal cyklicli dako ziskavat prefix pokial ide o namespacy vnorene do seba
                    NamespaceDecl* namespaceDecl = qualifier->getAsNamespace();

                    //cout << namespaceDecl->getNameAsString() << ": (from DeclRefExpr - namespace)\n";
                    //cout << "\tAt:\t\t" << nLoc1.printToString(_astContext->getSourceManager()) << endl;
                    //cout << "\tAt:\t\t" << nLoc2.printToString(_astContext->getSourceManager()) << endl;
                    //cout << "\tIs:\t\t" << Utils::identifyDecl(decl);
                    //cout << "\t\t\tNamespace" << endl << endl;

                    if( namespaceDecl->getNameAsString() == _renamer->getOldSymbol() )
                    {
                        Location location = Location::getAsThisLocation(_astContext->getSourceManager(), nLoc);
                        refactor::Replacements& replacements = _renamer->getChanges();
                        replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

                        cout << "Qualifier of (" << namespaceDecl->getNameAsString() << ") at " << nLoc.printToString(_astContext->getSourceManager()) << endl;
                    }
                    break;
                }

                case NestedNameSpecifier::NamespaceAlias:
                {
                    //NamespaceAliasDecl* aliasDecl = qualifier->getAsAliasNamespace();
                    //cout << "For (" << name << ") is identifier: " << aliasDecl->getNameAsString() << endl;
                    break;
                }

                case NestedNameSpecifier::TypeSpec:
                {
                    //Type* type qualifier->getAsType();
                    // TODO (nilbeleth#1#): tuna by som si mal ziskat triedu z Class::methond()
                    break;
                }
            }

            if( qualifier->getAsType() && !qualifier->getAsType()->isBuiltinType() )
                ;//cout << "Of type: " << qualifier->getAsType()->getTypeClassName() << endl;
        }

        //cout << name << ": (from DeclRefExpr)\n";
        //cout << "\tAt:\t\t" << loc.printToString(_astContext->getSourceManager()) << endl;
        //cout << "\tIs:\t\t" << Utils::identifyDecl(decl);
        //cout << "\t\t\t"; _renamer->identify(decl); cout << endl << endl;

        // does the name match?
        if( CXXDestructorDecl::classof(decl) )
        {
            if( string("~" + oldSymbol) != name )
                return true;

            offset = 1;
        }
        else
            if( oldSymbol != name )
                return true;

        Location location = Location::getAsThisLocation(_astContext->getSourceManager(), loc.getLocWithOffset(offset));
        refactor::Replacements& replacements = _renamer->getChanges();
        replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

        cout << "Reference of (" << name << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
    }

    return true;
}

bool RenamingMutator::VisitMemberExpr(MemberExpr* expr)
{
    // is this somewhere declared?
    NamedDecl* decl = expr->getFoundDecl();
    string oldSymbol = _renamer->getOldSymbol();
    int offset = 0;

    if( decl )
    {
        SourceLocation loc = expr->getExprLoc();
        string name = decl->getNameAsString();

        if(!inValidLoc(loc))
            return true;

        //cout << name << ": (from MemberExpr)\n";
        //cout << "\tAt:\t\t" << loc.printToString(_astContext->getSourceManager()) << endl;
        //cout << "\tIs:\t\t" << Utils::identifyDecl(decl);
        //cout << "\t\t\t"; _renamer->identify(decl); cout << endl << endl;

        if( isa<CXXDestructorDecl>(decl) )
        {
            if( oldSymbol != name.substr(1) )
                return true;

            offset = 1;
        }
        else
            if( oldSymbol != name )
                return true;

        Location location = Location::getAsThisLocation(_astContext->getSourceManager(), loc.getLocWithOffset(offset));
        refactor::Replacements& replacements = _renamer->getChanges();
        replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

        cout << "Reference of (" << name << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
    }

    return true;
}

bool RenamingMutator::inValidLoc(const SourceLocation loc)
{
    if( loc.isInvalid() )
        return false;

    if( _astContext->getSourceManager().isInSystemHeader(loc) )
        return false;

    return true;
}

bool RenamingMutator::checkTypeIdent(const string decl, const string name)
{
    size_t pos = decl.find(name);

    // not at all there
    if( string::npos == pos )
        return false;

    // some prefix from const, struct and others
    if( 0 < pos && decl.at(pos-1) != ' ' )
        return false;

    // subfix is present, right after type name there must be space
    if( pos + name.size() < decl.size() && decl.at(pos+name.size()) != ' ' )
        return false;

    return true;
}



/**********************************
 *   Class FrontendActionFactory  *
 **********************************/
// do nothing more than return a correct Visitor
class RenamingActionFactory
{
    RenamerByName* _renamer;

    public:
        RenamingActionFactory(RenamerByName* renamer)
            : _renamer(renamer)
        { }

        virtual clang::ASTConsumer *newASTConsumer()
        {
            return new RenamingMutator(_renamer);
        }
};



/**********************************
 *       Class RenamerByName      *
 **********************************/
RenamerByName::RenamerByName(Resource* resource, const string& origSymbol, const string& newSymbol)
    : RenamingStrategy(resource, newSymbol, I_All, ""), m_old(origSymbol)
{
}

int RenamerByName::analyze()
{
    // ensure compilation DB is present
    string buildPath = "./";            // TODO (nilbeleth#1#): porozmyslaj nad dakou dynamickou cestou k buildPath
    if( File::exists(buildPath + COMPILE_DB_FILE) )
        WARNING("Compilation DB already present... overwriting.")
    m_resource->generateJSONDatabase();


    // and parse it
    string errMsg;
    llvm::OwningPtr<CompilationDatabase> compilations(CompilationDatabase::loadFromDirectory(buildPath, errMsg));
    if( !compilations )
    {
        WARNING(errMsg);
        return 1;
    }


    RefactoringTool tool(*compilations, getResource()->getSources());
    RenamingActionFactory finder(this);
    return tool.run(newFrontendActionFactory(&finder));
}
