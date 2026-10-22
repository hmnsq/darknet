#include "gemm.h"
#include "utils.h"
#include "cuda.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void gemm_bin(ціле M, ціле N, ціле K, float ALPHA, 
        char  *A, ціле lda, 
        float *B, ціле ldb,
        float *C, ціле ldc)
{
    ціле i,j,k;
    для(i = 0; i < M; ++i){
        для(k = 0; k < K; ++k){
            char A_PART = A[i*lda+k];
            якщо(A_PART){
                для(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } інакше {
                для(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

float *random_matrix(ціле rows, ціле cols)
{
    ціле i;
    float *m = calloc(rows*cols, sizeof(float));
    для(i = 0; i < rows*cols; ++i){
        m[i] = (float)rand()/RAND_MAX;
    }
    повернути m;
}

void time_random_matrix(ціле TA, ціле TB, ціле m, ціле k, ціле n)
{
    float *a;
    якщо(!TA) a = random_matrix(m,k);
    інакше a = random_matrix(k,m);
    ціле lda = (!TA)?k:m;
    float *b;
    якщо(!TB) b = random_matrix(k,n);
    інакше b = random_matrix(n,k);
    ціле ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    ціле i;
    clock_t start = clock(), end;
    для(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (float)(end-start)/CLOCKS_PER_SEC);
    free(a);
    free(b);
    free(c);
}


void gemm(ціле TA, ціле TB, ціле M, ціле N, ціле K, float ALPHA, 
        float *A, ціле lda, 
        float *B, ціле ldb,
        float BETA,
        float *C, ціле ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

void gemm_nn(ціле M, ціле N, ціле K, float ALPHA, 
        float *A, ціле lda, 
        float *B, ціле ldb,
        float *C, ціле ldc)
{
    ціле i,j,k;
    #pragma omp parallel для
    для(i = 0; i < M; ++i){
        для(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[i*lda+k];
            для(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

void gemm_nt(ціле M, ціле N, ціле K, float ALPHA, 
        float *A, ціле lda, 
        float *B, ціле ldb,
        float *C, ціле ldc)
{
    ціле i,j,k;
    #pragma omp parallel для
    для(i = 0; i < M; ++i){
        для(j = 0; j < N; ++j){
            register float sum = 0;
            для(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

void gemm_tn(ціле M, ціле N, ціле K, float ALPHA, 
        float *A, ціле lda, 
        float *B, ціле ldb,
        float *C, ціле ldc)
{
    ціле i,j,k;
    #pragma omp parallel для
    для(i = 0; i < M; ++i){
        для(k = 0; k < K; ++k){
            register float A_PART = ALPHA*A[k*lda+i];
            для(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

void gemm_tt(ціле M, ціле N, ціле K, float ALPHA, 
        float *A, ціле lda, 
        float *B, ціле ldb,
        float *C, ціле ldc)
{
    ціле i,j,k;
    #pragma omp parallel для
    для(i = 0; i < M; ++i){
        для(j = 0; j < N; ++j){
            register float sum = 0;
            для(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


void gemm_cpu(ціле TA, ціле TB, ціле M, ціле N, ціле K, float ALPHA, 
        float *A, ціле lda, 
        float *B, ціле ldb,
        float BETA,
        float *C, ціле ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    ціле i, j;
    для(i = 0; i < M; ++i){
        для(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    якщо(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    інакше якщо(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    інакше якщо(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    інакше
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#include <math.h>

void gemm_gpu(ціле TA, ціле TB, ціле M, ціле N, ціле K, float ALPHA, 
        float *A_gpu, ціле lda, 
        float *B_gpu, ціле ldb,
        float BETA,
        float *C_gpu, ціле ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void time_gpu_random_matrix(ціле TA, ціле TB, ціле m, ціле k, ціле n)
{
    float *a;
    якщо(!TA) a = random_matrix(m,k);
    інакше a = random_matrix(k,m);
    ціле lda = (!TA)?k:m;
    float *b;
    якщо(!TB) b = random_matrix(k,n);
    інакше b = random_matrix(n,k);
    ціле ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    ціле i;
    clock_t start = clock(), end;
    для(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (float)(end-start)/CLOCKS_PER_SEC);
    free(a);
    free(b);
    free(c);
}

void time_gpu(ціле TA, ціле TB, ціле m, ціле k, ціле n)
{
    ціле iter = 10;
    float *a = random_matrix(m,k);
    float *b = random_matrix(k,n);

    ціле lda = (!TA)?k:m;
    ціле ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);

    float *a_cl = cuda_make_array(a, m*k);
    float *b_cl = cuda_make_array(b, k*n);
    float *c_cl = cuda_make_array(c, m*n);

    ціле i;
    clock_t start = clock(), end;
    для(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    double flop = ((double)m)*n*(2.*k + 2.)*iter;
    double gflop = flop/pow(10., 9);
    end = clock();
    double seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    free(a);
    free(b);
    free(c);
}


void test_gpu_accuracy(ціле TA, ціле TB, ціле m, ціле k, ціле n)
{
    srand(0);
    float *a;
    якщо(!TA) a = random_matrix(m,k);
    інакше a = random_matrix(k,m);
    ціле lda = (!TA)?k:m;
    float *b;
    якщо(!TB) b = random_matrix(k,n);
    інакше b = random_matrix(n,k);
    ціле ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    float *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*sizeof(float));
    memset(c_gpu, 0, m*n*sizeof(float));
    ціле i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    double sse = 0;
    для(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    free(a);
    free(b);
    free(c);
    free(c_gpu);
}

ціле test_gpu_blas()
{
    /*
       test_gpu_accuracy(0,0,10,576,75); 

       test_gpu_accuracy(0,0,17,10,10); 
       test_gpu_accuracy(1,0,17,10,10); 
       test_gpu_accuracy(0,1,17,10,10); 
       test_gpu_accuracy(1,1,17,10,10); 

       test_gpu_accuracy(0,0,1000,10,100); 
       test_gpu_accuracy(1,0,1000,10,100); 
       test_gpu_accuracy(0,1,1000,10,100); 
       test_gpu_accuracy(1,1,1000,10,100); 

       test_gpu_accuracy(0,0,10,10,10); 

       time_gpu(0,0,64,2916,363); 
       time_gpu(0,0,64,2916,363); 
       time_gpu(0,0,64,2916,363); 
       time_gpu(0,0,192,729,1600); 
       time_gpu(0,0,384,196,1728); 
       time_gpu(0,0,256,196,3456); 
       time_gpu(0,0,256,196,2304); 
       time_gpu(0,0,128,4096,12544); 
       time_gpu(0,0,128,4096,4096); 
     */
    time_gpu(0,0,64,75,12544); 
    time_gpu(0,0,64,75,12544); 
    time_gpu(0,0,64,75,12544); 
    time_gpu(0,0,64,576,12544); 
    time_gpu(0,0,256,2304,784); 
    time_gpu(1,1,2304,256,784); 
    time_gpu(0,0,512,4608,196); 
    time_gpu(1,1,4608,512,196); 

    повернути 0;
}
#endif

