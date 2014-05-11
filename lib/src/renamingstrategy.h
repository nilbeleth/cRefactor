#ifndef RENAMINGSTRATEGY_H
#define RENAMINGSTRATEGY_H
#include "resource.h"
#include "replacement.h"
#include "renamer.h"
#include <string>
#include <clang/AST/AST.h>
#include <clang/AST/ASTContext.h>


namespace refactor
{



/**
 * @brief
 *
 * This class represents an interface class
 * in strategy design pattern.
 */
class RenamingStrategy
{
    friend class RenamerByName;

    public:
        /** Unify initialization from derived classes */
        RenamingStrategy(Resource* resource, const std::string newSymbol, const IdentType type, const std::string derivedFrom);

        /** Default destructor */
        virtual ~RenamingStrategy() {}

        /** */
        virtual int analyze() = 0;

        IdentType identify(const clang::Decl* decl) const;
        IdentType identify(const std::string& type) const;
        //IdentType identify(const clang::Stmt* stmt) const;


        Replacements getChanges() const { return m_replacements; }

        void setResource(Resource* resource) { m_resource = resource; }
        const Resource* getResource() const { return m_resource; }

        void setNewSymbol(const std::string& newSymbol) { m_new = newSymbol; }
        std::string getNewSymbol() const { return m_new; }

        void setRestrictType(const IdentType type) { m_type = type; }
        IdentType getRestrictType() const { return m_type; }

        void setDerivedFrom(const std::string& from) { m_inFamilyWith = from; }
        std::string getDerivedFrom() const { return m_inFamilyWith; }

    protected:
        Resource* m_resource;
        std::string m_new;
        IdentType m_type;
        std::string m_inFamilyWith;

        Replacements m_replacements;

    private:
};


/**
 * @brief
 *
 *
 */
class RenamerByName : public RenamingStrategy
{
    public:
        RenamerByName(Resource* resource, const std::string& origSymbol, const std::string& newSymbol);

        ~RenamerByName() {}

        int analyze();

        std::string getOldSymbol() const { return m_old; }
    protected:

    private:
        std::string m_old;
};


/**
 * @brief
 *
 *
 */
class LocatorRenamer : public RenamingStrategy
{
    public:
        LocatorRenamer();
        ~LocatorRenamer() {}
    protected:

    private:

};

}   // end namespace refactor
#endif // RENAMINGSTRATEGY_H
