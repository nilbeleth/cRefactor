#ifndef REPLACEMENT_H
#define REPLACEMENT_H
#include <string>
#include <vector>
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
class Replacement final
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


        Location getLocation() const { return m_location; }
        int getOffset() const { return m_offset; }
        std::string getReplaceText() const { return m_replaceText; }


        /** @brief Returns a human readable string representation. */
        std::string asString() const;


    protected:
    private:
        Location m_location;
        int m_offset;
        std::string m_replaceText;
};



/**
 * @brief A container class for replacements.
 *
 * This class acts as a container for all replacements.
 * The primary responsibility lies in manageing and
 * storing replacements. It also ensures that in one
 * container are exclusive modifications to the source
 * text.
 *
 * Note that this class is simply a wrapper around
 * STL vector container providing uniqueness for its
 * elements.
 *
 * @note It could be interesting to create a wrapper
 * template base class that could derive all container
 * classes and possibly overload some interesting
 * operators. Something like:
 @code{.cpp}
template <typename T>
class IterableContainer
{
    public:
        virtual int add(T item);
        virtual int remove(T item);
        T operator[](const unsingned int& index);
        ...

        T* begin();
        const T* begin();
        ...
    protected:
        std::vector<T> _data;
    private:
};
 @endcode
 */
class Replacements
{
    public:
        typedef Replacement* iterator;
        typedef const Replacement* const_iterator;

        /** Default constructor. */
        Replacements() {}

        /** Default destructor. */
        ~Replacements() {}

        int add(Replacement item);
        int remove(const Replacement& item);
        void clear() { _data.clear(); }

        size_t size() const { return _data.size(); }
        bool isEmpty() const { return _data.size() == 0; }
        Replacement& operator[](const int& index) { return _data.at(index); }

// TODO (nilbeleth#5#): s tymto == by som casom daco mohol spravit
        //bool operator==(const Replacements& rhs);
        //bool operator!=(const Replacements& rhs);

        int operator+(const Replacements& rhs);
        inline int merge(const Replacements& rhs) { return *this + rhs; }

        iterator begin() { return &_data[0]; }
        const_iterator begin() const { return &_data[0]; }
        iterator end() { return &_data[_data.size()]; }
        const_iterator end() const { return &_data[_data.size()]; }

        std::string asString() const;

    protected:
    private:
        std::vector<Replacement> _data;

};

}   // end namespace refactor
#endif // REPLACEMENT_H
