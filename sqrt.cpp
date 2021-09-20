#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	clock_t a = clock();   //用以计时，比较不同进程数所需时间的不同
	int myid,numprocs,N;
	double SqrtSum = 0.0, d = 0.0, SingleSum = 0.0, duration;
	double* data;
	MPI_Status status; char message[100];
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	if (myid == 0) {
		scanf("%d", &N);
		int i;
		for (i = 1; i < numprocs; ++i) {  //从进程1开始，将N传给子节点
			MPI_Send(&N, 1, MPI_DOUBLE, i, 50, MPI_COMM_WORLD); //将N传给各个节点
		}
	}
	else {  //其他节点接收N
		MPI_Recv(&N, 1, MPI_DOUBLE, 0, 50, MPI_COMM_WORLD, &status);
	}
	data = (double*)malloc(sizeof(double) * N);
	int j;
	for (j = 0; j < N; j++)
		data[j] = j* (j + 1);   //为data赋值
		//子节点接收数据，本地计算此节点中数据的开平方和
	int k;
	for (k = myid; k < N; k += numprocs) {
		SingleSum += sqrt(data[k]);  //本地计算，各子节点其实可以自行判断要不要计算
	}
		//规约所有节点上的SingleSum累加和到SqrtSum并送到主节点0
	MPI_Reduce(&SingleSum, &SqrtSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);  
	//打印累加和
	if (myid == 0) {
		clock_t b = clock();
		duration = (double)(b - a) / CLOCKS_PER_SEC;
		printf("I am process%d. I calculate that SqrtSum =%f.\n", myid, SqrtSum);
		printf("It took %f seconds\n", duration);
	}
	MPI_Finalize();
	free(data);
}
