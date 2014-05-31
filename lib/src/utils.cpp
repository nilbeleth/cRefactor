#include "utils.h"

#include <sstream>


using namespace std;
using namespace clang;
using namespace refactor;



std::string refactor::escapeString(const std::string& str)
{
    return str;
}

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
    if( astContext != nullptr )
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
    //ss << "\tType:          " << identifyDecl(decl) << endl;

    return ss.str();
}


string Utils::identifyDecl(const Decl* decl)
{
    stringstream ss;


    if( isa<BlockDecl>(decl) )
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
                else if(isa<VarTemplateDecl>(decl))
                    ss << "VarTemplateDecl";
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

string Utils::identifyStmt(const Stmt* stmt)
{
    stringstream ss;

    if( isa<AsmStmt>(stmt) )
    {
        ss << "AsmStmt::";

        if( isa<GCCAsmStmt>(stmt) )
            ss << "GCCAsmStmt";
        else if( isa<MSAsmStmt>(stmt) )
            ss << "MSAsmStmt";
        else
            ss << "<Unknown statement>";
    }
    else if( isa<AttributedStmt>(stmt) )
        ss << "AttributedStmt";
    else if( isa<BreakStmt>(stmt) )
        ss << "BreakStmt";
    else if( isa<CapturedStmt>(stmt) )
        ss << "CapturedStmt";
    else if( isa<CompoundStmt>(stmt) )
        ss << "CompoundStmt";
    else if( isa<ContinueStmt>(stmt) )
        ss << "ContinueStmt";
    else if( isa<CXXCatchStmt>(stmt) )
        ss << "CXXCatchStmt";
    else if( isa<CXXForRangeStmt>(stmt) )
        ss << "CXXForRangeStmt";
    else if( isa<CXXTryStmt>(stmt) )
        ss << "CXXTryStmt";
    else if( isa<DeclStmt>(stmt) )
        ss << "DeclStmt";
    else if( isa<DoStmt>(stmt) )
        ss << "DoStmt";
    else if( isa<Expr>(stmt) )
    {
        ss << "Expr::";

        if( isa<AbstractConditionalOperator>(stmt) )
        {
            ss << "AbstractConditionalOperator::";

            if( isa<BinaryConditionalOperator>(stmt) )
                ss << "BinaryConditionalOperator";
            else if( isa<ConditionalOperator>(stmt) )
                ss << "ConditionalOperator";
            else
                ss << "<Unknown statement>";
        }
        else if( isa<AddrLabelExpr>(stmt) )
            ss << "AddrLabelExpr";
        else if( isa<ArraySubscriptExpr>(stmt) )
            ss << "ArraySubscriptExpr";
        else if( isa<ArrayTypeTraitExpr>(stmt) )
            ss << "ArrayTypeTraitExpr";
        else if( isa<AsTypeExpr>(stmt) )
            ss << "AsTypeExpr";
        else if( isa<AtomicExpr>(stmt) )
            ss << "AtomicExpr";
        else if( isa<BinaryOperator>(stmt) )
        {
            ss << "BinaryOperator::";

            if( isa<CompoundStmt>(stmt) )
                ss << "CompoundStmt";
            else
                ss << "<Unknown statement>";
        }
        else if( isa<BlockExpr>(stmt) )
            ss << "BlockExpr";
        else if( isa<CallExpr>(stmt) )
        {
            ss << "CallExpr::";

            if( isa<CUDAKernelCallExpr>(stmt) )
                ss << "CUDAKernelCallExpr";
            else if( isa<CXXMemberCallExpr>(stmt) )
                ss << "CXXMemberCallExpr";
            else if( isa<CXXOperatorCallExpr>(stmt) )
                ss << "CXXOperatorCallExpr";
            else if( isa<UserDefinedLiteral>(stmt) )
                ss << "UserDefinedLiteral";
            else
                ss << "<Unknown statement>";
        }
        else if( isa<CastExpr>(stmt) )
        {
            ss << "CastExpr::";

            if( isa<ExplicitCastExpr>(stmt) )
            {
                ss << "ExplicitCastExpr::";

                if( isa<CStyleCastExpr>(stmt) )
                    ss << "CStyleCastExpr";
                else if( isa<CXXFunctionalCastExpr>(stmt) )
                    ss << "CXXFunctionalCastExpr";
                else if( isa<CXXNamedCastExpr>(stmt) )
                {
                    ss << "CXXNamedCastExpr::";

                    if( isa<CXXConstCastExpr>(stmt) )
                        ss << "CXXConstCastExpr";
                    else if( isa<CXXDynamicCastExpr>(stmt) )
                        ss << "CXXDynamicCastExpr";
                    else if( isa<CXXReinterpretCastExpr>(stmt) )
                        ss << "CXXReinterpretCastExpr";
                    else if( isa<CXXStaticCastExpr>(stmt) )
                        ss << "CXXStaticCastExpr";
                    else
                        ss << "";
                }
                else if( isa<ObjCBridgedCastExpr>(stmt) )
                    ss << "ObjCBridgedCastExpr";
                else
                    ss << "<Unknown statement>";
            }
            else if( isa<ImplicitCastExpr>(stmt) )
                ss << "ImplicitCastExpr";
            else
                ss << "<Unknown statement>";
        }
        else if( isa<CharacterLiteral>(stmt) )
            ss << "CharacterLiteral";
        else if( isa<ChooseExpr>(stmt) )
            ss << "ChooseExpr";
        else if( isa<CXXDependentScopeMemberExpr>(stmt) )
            ss << "CXXDependentScopeMemberExpr";
        else if( isa<CXXThisExpr>(stmt) )
            ss << "CXXThisExpr";
        else if( isa<MemberExpr>(stmt) )
            ss << "MemberExpr";
        else if( isa<OverloadExpr>(stmt) )
        {
            ss << "OverloadExpr::";

            if( isa<UnresolvedLookupExpr>(stmt) )
                ss << "UnresolvedLookupExpr";
            else if( isa<UnresolvedMemberExpr>(stmt) )
                ss << "UnresolvedMemberExpr";
            else
                ss << "<Unknown statement>";
        }
        else if( isa<ParenExpr>(stmt) )
            ss << "ParenExpr";
        else if( isa<ParenListExpr>(stmt) )
            ss << "ParenListExpr";
        else if( isa<StringLiteral>(stmt) )
            ss << "StringLiteral";
        else
            ss << "<Unkown statement>";
    }
    else if( isa<ForStmt>(stmt) )
        ss << "ForStmt";
    else if( isa<GotoStmt>(stmt) )
        ss << "GotoStmt";
    else if( isa<IfStmt>(stmt) )
        ss << "IfStmt";
    else if( isa<IndirectGotoStmt>(stmt) )
        ss << "IndirectGotoStmt";
    else if( isa<LabelStmt>(stmt) )
        ss << "LabelStmt";
    else if( isa<MSDependentExistsStmt>(stmt) )
        ss << "MSDependentExistsStmt";
    else if( isa<NullStmt>(stmt) )
        ss << "NullStmt";
    else if( isa<ObjCAtCatchStmt>(stmt) )
        ss << "ObjCAtCatchStmt";
    else if( isa<ObjCAtFinallyStmt>(stmt) )
        ss << "ObjCAtFinallyStmt";
    else if( isa<ObjCAtSynchronizedStmt>(stmt) )
        ss << "ObjCAtSynchronizedStmt";
    else if( isa<ObjCAtThrowStmt>(stmt) )
        ss << "ObjCAtThrowStmt";
    else if( isa<ObjCAtTryStmt>(stmt) )
        ss << "ObjCAtTryStmt";
    else if( isa<ObjCAutoreleasePoolStmt>(stmt) )
        ss << "ObjCAutoreleasePoolStmt";
    else if( isa<ObjCForCollectionStmt>(stmt) )
        ss << "ObjCForCollectionStmt";
    else if( isa<OMPExecutableDirective>(stmt) )
        ss << "OMPExecutableDirective";
    else if( isa<ReturnStmt>(stmt) )
        ss << "ReturnStmt";
    else if( isa<SEHExceptStmt>(stmt) )
        ss << "SEHExceptionStmt";
    else if( isa<SEHExceptStmt>(stmt) )
        ss << "SEHExceptStmt";
    else if( isa<SEHFinallyStmt>(stmt) )
        ss << "SEHFinallyStmt";
    else if( isa<SEHTryStmt>(stmt) )
        ss << "SEHTryStmt";
    else if( isa<SwitchCase>(stmt) )
        ss << "SwitchCase";
    else if( isa<SwitchStmt>(stmt) )
        ss << "SwitchStmt";
    else if( isa<WhileStmt>(stmt) )
        ss << "WhileStmt";
    else
        ss << "<Unknown statement>";

    ss << endl;
    return ss.str();
}
