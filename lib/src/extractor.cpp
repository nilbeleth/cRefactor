#include "extractor.h"
#include "debug.h"


using namespace refactor;



Extractor::Extractor(Resource* resource, const Location& begin, const Location& end)
{
    (void) resource;
    (void) begin;
    (void) end;
    ERROR("Not yet implemented.")
}

Extractor::Extractor(Resource* resource, const LocationRange& range)
{
    (void) resource;
    (void) range;
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
