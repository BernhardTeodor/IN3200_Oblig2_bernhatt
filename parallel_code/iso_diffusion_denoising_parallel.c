#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa, int iters)
{
    int my_rank, num_procs;
    int m = u->m;
    int n = u->n;

    float *under = malloc(n* sizeof(float));
    float *over = malloc(n * sizeof(float));
    float *sender_rad1 = malloc(n * sizeof(float));
    float *sender_radm = malloc(n * sizeof(float));

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    for(int iter = 0; iter < iters; iter ++)
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

        for(int i = 0; i < m; i++)
        {
            u_bar->image_data[i][0] = u->image_data[i][0];
            u_bar->image_data[i][n-1] =  u->image_data[i][n-1];
        }

        for(int i = 0; i < n; i++)
        {
            sender_rad1[i] = u->image_data[0][i];
            sender_radm[i] = u->image_data[m-1][i];
        }

        if(my_rank == 0)
        {
            MPI_Send(sender_radm, n, MPI_FLOAT, my_rank + 1, 1, MPI_COMM_WORLD);
            MPI_Recv(under, n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for(int j = 1; j <= (n-2); j++)
            {
                float s = u->image_data[m - 2][j] + u->image_data[m - 1][j - 1] - 
                4*u->image_data[m - 1][j] + u->image_data[m - 1][j+1] +under[j];

                u_bar->image_data[m - 1][j] = u->image_data[m - 1][j] + kappa*s;
            }
        }
        else if(my_rank  == num_procs - 1)
        {

            if(my_rank %2 == 0)
            {
                MPI_Send(sender_rad1, n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD); 
                MPI_Recv(over, n, MPI_FLOAT, my_rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                MPI_Recv(over, n, MPI_FLOAT,my_rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(sender_rad1, n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD); 
                
            }


            for(int j = 1; j <= (n-2); j++)
            {
                //U_0,i,
                float s = over[j] + u->image_data[0][j - 1] - 
                4*u->image_data[0][j] + u->image_data[0][j + 1] + u->image_data[1][j];

                u_bar->image_data[0][j] = u->image_data[0][j] + kappa*s;
            }

        }
        else
        {
            if (my_rank % 2 == 0) 
            {
                MPI_Send(sender_rad1, n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD); 
                MPI_Send(sender_radm, n, MPI_FLOAT, my_rank + 1, 1, MPI_COMM_WORLD); 
                MPI_Recv(over, n, MPI_FLOAT, my_rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(under, n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } 
            else 
            {
                MPI_Recv(over, n, MPI_FLOAT,my_rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(under, n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(sender_rad1, n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD); 
                MPI_Send(sender_radm, n, MPI_FLOAT, my_rank + 1, 1, MPI_COMM_WORLD);   
            }

            for(int j = 1; j <= (n-2); j++)
            {
                float s_null = over[j] + u->image_data[0][j - 1] - 
                4*u->image_data[0][j] + u->image_data[0][j + 1] + u->image_data[1][j];

                float s_m = u->image_data[m - 2][j] + u->image_data[m - 1][j - 1] - 
                4*u->image_data[m - 1][j] + u->image_data[m - 1][j+1] +under[j];

                u_bar->image_data[0][j] = u->image_data[0][j] + kappa*s_null;

                u_bar->image_data[m - 1][j] = u->image_data[m - 1][j] + kappa*s_m;
            }
        }

        if(iter != (iters -1))
        {   
            image *temp = u;
            u = u_bar;
            u_bar = temp;
        }
    }
    free(sender_rad1);
    free(sender_radm);
    free(under);
    free(over);

}
