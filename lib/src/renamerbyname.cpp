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
            : _astContext(nullptr), _renamer(renamer)
        { }

        // set new context and traverse AST
        virtual void HandleTranslationUnit(ASTContext &context);

        // check any named declaration node
        virtual bool VisitNamedDecl(NamedDecl* decl);

        //
        virtual bool VisitDeclaratorDecl(DeclaratorDecl* decl);

        //
        virtual bool VisitVarDecl(VarDecl* decl);

        //
        virtual bool VisitDeclRefExpr(DeclRefExpr* expr);

        //
        virtual bool VisitMemberExpr(MemberExpr* expr);

    protected:
    private:
        ASTContext* _astContext;
        //SourceManager* _SM;
        RenamerByName* _renamer;

        // true if loc is in project files (has valid location
        // and is not in system libraries)
        inline bool inValidLoc(const SourceLocation loc);

        inline bool checkTypeIdent(const string decl, const string name);

        bool checkQualifier(NestedNameSpecifier* qualifier, NestedNameSpecifierLoc qLoc);
};

//
void RenamingMutator::HandleTranslationUnit(ASTContext& context)
{
    _astContext = &context;

    TranslationUnitDecl* D = context.getTranslationUnitDecl();
    TraverseDecl(D);

    _astContext = nullptr;
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

    Location location(_astContext->getSourceManager(),loc.getLocWithOffset(offset));
    refactor::Replacements& replacements = _renamer->getChanges();
    replacements.push_back(refactor::Replacement(location, name.size()-offset, _renamer->getNewSymbol()));

    //cout << "Declaration of (" << _renamer->getOldSymbol() << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;

    return true;
}

// check only for qualifiers in this
bool RenamingMutator::VisitDeclaratorDecl(DeclaratorDecl* decl)
{
    NestedNameSpecifier* qualifier = decl->getQualifier();
    NestedNameSpecifierLoc loc = decl->getQualifierLoc();

    if( qualifier )
        checkQualifier(qualifier, loc);

    return true;
}

bool RenamingMutator::VisitVarDecl(VarDecl* decl)
{
    QualType typeQT = decl->getType();
    SourceLocation loc = decl->getTypeSpecStartLoc();


    if( !inValidLoc(loc) )
        return true;

    string name = typeQT.getAsString();

    // is this type we are looking for?
    if( !(_renamer->getRestrictType() & _renamer->identify(decl)) )
        return true;

    // Type
    if( checkTypeIdent(name, _renamer->getOldSymbol()) )
    {
        Location location(_astContext->getSourceManager(), loc);
        refactor::Replacements& replacements = _renamer->getChanges();
        replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

        //cout << "Type usage of (" << name << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
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
            checkQualifier(expr->getQualifier(), expr->getQualifierLoc());
        }

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

        // is this type we are looking for?
        if( !(_renamer->getRestrictType() & _renamer->identify(decl)) )
            return true;

        Location location(_astContext->getSourceManager(), loc.getLocWithOffset(offset));
        refactor::Replacements& replacements = _renamer->getChanges();
        replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

        //cout << "Reference of (" << name << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
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


        if( isa<CXXDestructorDecl>(decl) )
        {
            if( oldSymbol != name.substr(1) )
                return true;

            offset = 1;
        }
        else
            if( oldSymbol != name )
                return true;

        // is this type we are looking for?
        if( !(_renamer->getRestrictType() & _renamer->identify(decl)) )
            return true;

        Location location(_astContext->getSourceManager(), loc.getLocWithOffset(offset));
        refactor::Replacements& replacements = _renamer->getChanges();
        replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

        //cout << "Reference of (" << name << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
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

bool RenamingMutator::checkQualifier(NestedNameSpecifier* qualifier, NestedNameSpecifierLoc qLoc)
{
    if( !qualifier )
        return false;

    SourceLocation loc = qLoc.getLocalBeginLoc();

    switch( qualifier->getKind() )
    {
        case NestedNameSpecifier::Identifier:
            DEBUG("Option \'NestedNameSpecifier::NamespaceAlias\' not supported.")
            break;
        case NestedNameSpecifier::Namespace:
        {
            // este by sa mal cyklicli dako ziskavat prefix pokial ide o namespacy vnorene do seba
            NamespaceDecl* namespaceDecl = qualifier->getAsNamespace();

            if( !(_renamer->getRestrictType() & IdentType::I_Namespace) )
                break;

            if( namespaceDecl->getNameAsString() == _renamer->getOldSymbol() )
            {
                Location location(_astContext->getSourceManager(), loc);
                refactor::Replacements& replacements = _renamer->getChanges();
                replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

                //cout << "Qualifier of (" << namespaceDecl->getNameAsString() << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
            }
            break;
        }
        case NestedNameSpecifier::NamespaceAlias:
            // namespace aliases... who use that...
            DEBUG("Option \'NestedNameSpecifier::NamespaceAlias\' not supported.")
            break;
        case NestedNameSpecifier::TypeSpec:
        {
            const Type* type = qualifier->getAsType();
            CXXRecordDecl* record = type->getAsCXXRecordDecl();     // TODO (nilbeleth#1#): ziskat ako RecordDecl a nie  CXX
            if( record )
            {
                string recordName = record->getNameAsString();

                // is this type we are looking for?
                if( !(_renamer->getRestrictType() & _renamer->identify(record)) )
                    return true;

                if( recordName == _renamer->getOldSymbol() )
                {
                    Location location(_astContext->getSourceManager(), loc);
                    refactor::Replacements& replacements = _renamer->getChanges();
                    replacements.push_back(refactor::Replacement(location, _renamer->getOldSymbol().size(), _renamer->getNewSymbol()));

                    //cout << "Qualifier of (" << recordName << ") at " << loc.printToString(_astContext->getSourceManager()) << endl;
                }
            }
            break;
        }
        case NestedNameSpecifier::TypeSpecWithTemplate:
            // templates sucks... so far
            DEBUG("Option \'NestedNameSpecifier::TypeSpecWithTemplate\' not supported.")
            break;
        case NestedNameSpecifier::Global:
            // just ignore this case - '::' is not interesting
            break;
        default:
            WARNING("Unrecognized option: check doxy for new types.")
            break;
    }
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
    : RenamingStrategy(resource, newSymbol, IdentType::I_All, ""), m_old(origSymbol)
{
}

RenamerByName::~RenamerByName()
{

}

int RenamerByName::analyze()
{
    // ensure compilation DB is present
    string buildPath = "./";            // TODO (nilbeleth#1#): porozmyslaj nad dakou dynamickou cestou k buildPath
    if( File::exists(buildPath + COMPILE_DB_FILE) )
    {
        if( COMPILE_DB_OVERWRITE )
        {
            WARNING("Compilation DB already present... overwriting.")
            m_resource->generateJSONDatabase();
        }
    }
    else
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
