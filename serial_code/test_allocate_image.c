#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void test_allocate_image()
{
    
    int m = 5; // col 
    int n = 4; // row
    double size = m*n*sizeof(double); 

    image u;

    allocate_image(&u, m, n);

    assert(u.m == m);
    assert(u.n == n);
    assert(sizeof(u.image_data) == size); 

    free(u.image_data[0]);
    free(u.image_data);
}

int main()
{
    test_allocate_image();
    return(0);
}
