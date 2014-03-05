#include "renamer.h"


using namespace refactor;



/**
 * @brief
 *
 *
 */
class RenamerImpl : public Renamer
{
    public:
        RenamerImpl();
        ~RenamerImpl();
    protected:

    private:


};


/*****************************
 *       Class Renamer       *
 *****************************/
Renamer::~Renamer()
{
    delete m_impl;
    m_impl = NULL;
}
