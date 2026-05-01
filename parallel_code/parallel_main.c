#include "function_declarations.h"
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
    /* ... */

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

    convert_jpeg_to_image (my_image_chars, &u);
    iso_diffusion_denoising_parallel (&u, &u_bar, kappa, iters);

    /* each process sends its resulting content of u_bar to process 0 */
    /* process 0 receives from each process incoming values and */
    /* copy them into the designated region of struct whole_image */
    /* ... */

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