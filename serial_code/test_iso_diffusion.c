#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



void test_iso_diffusion_denoising()
{
    unsigned char test_data[16] =
    {1,1,1,1,
    1,10,50,1,
    1,100,80,1,
    1,1,1,1};
    
    int m = 4;
    int n = 4;
    image u;
    image u_bar;
    
    allocate_image(&u, m, n);
    convert_jpeg_to_image(test_data, &u);
    iso_diffusion_denoising(&u, &u_bar, 0.1, 2);
    
    float test_iso[16] = 
    {1,1,1,1, 
    1,22.688,26.768, 1,
    1, 37.568, 37,808,1,
    1,1,1,1};


    float tol = 1e-6;
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j<n; j++)
        {
            assert(abs(u.image_data[i][j] - test_iso[i*n + j]) < tol);
        }
    }

    deallocate_image(&u);
    deallocate_image(&u_bar);
    printf("success\n");


}

int main()
{

    return(0);
}