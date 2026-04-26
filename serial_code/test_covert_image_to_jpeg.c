#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void test_image_to_jpeg()
{

    int m = 4; //row
    int n = 3;
    image u;

    unsigned char actual[12] ={1,2,3,
        4,5,6,
        7,8,9,
        10,11,12};


    allocate_image(&u, m, n);

    convert_jpeg_to_image(actual, &u);


    unsigned char *new;

    new = (unsigned char*)malloc(m*n*sizeof(unsigned char));
    convert_image_to_jpeg(&u, &new);

    float tol = 1e-7;

    for(int i =0; i <(m*n); i++)
    {
        assert(abs(new[i] - actual[i]) < tol);
    }

    printf("success\n");
    deallocate_image(&u);
    free(new);

}

int main()
{
    test_image_to_jpeg();
    return(0);
}