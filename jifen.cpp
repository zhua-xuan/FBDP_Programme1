#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
int main(int argc, char** argv)
{
    int a = 10, b = 100, N = 100000000;
    int myid, numprocs;
    int i, source;
    double local = 0.0, dx = (double)(b - a) / N;
    double inte, x, temp, duration;
    clock_t start = clock();
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    for (i = myid; i < N; i = i + numprocs)
    {
        x = a + i * dx + dx / 2;
        local += x * x * x * dx;
    }

    if (myid != 0)
    {
        MPI_Send(&local, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    else
    {
        inte = local;
        for (source = 1; source < numprocs; source++)
        {
            MPI_Recv(&temp, 1, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
            inte += temp;
        }
        clock_t finish = clock();
        duration = (double)(finish - start) / CLOCKS_PER_SEC;
        printf("The integal of x^3 in region [%d,%d] =%16.15f\n", a, b, inte);
        printf("It took %f seconds\n", duration);
    }
    MPI_Finalize();
}