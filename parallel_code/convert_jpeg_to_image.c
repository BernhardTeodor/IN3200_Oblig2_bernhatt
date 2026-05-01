#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>


void convert_jpeg_to_image(const unsigned char* image_chars, image *u)
{

    int m = u->m;
    int n = u->n;

    for(int i = 0; i <m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            u->image_data[i][j] = (float)image_chars[i*n + j];
        }
    }

}