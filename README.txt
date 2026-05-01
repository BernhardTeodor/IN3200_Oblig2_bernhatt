

Serial code
-----------

In the directory serial_code/ use the Makefile.

To compile:

    make serial_main

This creates an executable file "serial_main".

To execute:

    ./serial_main <kappa> <iters> <input_jpeg_filename> <output_jpeg_filename>

kappa is a float and iters is an int
input_jpeg_filename and output_jpeg_filename are strings


Parallel code
-----------

In the directory parallel_code/ use the Makefile.

To compile:

    make parallel_main

This creates an executable file "parallel_main".

To execute:

    mpirun ./parallel_main <kappa> <iters> <input_jpeg_filename> <output_jpeg_filename>

kappa is a float and iters is an int
input_jpeg_filename and output_jpeg_filename are strings

Additionally, you can specifie number of processes by adding " -np <number>" betwwen mpirun and ./parallel_main

    mpirun -np <number of processes>./parallel_main <kappa> <iters> <input_jpeg_filename> <output_jpeg_filename>