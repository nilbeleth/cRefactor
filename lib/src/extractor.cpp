#include "extractor.h"
#include "debug.h"
#include "utils.h"


using namespace refactor;



Extractor::Extractor(Resource* resource, const Location& begin, const Location& end)
{
    ignore(resource);
    ignore(begin);
    ignore(end);
    ERROR("Not yet implemented.")
}

Extractor::Extractor(Resource* resource, const LocationRange& range)
{
    ignore(resource);
    ignore(range);
    ERROR("Not yet implemented.")
}

Extractor::~Extractor()
{
    //dtor
}

int Extractor::analyze()
{
    return 0;
}
