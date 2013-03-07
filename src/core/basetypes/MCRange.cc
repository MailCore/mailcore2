#include "MCRange.h"

using namespace mailcore;

Range mailcore::RangeMake(uint64_t location, uint64_t length)
{
    Range range;
    range.location = location;
    range.length = length;
    return range;
}

