#ifndef RENAMER_H
#define RENAMER_H
#include "task.h"
#include "resource.h"
#include "location.h"
#include <string>


namespace refactor
{


class RenamingStrategy;


/**
 * @brief Identifiers.
 *
 * Describe the types of identifiers.
 */
enum class IdentType : unsigned int
{
    I_Unknown       = 0x0000,     /**< Reserve for invalid results. */
    I_Variable      = 0x0001,     /**< A name of variable. */
    I_Argument      = 0x0002,     /**< An parameter of function or method. */
    I_ClassName     = 0x0004,     /**< A class name. */
    I_StructName    = 0x0008,     /**< A structure name. */
    I_UnionName     = 0x0010,     /**< An union name. */
    I_EnumName      = 0x0020,     /**< An enumeration name. */
    I_ClassMember   = 0x0040,     /**< A class member variable. */
    I_StructMember  = 0x0080,     /**< A struct member variable. */
    I_UnionMember   = 0x0100,     /**< A union member variable. */
    I_EnumItem      = 0x0200,     /**< An enumeration item. */
    I_FunctionName  = 0x0400,     /**< Function. */
    I_MethodName    = 0x0800,     /**< A class instance method.*/
    I_Typedef       = 0x1000,     /**< A type definition. */
    I_Label         = 0x2000,     /**< A label name. */
    I_Namespace     = 0x4000,     /**< Namespace identifier. */
//    I_MacroName,        /**< */
//    I_MacroParm,        /**< */
    I_All           = 0x7FFF
};

inline constexpr IdentType operator&(IdentType __x, IdentType __y)
{
    return static_cast<IdentType>(static_cast<unsigned int>(__x) & static_cast<unsigned int>(__y));
}

inline constexpr IdentType operator|(IdentType __x, IdentType __y)
{
    return static_cast<IdentType>(static_cast<unsigned int>(__x) | static_cast<unsigned int>(__y));
}

inline constexpr IdentType operator^(IdentType __x, IdentType __y)
{
    return static_cast<IdentType>(static_cast<unsigned int>(__x) ^ static_cast<unsigned int>(__y));
}

inline constexpr IdentType operator~(IdentType __x)
{
    return static_cast<IdentType>(~static_cast<unsigned int>(__x));
}

inline constexpr IdentType operator&=(IdentType __x, IdentType __y)
{
    return __x & __y;
}

inline constexpr IdentType operator|=(IdentType __x, IdentType __y)
{
    return __x | __y;
}

inline constexpr IdentType operator^=(IdentType __x, IdentType __y)
{
    return __x ^ __y;
}


/**
 * @brief Rename a identifier.
 *
 *
 */
class Renamer final : public Task
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
        void restrictToType(const IdentType type);


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
        virtual int analyze() override;

        /**
         * Get all possible replacements determined
         * by analysis.
         */
        //virtual Replacements getChanges() const;

        /**
         * Apply a replacement if it's possible.
         * @param replace   A replacement to be apllied.
         * @return          Return 0 if the replacement
         *                  is successfully apllied, 1 otherwise.
         */
        //virtual int applyChange(const Replacement& replace) const;

        /**
         * Apply all possible replacements.
         * @return          Return 0 if the replacement
         *                  is successfully apllied, 1 otherwise.
         */
        //virtual int commit() const;

        //const Resource* getResource() const { return _resource; }
        //std::string getOrigSymbol() const { return _origSymbol; }
        //std::string getNewSymbol() const { return _newSymbol; }
        //IdentType getRestrictType() const { return m_type; }


    protected:
    private:
        // make object noncopyable becuase it holds pointer
        Renamer(const Renamer &);
        const Renamer& operator=(const Renamer &);

        RenamingStrategy* m_impl;


        //Resource* _resource;
        //std::string _origSymbol;
        //std::string _newSymbol;
        //IdentType m_type;
};

}   // end namespace refactor
#endif // RENAMER_H
