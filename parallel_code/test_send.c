#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>
#include <math.h>

typedef struct
{
    float** image_data; /* a 2D array of floats */
    int m; /* # pixels in vertical-direction */
    int n; /* # pixels in horizontal-direction */
} 
image;



int main(int argc, char *argv[])
{

    int m, n, c, iters;
    int my_m, my_rank, num_procs;
    float kappa;
    image u, u_bar, whole_image;
    unsigned char *image_chars, *my_image_chars;
    char *input_jpeg_filename, *output_jpeg_filename;

    if(argc < 5)
    {
        printf("Remember commando line arguments: <kappa> <iters> <input_jpeg_filename> <outpu_jpeg_filename>\n");
    }
    kappa = atof(argv[1]);
    iters = atoi(argv[2]);
    input_jpeg_filename = argv[3];
    output_jpeg_filename = argv[4];


    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size (MPI_COMM_WORLD, &num_procs);

    double tol = 1e-7;

    // printf("Kappa = %f\n iters = %d, input_jpeg_filename = %s\n  output_jpeg_filename = %s\n", kappa, iters, input_jpeg_filename, output_jpeg_filename);
    
    if (my_rank==0) 
    {
        m = 12; 
        n = 10;
        image_chars = malloc(m * n * sizeof(unsigned char));
        for(int i = 0; i < n*m; i++)
        {
            image_chars[i] = i;
        }
    }

    MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
 
    my_m = (int)m/num_procs; 
    int rest = m%num_procs;

    if(my_rank < rest)
    {
        my_m +=1;
    }

    my_image_chars = malloc(my_m * n * sizeof(unsigned char));


    if(my_rank == 0)
    {
        for(int i = 0; i < n*my_m; i++)
        {
            my_image_chars[i] = image_chars[i];
        }
    }

    if(my_rank > 0)
    {

        MPI_Send(&my_m, 1, MPI_INT, 0,0,MPI_COMM_WORLD);
        MPI_Recv(my_image_chars, n*my_m, MPI_UNSIGNED_CHAR,0,1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);  
        
        printf("Hei, dette er %d og jeg fikk dette: fra rank %d\n", my_rank,0);
    }
    else
    {
        int prev = my_m;
        int next; // Antall rader

        for(int i = 1; i < num_procs; i++)
        {
            MPI_Recv(&next, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for(int j = 0; j < n*next; j++)
            {
                my_image_chars[j] = image_chars[i*prev*n + j];
            }

            prev = next;

            MPI_Send(my_image_chars, n*next, MPI_UNSIGNED_CHAR, i  , 1, MPI_COMM_WORLD);
        }
    }
    printf("HEI\n");

    if(my_rank == 1)
    {
        for(int i = 0; i < my_m*n; i ++)
        {
            int val = my_image_chars[i];
            int expected = my_rank*my_m*n + i;

            assert(abs(val - expected) < tol);
        }
        MPI_Send(&my_m, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);
        printf("success fra rank %d\n", my_rank);

    }
    
    if(my_rank == 2)
    {
        double tol = 1e-7;
        int prev;
        MPI_Recv(&prev, 1, MPI_INT, 1,  2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i = 0; i < my_m*n; i ++)
        {
            int val = my_image_chars[i];
            int expected = my_rank*prev*n + i;
            printf("val = %d, expected = %d\n", val, expected);
            assert(abs(val - expected) < tol);
        }
        MPI_Send(&my_m, 1, MPI_INT, 3, 2, MPI_COMM_WORLD);

        printf("success fra rank %d\n", my_rank);

    }

    if(my_rank == 3)
    {
        double tol = 1e-7;
        int prev;
        MPI_Recv(&prev, 1, MPI_INT, 2,  2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(int i = 0; i < my_m*n; i ++)
        {
            int val = my_image_chars[i];
            int expected = my_rank*prev*n + i;
            

            assert(abs(val - expected) < tol);

        }
        printf("success fra rank %d\n", my_rank);

    }










    MPI_Finalize();



    return(0);
}