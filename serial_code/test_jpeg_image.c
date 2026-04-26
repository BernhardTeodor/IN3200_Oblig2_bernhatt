#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



void test_convert_jpeg_to_image()
{

    int m = 4; //row
    int n = 3;
    image u;

    unsigned char test[12] ={1,2,3,
        4,5,6,
        7,8,9,
        10,11,12};


    allocate_image(&u, m, n);

    convert_jpeg_to_image(test, &u);

    float expected[4][3] ={
        {1,2,3},{4,5,6},{7,8,9},{10,11,12}
    };

    float tol = 1e-7;

    for(int i =0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            float actual = u.image_data[i][j];
            float expected_val = expected[i][j];
            assert(abs(actual - expected_val) < tol);
        }
    }
    printf("succsess\n");
    deallocate_image(&u);
}


int main()
{
    test_convert_jpeg_to_image();
    return(0);
}