#include "MCAssert.h"

#include <stdio.h>
#include <stdlib.h>

void MCAssertInternal(const char * filename, unsigned int line, int cond, const char * condString)
{
    if (cond) {
        return;
    }
    
    fprintf(stderr, "%s:%i: assert %s\n", filename, line, condString);
    abort();
}
