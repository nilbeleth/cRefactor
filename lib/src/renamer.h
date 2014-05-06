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
    I_Variable      =    1,     /**< A name of variable. */
    I_ClassName     =    2,     /**< A class name. */
    I_StructName    =    4,     /**< A structure name. */
    I_UnionName     =    8,     /**< An union name. */
    I_EnumName      =   16,     /**< An enumeration name. */
    I_ClassMember   =   32,     /**< A class member variable. */
    I_StructMember  =   64,     /**< A struct member variable. */
    I_UnionMemeber  =  128,     /**< A union member variable. */
    I_EnumItem      =  256,     /**< An enumeration item. */
    I_FunctionName  =  512,     /**< Function. */
    I_MethodName    = 1024,     /**< A class instance method.*/
    I_Typedef       = 2048,     /**< A type definition. */
    I_Label         = 4096,     /**< A label name. */
    I_Namespace     = 8192,     /**< Namespace identifier. */
//    I_MacroName,        /**< */
//    I_MacroParm,        /**< */
    I_All           = 16383
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
        void restrictToType(const IdentType type) { m_type = type; }


        /**
         *
         */
        void derivedFrom(const std::string className);


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

        const Resource* getResource() const { return _resource; }
        std::string getOrigSymbol() const { return _origSymbol; }
        std::string getNewSymbol() const { return _newSymbol; }
        IdentType getRestrictType() const { return m_type; }


    protected:
    private:
        // make object noncopyable becuase it holds pointer
        Renamer(const Renamer &);
        const Renamer& operator=(const Renamer &);

        class RenamingStrategy;
        RenamingStrategy* m_impl;


        Resource* _resource;
        std::string _origSymbol;
        std::string _newSymbol;
        IdentType m_type;
};

}   // end namespace refactor
#endif // RENAMER_H
