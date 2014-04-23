#ifndef RENAMER_H
#define RENAMER_H
#include "task.h"
#include "resource.h"
#include "location.h"
#include <string>


namespace refactor
{



/**
 * @brief Identifiers.
 *
 * Describe the types of identifiers.
 */
enum IdentType
{
    I_Variable,         /**< A name of variable. */
    I_ClassName,        /**< A class name. */
    I_StructName,       /**< A structure name. */
    I_UnionName,        /**< An union name. */
    I_EnumName,         /**< An enumeration name. */
    I_ClassMember,      /**< A class member variable. */
    I_StructMember,     /**< A struct member variable. */
    I_UnionMemeber,     /**< A union member variable. */
    I_EnumItem,         /**< An enumeration item. */
    I_FunctionName,     /**< Function. */
    I_MethodName,       /**< A class instance method.*/
    I_Typedef,          /**< A type definition. */
    I_Label,            /**< A label name. */
    I_MacroName,        /**< */
    I_MacroParm,        /**< */
    I_Namespace,        /**< Namespace identifier. */
    I_All
};


/**
 * @brief Rename a identifier.
 *
 *
 */
class Renamer : public Task
{
    public:
        /**
         * Construct a new refactoring task to rename a symbol.
         * @param resource      Gathers the source files on which should be the task performed.
         * @param origSymbol    The name of original symbol.
         * @param newSymbol     The new name for symbol.
         */
        Renamer(Resource* resource, const std::string& origSymbol, const std::string& newSymbol);


        /**
         * Construct a new refactoring task to rename a symbol.
         * @param resource
         * @param loc
         * @param newSymbol
         */
        Renamer(Resource* resource, const Location& loc, const std::string& newSymbol);

        /** Default destructor. */
        virtual ~Renamer();


        /**
         * Rename only identifier with given type.
         */
        void restrictTo(const IdentType type) { m_type = type; }


        /**
         * Process the possible changes to
         * all files done by refactoring task.
         * @note Error can occured also from parsing
         *       error.
         * @return  Returns 1 if processing fails, 0 otherwise.
         */
        virtual int analyze();

        /**
         * Get all possible replacements determined
         * by analysis.
         */
        virtual Replacements getChanges() const;

        /**
         * Apply a replacement if it's possible.
         * @param replace   A replacement to be apllied.
         * @return          Return 0 if the replacement
         *                  is successfully apllied, 1 otherwise.
         */
        virtual int applyChange(const Replacement& replace) const;

        /**
         * Apply all possible replacements.
         * @return          Return 0 if the replacement
         *                  is successfully apllied, 1 otherwise.
         */
        virtual int commit();


    protected:
    private:
        Resource* _resource;
        std::string _origSymbol;
        std::string _newSymbol;
        IdentType m_type;
};

}   // end namespace refactor
#endif // RENAMER_H
