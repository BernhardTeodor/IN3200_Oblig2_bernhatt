#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>

void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters)
{
    int m = u->m;
    int n = u->n;
    
    image temp;
    allocate_image(&temp, m, n);

    for(int iter = 0; iter < iters; iter++)
    {
        for(int i = 1; i <= (m-2) ; i++)
        {
            for(int j = 1; j <= (n-2); j++)
            {
                float s = u->image_data[i - 1][j] + u->image_data[i][j -1] - 
                4*u->image_data[i][j] + u->image_data[i][j+1] +u->image_data[i+1][j];

                u_bar->image_data[i][j] = u->image_data[i][j] + kappa*s;
            }
        }

        for(int i = 0; i < n; i++)
        {
            u_bar->image_data[0][i] = u->image_data[0][i];
            u_bar->image_data[m - 1][i] =  u->image_data[m-1][i];
        }
        for(int i = 0; i < m; i++)
        {
            u_bar->image_data[i][0] = u->image_data[i][0];
            u_bar->image_data[i][n-1] =  u->image_data[i][n-1];
        }

        if(iter != iters -1){ 
        image *temp = u;
        u = u_bar;
        u_bar = temp;
        }
    }
    deallocate_image(&temp);


}

