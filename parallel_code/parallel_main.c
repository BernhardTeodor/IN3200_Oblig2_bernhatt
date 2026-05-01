#include "function_declarations.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int m, n, c, iters;
    int my_m, my_rank, num_procs;
    float kappa;
    image u, u_bar, whole_image;
    unsigned char *image_chars, *my_image_chars;
    char *input_jpeg_filename, *output_jpeg_filename;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size (MPI_COMM_WORLD, &num_procs);

    /* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_filename */

    if(argc < 5)
        {
            printf("Remember commando line arguments: <kappa> <iters> <input_jpeg_filename> <outpu_jpeg_filename>\n");
        }
        
    kappa = atof(argv[1]);
    iters = atoi(argv[2]);
    input_jpeg_filename = argv[3];
    output_jpeg_filename = argv[4];

    if (my_rank==0) 
    {
        import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
        allocate_image (&whole_image, m, n);
    }

    MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (&iters, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast (&kappa, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    /* 1D horizontal decomposition of the m x n pixels evenly among the MPI processes */
    /* If there is a neighbor from below, allocate one more row; Same if there is neighbor from above */

    my_m = (int)m/num_procs; 
    int rest = m%num_procs;

    if(my_rank < rest)
    {
        my_m +=1;
    }

    my_image_chars = malloc(my_m * n * sizeof(unsigned char));

    allocate_image (&u, my_m, n);
    allocate_image (&u_bar, my_m, n);

    /* each process asks process 0 for a partitioned region */
    /* of image_chars and copy the values into u */
    /* ... */

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
    }
    else
    {
        int next; 
        unsigned char *sending;

        int start = my_m;

        for(int i = 1; i < num_procs; i++)
        {
            MPI_Recv(&next, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sending = malloc(n*next*sizeof(unsigned char));

            for(int j = 0; j < n*next; j++)
            {
                sending[j] = image_chars[start*n + j];
            }

            start += next;
            MPI_Send(sending, n*next, MPI_UNSIGNED_CHAR, i  , 1, MPI_COMM_WORLD);
            free(sending);
        }
    }

    convert_jpeg_to_image (my_image_chars, &u);
    iso_diffusion_denoising_parallel (&u, &u_bar, kappa, iters);

    /* each process sends its resulting content of u_bar to process 0 */
    /* process 0 receives from each process incoming values and */
    /* copy them into the designated region of struct whole_image */
    /* ... */


    if(my_rank == 0)
    {
        int initial_split = (int)m/num_procs;

        for(int i = 0; i < my_m; i++)
        {
            for(int j = 0; j < n; j++)
            {
                whole_image.image_data[i][j] = u_bar.image_data[i][j];
            }
        }

        int start = my_m;

        for(int rank = 1; rank < num_procs ; rank++)
        {
            int rows = initial_split;
            if(rank < rest)
            {
                rows +=1;
            }

            MPI_Recv(whole_image.image_data[start], rows*n, MPI_FLOAT, rank, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            start += rows;
        }
    }
    else
    {
        MPI_Send(u_bar.image_data[0], my_m*n, MPI_FLOAT, 0, 5, MPI_COMM_WORLD);
    }

    if (my_rank==0) 
    {
        convert_image_to_jpeg(&whole_image, image_chars);
        export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
        deallocate_image (&whole_image);
    }

    deallocate_image (&u);
    deallocate_image (&u_bar);
    MPI_Finalize ();

    return 0;
}