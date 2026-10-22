#include "gemm.h"
#include "utils.h"
#include "cuda.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void gemm_bin(จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        var_na  *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float *C, จำนวนเต็ม ldc)
{
    จำนวนเต็ม i,j,k;
    สำหรับ(i = 0; i < M; ++i){
        สำหรับ(k = 0; k < K; ++k){
            var_na A_PART = A[i*lda+k];
            ถ้า(A_PART){
                สำหรับ(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } มิฉะนั้น {
                สำหรับ(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

float *random_matrix(จำนวนเต็ม rows, จำนวนเต็ม cols)
{
    จำนวนเต็ม i;
    float *m = calloc(rows*cols, parimaa_na(float));
    สำหรับ(i = 0; i < rows*cols; ++i){
        m[i] = (float)rand()/RAND_MAX;
    }
    คืน m;
}

void time_random_matrix(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม m, จำนวนเต็ม k, จำนวนเต็ม n)
{
    float *a;
    ถ้า(!TA) a = random_matrix(m,k);
    มิฉะนั้น a = random_matrix(k,m);
    จำนวนเต็ม lda = (!TA)?k:m;
    float *b;
    ถ้า(!TB) b = random_matrix(k,n);
    มิฉะนั้น b = random_matrix(n,k);
    จำนวนเต็ม ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    จำนวนเต็ม i;
    clock_t start = clock(), end;
    สำหรับ(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (float)(end-start)/CLOCKS_PER_SEC);
    free(a);
    free(b);
    free(c);
}


void gemm(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float BETA,
        float *C, จำนวนเต็ม ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

void gemm_nn(จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float *C, จำนวนเต็ม ldc)
{
    จำนวนเต็ม i,j,k;
    #pragma omp parallel สำหรับ
    สำหรับ(i = 0; i < M; ++i){
        สำหรับ(k = 0; k < K; ++k){
            pa_njikaa float A_PART = ALPHA*A[i*lda+k];
            สำหรับ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

void gemm_nt(จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float *C, จำนวนเต็ม ldc)
{
    จำนวนเต็ม i,j,k;
    #pragma omp parallel สำหรับ
    สำหรับ(i = 0; i < M; ++i){
        สำหรับ(j = 0; j < N; ++j){
            pa_njikaa float sum = 0;
            สำหรับ(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

void gemm_tn(จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float *C, จำนวนเต็ม ldc)
{
    จำนวนเต็ม i,j,k;
    #pragma omp parallel สำหรับ
    สำหรับ(i = 0; i < M; ++i){
        สำหรับ(k = 0; k < K; ++k){
            pa_njikaa float A_PART = ALPHA*A[k*lda+i];
            สำหรับ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

void gemm_tt(จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float *C, จำนวนเต็ม ldc)
{
    จำนวนเต็ม i,j,k;
    #pragma omp parallel สำหรับ
    สำหรับ(i = 0; i < M; ++i){
        สำหรับ(j = 0; j < N; ++j){
            pa_njikaa float sum = 0;
            สำหรับ(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


void gemm_cpu(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float BETA,
        float *C, จำนวนเต็ม ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    จำนวนเต็ม i, j;
    สำหรับ(i = 0; i < M; ++i){
        สำหรับ(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    ถ้า(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    มิฉะนั้น ถ้า(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    มิฉะนั้น ถ้า(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    มิฉะนั้น
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#include <math.h>

void gemm_gpu(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A_gpu, จำนวนเต็ม lda, 
        float *B_gpu, จำนวนเต็ม ldb,
        float BETA,
        float *C_gpu, จำนวนเต็ม ldc)
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

void time_gpu_random_matrix(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม m, จำนวนเต็ม k, จำนวนเต็ม n)
{
    float *a;
    ถ้า(!TA) a = random_matrix(m,k);
    มิฉะนั้น a = random_matrix(k,m);
    จำนวนเต็ม lda = (!TA)?k:m;
    float *b;
    ถ้า(!TB) b = random_matrix(k,n);
    มิฉะนั้น b = random_matrix(n,k);
    จำนวนเต็ม ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    จำนวนเต็ม i;
    clock_t start = clock(), end;
    สำหรับ(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (float)(end-start)/CLOCKS_PER_SEC);
    free(a);
    free(b);
    free(c);
}

void time_gpu(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม m, จำนวนเต็ม k, จำนวนเต็ม n)
{
    จำนวนเต็ม iter = 10;
    float *a = random_matrix(m,k);
    float *b = random_matrix(k,n);

    จำนวนเต็ม lda = (!TA)?k:m;
    จำนวนเต็ม ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);

    float *a_cl = cuda_make_array(a, m*k);
    float *b_cl = cuda_make_array(b, k*n);
    float *c_cl = cuda_make_array(c, m*n);

    จำนวนเต็ม i;
    clock_t start = clock(), end;
    สำหรับ(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    dviguu_na flop = ((dviguu_na)m)*n*(2.*k + 2.)*iter;
    dviguu_na gflop = flop/pow(10., 9);
    end = clock();
    dviguu_na seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    free(a);
    free(b);
    free(c);
}


void test_gpu_accuracy(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม m, จำนวนเต็ม k, จำนวนเต็ม n)
{
    srand(0);
    float *a;
    ถ้า(!TA) a = random_matrix(m,k);
    มิฉะนั้น a = random_matrix(k,m);
    จำนวนเต็ม lda = (!TA)?k:m;
    float *b;
    ถ้า(!TB) b = random_matrix(k,n);
    มิฉะนั้น b = random_matrix(n,k);
    จำนวนเต็ม ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    float *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*parimaa_na(float));
    memset(c_gpu, 0, m*n*parimaa_na(float));
    จำนวนเต็ม i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    dviguu_na sse = 0;
    สำหรับ(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    free(a);
    free(b);
    free(c);
    free(c_gpu);
}

จำนวนเต็ม test_gpu_blas()
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

    คืน 0;
}
#endif

