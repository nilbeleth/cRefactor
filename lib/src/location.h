#ifndef LOCATION_H
#define LOCATION_H
#include <string>



namespace refactor
{


class File;


/**
 * @brief A location in source file.
 *
 */
class Location
{
    public:
        /** Create an invalid location. */
        Location()
            : m_filePath(""), m_valid(false), m_position(1)
        { }

        /**
         * Create a new location.
         * @param filePath  Reference to file in which the position is located.
         * @param position  A position inside a file.
         */
        Location(const std::string filePath, const int position);

        /**
         * Create a new location.
         * @param filePath  Reference to file in which the position is located.
         * @param line      Number of line (starting with 1).
         * @param column    Number of character in line.
         */
        Location(const std::string filePath, const int line, const int column);

        /** Default destructor */
        virtual ~Location() {}


        /// @brief Compare operators.
        /// @{
        bool operator==(const Location& rhs);
        bool operator!=(const Location& rhs);
        /// @}


        /// @brief Accessors.
        /// @{
        int getLine() const;

        int getColumn() const;
        /// @}


        /** @brief Returns true if these location is valid. */
        bool isValid() const { return m_valid; }

        /// @brief Returns a human readable string representation.
        std::string asString() const;

    protected:
    private:
        bool m_valid;
        std::string m_filePath;
        int m_position;

        // TODO: vymysliet ci to chcem ako position alebo ako col+line a spravit prechodnu funkciu
};


/**
 * @brief A location range.
 *
 * A trivial tuple used to represent a source range.
 */
class LocationRange
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
