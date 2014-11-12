#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>


namespace refactor
{


/**
 * Add escape sequances where it's needed.
 */
std::string escapeString(const std::string& str);


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
        static std::string identifyStmt(const clang::Stmt* stmt);
    protected:
    private:
};

/**
 * Generic template to print all enum class types as integers.
 */
template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

}   // end namespace refactor
#endif // UTILS_H
