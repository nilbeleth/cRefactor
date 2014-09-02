#include "renamingstrategy.h"
#include "debug.h"

#include <clang/AST/AST.h>
#include <iostream>


using namespace refactor;
using namespace clang;
using namespace std;



RenamingStrategy::RenamingStrategy(Resource* resource,
                                   const std::string newSymbol,
                                   const IdentType type,
                                   const std::string derivedFrom)
    : m_resource(resource), m_new(newSymbol), m_type(type), m_inFamilyWith(derivedFrom), m_replacements(vector<Replacement>())
{
}


IdentType RenamingStrategy::identify(const Decl* decl) const
{
    if( isa<VarDecl>(decl) )
    {
        if( isa<ParmVarDecl>(decl) )
            return IdentType::I_Argument;
        else
            return IdentType::I_Variable;
    }
    else if( isa<CXXRecordDecl>(decl) )
    {
        const CXXRecordDecl* recordDecl = cast<CXXRecordDecl>(decl);
        if( recordDecl->isClass() )
            return IdentType::I_ClassName;
        else if( recordDecl->isStruct() )
            return IdentType::I_StructName;
        else if( recordDecl->isUnion() )
            return IdentType::I_UnionName;
        else
            WARNING("Unable to distinguish between class, struct and union.");
    }
    else if( isa <EnumDecl>(decl) )
    {
        return IdentType::I_EnumName;
    }
    else if( isa<FieldDecl>(decl) )
    {
        const FieldDecl* fieldDecl = cast<FieldDecl>(decl);
        const RecordDecl* recordDecl = fieldDecl->getParent();

        if( recordDecl->isClass() )
            return IdentType::I_ClassMember;
        else if( recordDecl->isStruct() )
            return IdentType::I_StructMember;
        else if( recordDecl->isUnion() )
            return IdentType::I_UnionMemeber;
        else
            WARNING("Unable to distinguish between class, struct and union member.");
    }
    else if( isa<EnumConstantDecl>(decl) )
    {
        return IdentType::I_EnumItem;
    }
    else if( isa<FunctionDecl>(decl) )
    {
        if( isa<CXXMethodDecl>(decl) )
            return IdentType::I_MethodName;
        else
            return IdentType::I_FunctionName;
    }
    else if( isa<TypedefDecl>(decl) )
    {
        return IdentType::I_Typedef;
    }
    else if( isa<NamespaceDecl>(decl) )
    {
        return IdentType::I_Namespace;
    }
    else
    {
        WARNING("Unable to identify the type of identifier.")
    }

    return IdentType::I_Unknown;
}

IdentType RenamingStrategy::identify(const string& type) const
{
    if( type.find("class") != string::npos )
        return IdentType::I_ClassName;
    else if( type.find("struct") != string::npos )
        return IdentType::I_StructName;
    else if( type.find("union") != string::npos )
        return IdentType::I_UnionName;
    else if( type.find("enum") != string::npos )
        return IdentType::I_EnumName;
    else
        WARNING("Unable to identify this identifier (" << type << ").")


    return IdentType::I_Unknown;
}
