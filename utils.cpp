#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

void must_alloc(void * ptr)
{
    if (!ptr) {
        perror("malloc failure: ");
        exit(1);
    }
}