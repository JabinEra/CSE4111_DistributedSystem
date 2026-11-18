// #include <mpi.h>
// #include <stdio.h>
// #include <stdlib.h>
 
// static double f(double x) { return x*x; }
// int main(int argc, char **argv) {
//     MPI_Init(&argc,&argv);
//     int rank,size;
//     MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
//     MPI_Comm_size(MPI_COMM_WORLD,&size);
//     long n=(argc>1)?atol(argv[1]):1000;
//     double a=0.0,b=1.0,h=(b-a)/n,local=0.0,global=0.0;
//     if(n<1) { if(rank==0) fprintf(stderr,"n must be positive.\n"); MPI_Finalize(); return 1; }
//     for(long i=rank;i<n;i+=size) {
//         double x0=a+i*h, x1=x0+h;
//         local += (f(x0)+f(x1))*h/2.0;
//     }
//     printf("P%d partial integral = %.9f\n",rank,local);
//     MPI_Reduce(&local,&global,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
//     if(rank==0) {
//         double exact=1.0/3.0;
//         printf("Numerical integral = %.9f\nExact integral = %.9f\nAbsolute error = %.9g\n",
//                global,exact,(global>exact)?global-exact:exact-global);
//     }
//     MPI_Finalize(); return 0;
// }

#include<mpi.h>
#include<stdio.h>
#include<math.h>

double f(double x)
{
    return x*x;
}

int main(int argc , char **argv)
{
    int rank,size;
    int n=1000;
    double a=0,b=1;
    double h,local_a,local_b;
    double sum=0,total;
    double x,exact,error;


    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    h=(b-a)/n;

    int local_n=n/size;
    local_a=a+rank*local_n*h;
    local_b=local_a+local_n*h;

    sum=(f(local_a)+f(local_b))/2.0;

    for(int i=1;i<local_n;i++)
    {
        x=local_a + i*h;
        sum+=f(x);
    }
    sum=sum*h;

    printf("P%d: Local integral = %f\n",rank,sum);

    MPI_Reduce(&sum,&total,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

    if(rank==0)
    {
        exact=1.0/3.0;
        error=fabs(exact-total);

        printf("\nNumerical integral = %f\n",total);
        printf("Exact integral = %f\n",exact);
        printf("Error = %f\n",error);

    }

    MPI_Finalize();
    return 0;
}