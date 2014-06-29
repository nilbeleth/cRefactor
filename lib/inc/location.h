#ifndef LOCATION_H
#define LOCATION_H
#include <string>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>



namespace refactor
{


class File;


/**
 * @brief A location in source file.
 *
 */
class Location final
{
    friend class Task;
    friend class RenamingMutator;

    public:
        /** Create an invalid location. */
        Location()
            : m_filePath(""), m_line(1), m_column(1)
        { }

        /**
         * Create a new location.
         * @param filePath  Reference to file in which the position is located.
         * @param position  A position inside a file.
         */
        Location(const std::string filePath, const unsigned position);

        /**
         * Create a new location.
         * @param filePath  Reference to file in which the position is located.
         * @param line      Number of line (starting with 1).
         * @param column    Number of character in line.
         */
        Location(const std::string filePath, const unsigned line, const unsigned column);

        /**
         *
         */
        Location(const clang::SourceManager& SM, const clang::SourceLocation loc);

        /** Default destructor */
        virtual ~Location() {}


        ///
        Location getLocWithOffset(const int offset) const;

        /// @deprecated Use @ref Location(const clang::SourceLocation loc, const clang::SourceManager& SM) instead.
        static Location getAsThisLocation(const clang::SourceManager& SM, const clang::SourceLocation loc);


        /// @brief Compare operators.
        /// @{
        bool operator==(const Location& rhs);
        bool operator!=(const Location& rhs);
        /// @}


        /// @brief Accessors.
        /// @{
        std::string getFilePath() const { return m_filePath; }

        unsigned getLine() const;

        unsigned getColumn() const;
        /// @}


        /** @brief Returns true if these location is valid. */
        bool isValid() const { return m_filePath == "" ? false : true; }

        /// @brief Returns a human readable string representation.
        std::string asString() const;

    protected:
    private:
        std::string m_filePath;
        unsigned m_line;
        unsigned m_column;

        clang::SourceLocation getAsSourceLocation(clang::SourceManager& SM) const;
};


/**
 * @brief A location range.
 *
 * A trivial tuple used to represent a source range.
 */
class LocationRange final
{
    public:
        LocationRange(const Location& from, const Location& to);
        ~LocationRange();

        Location getBegin() const { return m_start; }
        Location getEnd() const { return m_end; }
    protected:

    private:
        Location m_start;
        Location m_end;

};

}   // end namespace refactor
#endif // LOCATION_H
