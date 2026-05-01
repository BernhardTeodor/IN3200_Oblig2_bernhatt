#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>

void convert_image_to_jpeg(const image *u, unsigned char* image_chars)
{

    int m = u->m;
    int n = u->n;

    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            image_chars[i*n + j] = (unsigned char) u->image_data[i][j];
        }
    }
}