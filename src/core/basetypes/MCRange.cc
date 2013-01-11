#include "MCRange.h"

using namespace mailcore;

Range mailcore::RangeMake(unsigned int index, unsigned int length)
{
    Range range;
    range.index = index;
    range.length = length;
    return range;
}

