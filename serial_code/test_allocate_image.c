#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void test_allocate_image()
{

    int m = 5; // row 
    int n = 4; // col

    image u;

    allocate_image(&u, m, n);

    assert(u.m == m);
    assert(u.n == n);

    // Fill matrix

    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            u.image_data[i][j] = (float)i+j;
        }
    }

    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {

            assert((u.image_data[i][j]  - ((float)i+j))< 1e-7);
        }
    }

    printf("success\n");    

    free(u.image_data[0]);
    free(u.image_data);
}

int main()
{
    test_allocate_image();
    return(0);
}
