#ifndef REPLACEMENT_H
#define REPLACEMENT_H
#include <string>
#include <vector>
#include <set>
#include "location.h"


namespace refactor
{


/**
 * @brief A text replacement.
 *
 * Represents a modification in source files
 * with specific location, range and replacement.
 * These are mostly created by one of the
 * refactoring tasks.
 */
class Replacement
{
    //friend class Renamer;
    //friend class Reformatter;
    //friend class Extractor;
    friend class RenamerByName;

    public:
        /** Construct an invalid replacement. */
        Replacement();

        /**
         * Create a new valid replacement.
         * @note These replacements could be created only
         *       by library itself therefore it's hidden.
         * @param loc
         * @param offset
         * @param toReplace
         */
        Replacement(const Location& loc, const int offset, const std::string& toReplace );

        /** Default destructor */
        virtual ~Replacement();


        /**
         * Returns whether this replacement can be applied to a file.
         */
        bool isApplicable() const;


        bool operator==(const Replacement& rhs);
        bool operator!=(const Replacement& rhs);


        /** @brief Returns a human readable string representation. */
        std::string asString() const;


    protected:
    private:
        Location m_location;
        int m_offset;
        std::string m_replaceText;
};


/// A set of replacements.
/// @todo Should be deduplicated somewhere.
typedef std::vector<Replacement> Replacements;

}   // end namespace refactor
#endif // REPLACEMENT_H
