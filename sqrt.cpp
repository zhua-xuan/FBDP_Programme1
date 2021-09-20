#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	clock_t a = clock();   //���Լ�ʱ���Ƚϲ�ͬ����������ʱ��Ĳ�ͬ
	int myid,numprocs,N;
	double SqrtSum = 0.0, d = 0.0, SingleSum = 0.0, duration;
	double* data;
	MPI_Status status; char message[100];
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	if (myid == 0) {
		scanf("%d", &N);
		int i;
		for (i = 1; i < numprocs; ++i) {  //�ӽ���1��ʼ����N�����ӽڵ�
			MPI_Send(&N, 1, MPI_DOUBLE, i, 50, MPI_COMM_WORLD); //��N���������ڵ�
		}
	}
	else {  //�����ڵ����N
		MPI_Recv(&N, 1, MPI_DOUBLE, 0, 50, MPI_COMM_WORLD, &status);
	}
	data = (double*)malloc(sizeof(double) * N);
	int j;
	for (j = 0; j < N; j++)
		data[j] = j* (j + 1);   //Ϊdata��ֵ
		//�ӽڵ�������ݣ����ؼ���˽ڵ������ݵĿ�ƽ����
	int k;
	for (k = myid; k < N; k += numprocs) {
		SingleSum += sqrt(data[k]);  //���ؼ��㣬���ӽڵ���ʵ���������ж�Ҫ��Ҫ����
	}
		//��Լ���нڵ��ϵ�SingleSum�ۼӺ͵�SqrtSum���͵����ڵ�0
	MPI_Reduce(&SingleSum, &SqrtSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);  
	//��ӡ�ۼӺ�
	if (myid == 0) {
		clock_t b = clock();
		duration = (double)(b - a) / CLOCKS_PER_SEC;
		printf("I am process%d. I calculate that SqrtSum =%f.\n", myid, SqrtSum);
		printf("It took %f seconds\n", duration);
	}
	MPI_Finalize();
	free(data);
}
