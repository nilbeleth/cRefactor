#include "extractor.h"
#include "debug.h"


using namespace refactor;



Extractor::Extractor(Resource* resource, const Location& begin, const Location& end)
{
    //ctor
}

Extractor::Extractor(Resource* resource, const LocationRange& range)
{

}

Extractor::~Extractor()
{
    //dtor
}

int Extractor::analyze()
{
    return 0;
}

/*
refactor::Replacements Extractor::getChanges() const
{
    ERROR("Not yet implemented.")
}

int Extractor::commit()
{
    ERROR("Not yet implemented.")
    return 0;
}
*/
