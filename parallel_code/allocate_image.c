#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>

void allocate_image(image *u, int m, int n)
{
    u->m = m; // row
    u->n =n; // col

    float **data;

    data = malloc(m*sizeof(float *));
    data[0] = malloc(n*m*sizeof(float));


    for(int i = 0; i < m; i++)
    {
        data[i] = data[0] + i*n;
    }

    u->image_data = data;
}