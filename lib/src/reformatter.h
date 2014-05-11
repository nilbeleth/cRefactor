#ifndef REFORMATTER_H
#define REFORMATTER_H
#include "task.h"
#include "resource.h"



namespace refactor
{


/**
 * @brief Source code format style.
 *
 * The CodeStyle is used to configure
 * the formatting to follow specific
 * guidelines.
 */
class CodeStyle
{
    public:
        CodeStyle();

        /** Default destructor. */
        virtual ~CodeStyle();


        /**
         * Returns a format style complying with the LLVM coding standards.
         *  @see http://llvm.org/docs/CodingStandards.html
         */
        static CodeStyle getLLVMStyle();

        /**
         * Returns a format style complying with the Google's coding standards.
         * @see http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
         * @see http://google-styleguide.googlecode.com/svn/trunk/javascriptguide.xml
         * @see https://developers.google.com/protocol-buffers/docs/style
         */
        static CodeStyle getGoogleStyle();

        /**
         * Returns a format style complying with the Chromium's coding standards.
         * @see http://www.chromium.org/developers/coding-style
         */
        static CodeStyle getChromiumStyle();

        /**
         * Returns a format style complying with the Mozilla's coding standards.
         * @see https://developer.mozilla.org/en-US/docs/Developer_Guide/Coding_Style
         */
        static CodeStyle getMozillaStyle();

        /**
         * Returns a format style complying with the WebKit's coding standards.
         * @see https://developer.mozilla.org/en-US/docs/Developer_Guide/Coding_Style
         */
        static CodeStyle getWebKitStyle();

        /**
         * Returns a format style complying with the GNU coding standards.
         * @see http://www.webkit.org/coding/coding-style.html
         */
        static CodeStyle getGNUStyle();


        /**
         * Try to detect formatting style from a sample source file.
         * @param sample    A file with sample code.
         * @param style     Detected formatting style.
         * @return          Error status.
         */
        static int tryToDetectStyle(const std::string& sample, CodeStyle& style);


        /**
         * Parse a CodeStyle out of configuration file.
         * @param file      A configuration file.
         * @param style     A new style defined by configuration.
         * @return          Error status.
         */
        static int parseConfiguration(const std::string& file, CodeStyle& style);


        /**
         * Print out the style as configuration.
         */
        std::string configurationAsText() const;


    protected:

    private:

};


/**
 * @brief Format C/C++ source file.
 *
 *
 */
class Reformatter : public Task
{
    public:
        /** Default constructor */
        Reformatter(Resource* resource, CodeStyle style);

        /** Default destructor */
        virtual ~Reformatter();


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
        //virtual int commit();


    protected:
    private:
};

}   // end namespace refactor
#endif // REFORMATTER_H
