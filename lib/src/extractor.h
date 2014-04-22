#ifndef EXTRACTOR_H
#define EXTRACTOR_H
#include "task.h"
#include "location.h"



namespace refactor
{


/**
 * @brief Extract a function.
 *
 * Take a snippet of function and method and
 * extract it as new function.
 */
class Extractor : public Task
{
    public:
        /** Default constructor */
        Extractor(Resource* resource, const Location& begin, const Location& end);

        /** */
        Extractor(Resource* resource, const LocationRange& range);

        /** Default destructor */
        virtual ~Extractor();


        /**
         * Try to define the new function as class method.
         * If original function wasn't class method, just
         * define it as regular function (this is also
         * the default behavior).
         */
        void tryAsClassMethod();


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
};

}   // end namespace refactor
#endif // EXTRACTOR_H
