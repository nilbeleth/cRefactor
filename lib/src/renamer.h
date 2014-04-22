#ifndef RENAMER_H
#define RENAMER_H
#include "task.h"
#include "resource.h"
#include "location.h"
#include <string>


namespace refactor
{



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
};

}   // end namespace refactor
#endif // RENAMER_H
