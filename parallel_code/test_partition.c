#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <mpi.h>

int divi(int a, int b)
{
    int s = (int)a/b;
    return(s);
}



int main(int argc, char *argv[])
{
    // Lage riktig paritions
    // Jeg har 4 processess
    int rank, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &num_procs);


    int m = 11;

    int my_m = divi(m,num_procs);

    int rest = m%num_procs;

    if (rank < rest)
    {
        my_m += 1;
    }

    if(rank == 1 |rank == 0 | rank == 2)
    {
        assert(my_m == 3);
        printf("Good fra process %d, jeg har %d\n", rank, my_m);
    }
    else
    {
        assert(my_m == 2);
        printf("Good fra process %d, jeg har %d\n", rank, my_m);
    }

    m = 1000;

    my_m = divi(m,num_procs);

    rest = m%num_procs;

    if (rank < rest)
    {
        my_m += 1;
    }

    assert(my_m == 250);
    printf("Good fra process %d, jeg har %d\n", rank, my_m);
    
    m = 422;

    my_m = divi(m,num_procs);

    rest = m%num_procs;

    if (rank < rest)
    {
        my_m += 1;
    }

    if(rank == 0 | rank == 1)
    {
        assert(my_m == 106);
        printf("Good fra process %d, jeg har %d\n", rank, my_m);
    }
    else
    {
        assert(my_m == 105);
        printf("Good fra process %d, jeg har %d\n", rank, my_m);
    }

     m = 421;

    my_m = divi(m,num_procs);

    rest = m%num_procs;

    if (rank < rest)
    {
        my_m += 1;
    }

    if(rank == 0)
    {
        assert(my_m == 106);
        printf("Good fra process %d, jeg har %d\n", rank, my_m);
    }
    else
    {
        assert(my_m == 105);
        printf("Good fra process %d, jeg har %d\n", rank, my_m);
    }



    printf("success\n");

    MPI_Finalize();
    return(0);
}