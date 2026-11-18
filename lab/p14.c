#include <mpi.h>
#include <stdio.h>
 
int main(int argc, char **argv) {
    MPI_Init(&argc,&argv);
    int rank,size,vals[3],local_sum,global_sum,local_max,global_max,local_min,global_min;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(size!=4) { if(rank==0) fprintf(stderr,"Run with -np 4.\n"); MPI_Finalize(); return 1; }
    for(int i=0;i<3;++i) vals[i]=(rank*3+i+1)*10;
    local_sum=vals[0]+vals[1]+vals[2];
    local_min=vals[0]; local_max=vals[2];
    printf("P%d local sum = %d\n",rank,local_sum);
    MPI_Reduce(&local_sum,&global_sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    MPI_Reduce(&local_max,&global_max,1,MPI_INT,MPI_MAX,0,MPI_COMM_WORLD);
    MPI_Reduce(&local_min,&global_min,1,MPI_INT,MPI_MIN,0,MPI_COMM_WORLD);
    if(rank==0) printf("Global Sum = %d\nGlobal Maximum = %d\nGlobal Minimum = %d\nAverage = %.2f\n",
                       global_sum,global_max,global_min,global_sum/12.0);
    MPI_Finalize(); return 0;
}