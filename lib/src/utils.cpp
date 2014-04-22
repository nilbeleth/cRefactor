#include "utils.h"

#include <sstream>


using namespace std;
using namespace clang;
using namespace refactor;



Utils::Utils()
{
    //ctor
}

Utils::~Utils()
{
    //dtor
}

string Utils::printDecl(const NamedDecl* decl, const ASTContext* astContext)
{
    stringstream ss;
    SourceLocation b = decl->getLocation();

    ss << decl->getNameAsString();
    if( astContext != NULL )
        ss << ": " << b.printToString(astContext->getSourceManager()) << endl;
    else
        ss << endl;

    ss << "\tFileID:        " << (b.isFileID() ? "yes" : "no") << endl;
    ss << "\tMacroID:       " << (b.isMacroID() ? "yes" : "no") << endl;
    ss << "\tLinkage:       " << (decl->hasLinkage() ? "yes" : "no") << endl;
    ss << "\tHiden:         " << (decl->isHidden() ? "yes" : "no") << endl;
    ss << "\tAttr:          " << (decl->hasAttrs() ? "yes" : "no") << endl;
    ss << "\tFrom file:     " << (decl->isFromASTFile() ? "yes" : "no") << endl;
    ss << "\tSystem header: " << (astContext->getSourceManager().isInSystemHeader(b) ? "yes" : "no") << endl;
    ss << "\tType:          " << identifyDecl(decl) << endl;

    return ss.str();
}


