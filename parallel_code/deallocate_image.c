#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>


void deallocate_image(image *u)
{
    free(u->image_data[0]);
    free(u->image_data);
    u->m = 0;
    u->n = 0;
}