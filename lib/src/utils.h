#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <clang/AST/Decl.h>
#include <clang/AST/ASTContext.h>


namespace refactor
{


/**
 * Add escape sequances where it's needed.
 */
std::string escapeString(const std::string& str)
{
    return str;
}


/**
 * @brief Helper class.
 *
 */
class Utils
{
    public:
        /** Default constructor */
        Utils();
        /** Default destructor */
        virtual ~Utils();

        static std::string printDecl(const clang::NamedDecl* decl, const clang::ASTContext* astContext);
        static std::string identifyDecl(const clang::Decl* decl);
    protected:
    private:
};

}   // end namespace refactor
#endif // UTILS_H