string Utils::identifyDecl(const Decl* decl)
{
    stringstream ss;


    if( isa<AccessSpecDecl>(decl) )
        ss << "AccessSpecDecl" << endl;
    else if( isa<BlockDecl>(decl) )
        ss << "BlockDecl" << endl;
    else if( isa<NamedDecl>(decl) )
    {
        ss << "NamedDecl::";

        if( isa<LabelDecl>(decl) )
            ss << "LabelDecl";
        else if( isa<NamespaceAliasDecl>(decl) )
            ss << "NamespaceAliasDecl";
        else if( isa<NamespaceDecl>(decl) )
            ss << "NamespaceDecl";
        else if( isa<ObjCCompatibleAliasDecl>(decl) )
            ss << "ObjCCompatibleAliasDecl";
        else if( isa<ObjCContainerDecl>(decl) )
        {
            ss << "ObjCContainerDecl::";

            if(isa<ObjCCategoryDecl>(decl))
                ss << "ObjCCategory";
            else if(isa<ObjCImplDecl>(decl))
            {
                ss << "ObjCCImplDecl::";

                if(isa<ObjCCategoryImplDecl>(decl))
                    ss << "ObjCCategoryImplDecl";
                else if( isa<ObjCImplementationDecl>(decl))
                    ss << "ObjCImplementationDecl";
                else
                    ss << "<Unknown declaration>";
            }
            else if(isa<ObjCInterfaceDecl>(decl))
                ss << "ObjCInterfaceDecl";
            else if(isa<ObjCProtocolDecl>(decl))
                ss << "ObjCProtocolDecl";
            else
                ss << "<Unknown declaration>";
        }
        else if( isa<ObjCMethodDecl>(decl) )
            ss << "ObjCMethodDecl";
        else if( isa<ObjCPropertyDecl>(decl) )
            ss << "ObjCPropertyDecl";
        else if( isa<TemplateDecl>(decl) )
        {
            ss << "TemplateDecl::";

            if(isa<RedeclarableTemplateDecl>(decl))
            {
                ss << "RedeclarableTemplateDecl::";

                if(isa<ClassTemplateDecl>(decl))
                    ss << "ClassTemplateDecl";
                else if(isa<FunctionTemplateDecl>(decl))
                    ss << "FunctionTemplateDecl";
                else if(isa<TypeAliasTemplateDecl>(decl))
                    ss << "TypeAliasTemplateDecl";
//                else if(isa<VarTemplateDecl>(decl))
//                    ss << "VarTemplateDecl";
                else
                    ss << "<Unknown declaration>";
            }
            else if(isa<TemplateTemplateParmDecl>(decl))
                ss << "TemplateTemplateParmDecl";
            else
                ss << "<Unknown declaration>";
        }
        else if( isa<TypeDecl>(decl) )
        {
            ss << "TypeDecl::";

            if(isa<TagDecl>(decl))
            {
                ss << "TagDecl::";

                if(isa<EnumDecl>(decl))
                    ss << "EnumDecl";
                else if(isa<RecordDecl>(decl))
                {
                    ss << "RecordDecl::";

                    if(isa<CXXRecordDecl>(decl))
                    {
                        ss << "CXXRecordDecl::";

                        if(isa<ClassTemplateSpecializationDecl>(decl))
                        {
                            ss << "ClassTemplateSpecializationDecl::";

                            if(isa<ClassTemplatePartialSpecializationDecl>(decl))
                                ss << "ClassTemplatePartialSpecializationDecl";
                            else
                                ss << "<Unknown declaration>";
                        }
                        else
                            ss << "<Unknown declaration>";
                    }
                    else
                        ss << "<Unknown declaration>";
                }
                else
                    ss << "<Unknown declaration>";
            }
            else if(isa<TemplateTypeParmDecl>(decl))
                ss << "TemplateTypeParmDecl";
            else if(isa<TypedefNameDecl>(decl))
            {
                ss << "TypedefNameDecl::";

                if(isa<TypeAliasDecl>(decl))
                    ss << "TypeAliasDecl";
                else if(isa<TypedefDecl>(decl))
                    ss << "TypedefDecl";
                else
                    ss << "<Unknown declaration>";
            }
            else if(isa<UnresolvedUsingTypenameDecl>(decl))
                ss << "UnresolvedUsingTypenameDecl";
            else
                ss << "<Unknown declaration>";
        }
        else if( isa<UsingDecl>(decl) )
            ss << "UsingDecl";
        else if( isa<UsingDirectiveDecl>(decl) )
            ss << "UsingDirectiveDecl";
        else if( isa<UsingShadowDecl>(decl) )
            ss << "UsingShadowDecl";
        else if( isa<ValueDecl>(decl) )
        {
            ss << "ValueDecl::";

            if( isa<DeclaratorDecl>(decl) )
            {
                ss << "DeclaratorDecl::";

                if( isa<FieldDecl>(decl) )
                {
                    ss << "FieldDecl::";

                    if(isa<ObjCAtDefsFieldDecl>(decl))
                        ss << "ObjCAtDefsFieldDecl";
                    else if(isa<ObjCIvarDecl>(decl))
                        ss << "ObjCIvarDecl";
                    else
                        ss << "<Unknown declaration>";
                }
                else if(isa<FunctionDecl>(decl))
                {
                    ss << "FunctionDecl::";

                    if(isa<CXXMethodDecl>(decl))
                    {
                        ss << "CXXMethodDecl::";

                        if(isa<CXXConstructorDecl>(decl))
                            ss << "CXXConstructorDecl";
                        else if(isa<CXXConversionDecl>(decl))
                            ss << "CXXConversionDecl";
                        else if(isa<CXXDestructorDecl>(decl))
                            ss << "CXXDestructorDecl";
                        else
                            ss << "<Unknown declaration>";
                    }
                    else
                        ss << "<Unknown declaration>";
                }
                else if(isa<MSPropertyDecl>(decl))
                    ss << "MSPropertyDecl";
                else if(isa<NonTypeTemplateParmDecl>(decl))
                    ss << "NonTypeTemplateParmDecl";
                else if(isa<VarDecl>(decl))
                {
                    ss << "VarDecl::";

                    if(isa<ImplicitParamDecl>(decl))
                        ss << "ImplicitParamDecl";
                    else if(isa<ParmVarDecl>(decl))
                        ss << "ParmVarDecl";
                    else
                        ss << "<Unknown declaration>";
                }
                else
                    ss << "<Unknown declaration>";
            }
            else if(isa<EnumConstantDecl>(decl))
                ss << "EnumConstantDecl";
            else if(isa<IndirectFieldDecl>(decl))
                ss << "IndirectFieldDecl";
            else if( isa<UnresolvedUsingValueDecl>(decl))
                ss << "UnresolvedUsingValueDecl";
            else
                ss << "<Unknown declaration>";
        }
        else
            ss << "<Unknown declaration>";

        ss << endl;
    }
    else
        ss << "<Unknown declaration>" << endl;

    return ss.str();
}
