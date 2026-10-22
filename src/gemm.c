#include "gemm.h"
#include "utils.h"
#include "cuda.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void gemm_bin(puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        var_na  *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float *C, puur_naa_nka ldc)
{
    puur_naa_nka i,j,k;
    ꦏꦁꦒꦺꦴ(i = 0; i < M; ++i){
        ꦏꦁꦒꦺꦴ(k = 0; k < K; ++k){
            var_na A_PART = A[i*lda+k];
            ꦪꦼꦤ꧀(A_PART){
                ꦏꦁꦒꦺꦴ(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } ꦭꦶꦪ {
                ꦏꦁꦒꦺꦴ(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

float *random_matrix(puur_naa_nka rows, puur_naa_nka cols)
{
    puur_naa_nka i;
    float *m = calloc(rows*cols, parimaa_na(float));
    ꦏꦁꦒꦺꦴ(i = 0; i < rows*cols; ++i){
        m[i] = (float)rand()/RAND_MAX;
    }
    ꦮꦭꦶ m;
}

void time_random_matrix(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka m, puur_naa_nka k, puur_naa_nka n)
{
    float *a;
    ꦪꦼꦤ꧀(!TA) a = random_matrix(m,k);
    ꦭꦶꦪ a = random_matrix(k,m);
    puur_naa_nka lda = (!TA)?k:m;
    float *b;
    ꦪꦼꦤ꧀(!TB) b = random_matrix(k,n);
    ꦭꦶꦪ b = random_matrix(n,k);
    puur_naa_nka ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    puur_naa_nka i;
    clock_t start = clock(), end;
    ꦏꦁꦒꦺꦴ(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (float)(end-start)/CLOCKS_PER_SEC);
    free(a);
    free(b);
    free(c);
}


void gemm(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float BETA,
        float *C, puur_naa_nka ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

void gemm_nn(puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float *C, puur_naa_nka ldc)
{
    puur_naa_nka i,j,k;
    #pragma omp parallel ꦏꦁꦒꦺꦴ
    ꦏꦁꦒꦺꦴ(i = 0; i < M; ++i){
        ꦏꦁꦒꦺꦴ(k = 0; k < K; ++k){
            pa_njikaa float A_PART = ALPHA*A[i*lda+k];
            ꦏꦁꦒꦺꦴ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

void gemm_nt(puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float *C, puur_naa_nka ldc)
{
    puur_naa_nka i,j,k;
    #pragma omp parallel ꦏꦁꦒꦺꦴ
    ꦏꦁꦒꦺꦴ(i = 0; i < M; ++i){
        ꦏꦁꦒꦺꦴ(j = 0; j < N; ++j){
            pa_njikaa float sum = 0;
            ꦏꦁꦒꦺꦴ(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

void gemm_tn(puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float *C, puur_naa_nka ldc)
{
    puur_naa_nka i,j,k;
    #pragma omp parallel ꦏꦁꦒꦺꦴ
    ꦏꦁꦒꦺꦴ(i = 0; i < M; ++i){
        ꦏꦁꦒꦺꦴ(k = 0; k < K; ++k){
            pa_njikaa float A_PART = ALPHA*A[k*lda+i];
            ꦏꦁꦒꦺꦴ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

void gemm_tt(puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float *C, puur_naa_nka ldc)
{
    puur_naa_nka i,j,k;
    #pragma omp parallel ꦏꦁꦒꦺꦴ
    ꦏꦁꦒꦺꦴ(i = 0; i < M; ++i){
        ꦏꦁꦒꦺꦴ(j = 0; j < N; ++j){
            pa_njikaa float sum = 0;
            ꦏꦁꦒꦺꦴ(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


void gemm_cpu(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float BETA,
        float *C, puur_naa_nka ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    puur_naa_nka i, j;
    ꦏꦁꦒꦺꦴ(i = 0; i < M; ++i){
        ꦏꦁꦒꦺꦴ(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    ꦪꦼꦤ꧀(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ꦭꦶꦪ ꦪꦼꦤ꧀(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ꦭꦶꦪ ꦪꦼꦤ꧀(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ꦭꦶꦪ
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#include <math.h>

void gemm_gpu(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A_gpu, puur_naa_nka lda, 
        float *B_gpu, puur_naa_nka ldb,
        float BETA,
        float *C_gpu, puur_naa_nka ldc)
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

void time_gpu_random_matrix(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka m, puur_naa_nka k, puur_naa_nka n)
{
    float *a;
    ꦪꦼꦤ꧀(!TA) a = random_matrix(m,k);
    ꦭꦶꦪ a = random_matrix(k,m);
    puur_naa_nka lda = (!TA)?k:m;
    float *b;
    ꦪꦼꦤ꧀(!TB) b = random_matrix(k,n);
    ꦭꦶꦪ b = random_matrix(n,k);
    puur_naa_nka ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    puur_naa_nka i;
    clock_t start = clock(), end;
    ꦏꦁꦒꦺꦴ(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (float)(end-start)/CLOCKS_PER_SEC);
    free(a);
    free(b);
    free(c);
}

void time_gpu(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka m, puur_naa_nka k, puur_naa_nka n)
{
    puur_naa_nka iter = 10;
    float *a = random_matrix(m,k);
    float *b = random_matrix(k,n);

    puur_naa_nka lda = (!TA)?k:m;
    puur_naa_nka ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);

    float *a_cl = cuda_make_array(a, m*k);
    float *b_cl = cuda_make_array(b, k*n);
    float *c_cl = cuda_make_array(c, m*n);

    puur_naa_nka i;
    clock_t start = clock(), end;
    ꦏꦁꦒꦺꦴ(i = 0; i<iter; ++i){
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


void test_gpu_accuracy(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka m, puur_naa_nka k, puur_naa_nka n)
{
    srand(0);
    float *a;
    ꦪꦼꦤ꧀(!TA) a = random_matrix(m,k);
    ꦭꦶꦪ a = random_matrix(k,m);
    puur_naa_nka lda = (!TA)?k:m;
    float *b;
    ꦪꦼꦤ꧀(!TB) b = random_matrix(k,n);
    ꦭꦶꦪ b = random_matrix(n,k);
    puur_naa_nka ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    float *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*parimaa_na(float));
    memset(c_gpu, 0, m*n*parimaa_na(float));
    puur_naa_nka i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    dviguu_na sse = 0;
    ꦏꦁꦒꦺꦴ(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    free(a);
    free(b);
    free(c);
    free(c_gpu);
}

puur_naa_nka test_gpu_blas()
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

    ꦮꦭꦶ 0;
}
#endif

