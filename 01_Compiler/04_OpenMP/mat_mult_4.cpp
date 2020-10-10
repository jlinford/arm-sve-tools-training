#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <armpl.h>
#include <omp.h>

using timer_clock= std::chrono::high_resolution_clock;

inline unsigned int min(unsigned int a, unsigned int b)
{
    return (a < b) ? a : b;
}

void naive_multiply(double **matA, double **matB, double **matC, unsigned int n,
        unsigned int m, unsigned int l)
{
    timer_clock::time_point t1= timer_clock::now();
    unsigned int i, j, k;
    // Perform the matrix-matrix multiplication naively
#pragma omp parallel for private(i, j, k)
    for (i= 0; i < n; ++i){
        for (j= 0; j < l; ++j){
            for (k= 0; k < m; ++k){
                matC[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    timer_clock::time_point t2= timer_clock::now();
    std::chrono::duration<double> time_span= std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    printf("Naive multiply took: %.3lf seconds\n", time_span.count());
}

void block_multiply(double **matA, double **matB, double **matC, unsigned int n,
        unsigned int m, unsigned int l, unsigned int blockSize)
{
    timer_clock::time_point t1= timer_clock::now();
    unsigned int i, j, jj, k, kk;
    // Perform the matrix-matrix multiplication with a bit of blocking and
    // loop unrolling. We cheat with the loop unrolling, and just expect the
    // dimensions that are passed in to be a multiple of two.
#pragma omp parallel for private(i, j, jj, k, kk)
    for (kk= 0; kk < m; kk+= blockSize){
        for(jj= 0; jj < l; jj+= blockSize){
            for(i= 0; i < n; i+= 2){
                for(j= jj; j < min(l, jj + blockSize); j+= 2){
                    for(k =kk; k < min(m, kk + blockSize); ++k){
                        matC[i][j] += matA[i][k] * matB[k][j];
                        matC[i][j+1] += matA[i][k] * matB[k][j+1];
                        matC[i+1][j] += matA[i+1][k] * matB[k][j];
                        matC[i+1][j+1] += matA[i+1][k] * matB[k][j+1];
                    }
                }
            }
        }
    }
    timer_clock::time_point t2= timer_clock::now();
    std::chrono::duration<double> time_span= std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    printf("Block multiply took: %.3lf seconds\n", time_span.count());
}

int main(int argc, char** argv)
{
    double *dataA= NULL;
    double **matA= NULL;
    double *dataB= NULL;
    double **matB= NULL;
    double *dataC= NULL;
    double **matC= NULL;

    // Assume that the matrix dimensions have been passed in as n, m and l, where
    // A = n x m matrix
    // B = m x l matrix
    // C = n x l matrix
    
    unsigned int n, m, l;
    unsigned int blockSize;

    unsigned int i, j, k;

    if (argc < 5){
        printf("Usage: %s n m l blockSize\n", argv[0]);
        return 1;
    }

    n= (unsigned int)atoi(argv[1]);
    m= (unsigned int)atoi(argv[2]);
    l= (unsigned int)atoi(argv[3]);
    blockSize= (unsigned int)atoi(argv[4]);

    // Assign memory
    dataA= (double*)malloc(n*m*sizeof(double));
    matA= (double**)malloc(n*sizeof(double*));
    dataB= (double*)malloc(m*l*sizeof(double));
    matB= (double**)malloc(m*sizeof(double*));
    dataC= (double*)malloc(n*l*sizeof(double));
    matC= (double**)malloc(n*sizeof(double*));

    timer_clock::time_point t1= timer_clock::now();

    // Set up the matrices in row major format
    for(i= 0; i < n; ++i){
        matA[i]= dataA + i*m;
        matC[i]= dataC + i*l;
    }

    for(i= 0; i < m; ++i){
        matB[i]= dataB + i*l;
    }

    srand(time(NULL));
#pragma omp parallel for private(i, j)
    for (i= 0; i < n; ++i){
        for (j= 0; j < m; ++j){
            matA[i][j]= ((double)rand()) / RAND_MAX;
        }

        for (j= 0; j < l; ++j){
            matC[i][j]= 0.0;
        }
    }

#pragma omp parallel for private(i, j)
    for (i= 0; i < m; ++i){
        for (j= 0; j < l; ++j){
            matB[i][j]= ((double)rand()) / RAND_MAX;
        }
    }

    timer_clock::time_point t2= timer_clock::now();
    std::chrono::duration<double> time_span= std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    printf("Set up of matrices took: %.3lf seconds\n", time_span.count());

    // Perform the matrix-matrix multiplication with a bit of blocking and
    // loop unrolling
    printf("Performing blocked multiply\n");
    block_multiply(matA, matB, matC, n, m, l, blockSize);

    // Perform the matrix-matrix multiplication with the Arm libraries
    t1= timer_clock::now();
    printf("Using optimised BLAS routine\n");
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, m, l, 1, dataA,
            m, dataB, l, 1, dataC, m);
    t2= timer_clock::now();
    time_span= std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    printf("Arm Performance Library took: %.3lf seconds\n", time_span.count());

    // Free memory
    free(matA);
    free(dataA);
    free(matB);
    free(dataB);
    free(matC);
    free(dataC);
}