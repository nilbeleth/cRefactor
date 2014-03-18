#ifndef PREPROCESSORCONTEXT_H
#define PREPROCESSORCONTEXT_H
#include "resource.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/ASTConsumer.h"


namespace refactor
{


/**
 *
 */
class PreprocessorContext
{
    public:
        /** Default constructor */
        PreprocessorContext();
        /** Default destructor */
        virtual ~PreprocessorContext();

        void initialize();
        void parse(const Resource& resource, clang::ASTConsumer consumer);
    protected:
    private:
        clang::CompilerInstance *m_ci;
        //clang::Preprocessor *m_pp;
};

}   // end namespace refactor

#endif // PREPROCESSORCONTEXT_H
