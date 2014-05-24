#ifndef TASK_H
#define TASK_H
#include <string>

#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/FileManager.h>

#include "resource.h"
#include "replacement.h"


/**
 * @mainpage The mainpage documentation
 *
 * @author Matej Odalos <xodalo00@stud.fit.vutbr.cz>
 * @date       April 22, 2014
 * @version    0.0.1 (pre-alpha)
 *
 * @section intro Introduction
 * Master thesis.
 *
 * @section features What can it do?
 * @subsection renamer Renaming identifier
 * Rename an identidier.
 *
 * @subsection reformatter Formatting code
 * Format source files.
 *
 * @subsection extractor Method extraction
 * Extract method.
 *
 * @section examples User examples
 * A nice example.
 *
 * @section future Future work
 * TODO: I have really lot of work to do here.
 * <ul>
 *      <li>Manzovanie includov</li>
 * </ul>
 */

namespace refactor
{


/**
 * @brief An abstract refactoring task.
 *
 * The task class is an abstract interface for
 * a refactoring task. This interface provides
 * all interaction needed to perform some task
 * on group of files.
 *
 * The task should have a very short liveness.
 * Therefore after task construction (provided
 * be concrete task constructor) there is
 * no possibility to add, change or erase
 * any files from analysis. Rather than modify
 * an existing task one should construct a new
 * one.
 *
 * The replacements could be applied one by one
 * or all at once.
 */
class Task
{
    public:
        /** @brief Default destructor */
        virtual ~Task();

        /**
         * Process the possible changes to
         * all files done by refactoring task.
         * @note Error can occured also from parsing
         *       error.
         * @return  Returns 1 if processing fails, 0 otherwise.
         */
        virtual int analyze() =0;

        /**
         * Get all possible replacements determined
         * by analysis.
         */
        Replacements getChanges() const { return m_replacements; }

        /**
         * Apply a replacement if it's possible.
         * @param replace   A replacement to be apllied.
         * @return          Return 0 if the replacement
         *                  is successfully apllied, 1 otherwise.
         */
        int applyChange(const Replacement& replace);

        /**
         * Apply all possible replacements.
         * @return  Has been the replacement successfully apllied?
         * @retval  fails
         *          <ul>
         *              <li> 0 = Success
         *              <li> 1 = Failure
         *          </ul>
         */
        int commit();


    protected:
        /** Hide default constructor */
        Task();

        Replacements m_replacements;
        clang::Rewriter* m_rewriter;
        clang::SourceManager* m_SM;
        clang::FileManager* m_FM;

    private:
        bool initializeRewriter();
};

}   // end namespace refactor
#endif // TASK_H
