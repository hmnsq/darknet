#include "gemm.h"
#include "utils.h"
#include "cuda.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void gemm_bin(ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        var_na  *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float *C, ຈຳນວນເຕັມ ldc)
{
    ຈຳນວນເຕັມ i,j,k;
    ສຳລັບ(i = 0; i < M; ++i){
        ສຳລັບ(k = 0; k < K; ++k){
            var_na A_PART = A[i*lda+k];
            ຖ້າ(A_PART){
                ສຳລັບ(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } ບໍ່ດັ່ງນັ້ນ {
                ສຳລັບ(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

float *random_matrix(ຈຳນວນເຕັມ rows, ຈຳນວນເຕັມ cols)
{
    ຈຳນວນເຕັມ i;
    float *m = calloc(rows*cols, parimaa_na(float));
    ສຳລັບ(i = 0; i < rows*cols; ++i){
        m[i] = (float)rand()/RAND_MAX;
    }
    ສົ່ງຄືນ m;
}

void time_random_matrix(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ k, ຈຳນວນເຕັມ n)
{
    float *a;
    ຖ້າ(!TA) a = random_matrix(m,k);
    ບໍ່ດັ່ງນັ້ນ a = random_matrix(k,m);
    ຈຳນວນເຕັມ lda = (!TA)?k:m;
    float *b;
    ຖ້າ(!TB) b = random_matrix(k,n);
    ບໍ່ດັ່ງນັ້ນ b = random_matrix(n,k);
    ຈຳນວນເຕັມ ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    ຈຳນວນເຕັມ i;
    clock_t start = clock(), end;
    ສຳລັບ(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (float)(end-start)/CLOCKS_PER_SEC);
    free(a);
    free(b);
    free(c);
}


void gemm(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float BETA,
        float *C, ຈຳນວນເຕັມ ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

void gemm_nn(ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float *C, ຈຳນວນເຕັມ ldc)
{
    ຈຳນວນເຕັມ i,j,k;
    #pragma omp parallel ສຳລັບ
    ສຳລັບ(i = 0; i < M; ++i){
        ສຳລັບ(k = 0; k < K; ++k){
            pa_njikaa float A_PART = ALPHA*A[i*lda+k];
            ສຳລັບ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

void gemm_nt(ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float *C, ຈຳນວນເຕັມ ldc)
{
    ຈຳນວນເຕັມ i,j,k;
    #pragma omp parallel ສຳລັບ
    ສຳລັບ(i = 0; i < M; ++i){
        ສຳລັບ(j = 0; j < N; ++j){
            pa_njikaa float sum = 0;
            ສຳລັບ(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

void gemm_tn(ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float *C, ຈຳນວນເຕັມ ldc)
{
    ຈຳນວນເຕັມ i,j,k;
    #pragma omp parallel ສຳລັບ
    ສຳລັບ(i = 0; i < M; ++i){
        ສຳລັບ(k = 0; k < K; ++k){
            pa_njikaa float A_PART = ALPHA*A[k*lda+i];
            ສຳລັບ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

void gemm_tt(ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float *C, ຈຳນວນເຕັມ ldc)
{
    ຈຳນວນເຕັມ i,j,k;
    #pragma omp parallel ສຳລັບ
    ສຳລັບ(i = 0; i < M; ++i){
        ສຳລັບ(j = 0; j < N; ++j){
            pa_njikaa float sum = 0;
            ສຳລັບ(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


void gemm_cpu(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float BETA,
        float *C, ຈຳນວນເຕັມ ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    ຈຳນວນເຕັມ i, j;
    ສຳລັບ(i = 0; i < M; ++i){
        ສຳລັບ(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    ຖ້າ(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ບໍ່ດັ່ງນັ້ນ
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#include <math.h>

void gemm_gpu(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A_gpu, ຈຳນວນເຕັມ lda, 
        float *B_gpu, ຈຳນວນເຕັມ ldb,
        float BETA,
        float *C_gpu, ຈຳນວນເຕັມ ldc)
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

void time_gpu_random_matrix(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ k, ຈຳນວນເຕັມ n)
{
    float *a;
    ຖ້າ(!TA) a = random_matrix(m,k);
    ບໍ່ດັ່ງນັ້ນ a = random_matrix(k,m);
    ຈຳນວນເຕັມ lda = (!TA)?k:m;
    float *b;
    ຖ້າ(!TB) b = random_matrix(k,n);
    ບໍ່ດັ່ງນັ້ນ b = random_matrix(n,k);
    ຈຳນວນເຕັມ ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    ຈຳນວນເຕັມ i;
    clock_t start = clock(), end;
    ສຳລັບ(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (float)(end-start)/CLOCKS_PER_SEC);
    free(a);
    free(b);
    free(c);
}

void time_gpu(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ k, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ iter = 10;
    float *a = random_matrix(m,k);
    float *b = random_matrix(k,n);

    ຈຳນວນເຕັມ lda = (!TA)?k:m;
    ຈຳນວນເຕັມ ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);

    float *a_cl = cuda_make_array(a, m*k);
    float *b_cl = cuda_make_array(b, k*n);
    float *c_cl = cuda_make_array(c, m*n);

    ຈຳນວນເຕັມ i;
    clock_t start = clock(), end;
    ສຳລັບ(i = 0; i<iter; ++i){
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


void test_gpu_accuracy(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ k, ຈຳນວນເຕັມ n)
{
    srand(0);
    float *a;
    ຖ້າ(!TA) a = random_matrix(m,k);
    ບໍ່ດັ່ງນັ້ນ a = random_matrix(k,m);
    ຈຳນວນເຕັມ lda = (!TA)?k:m;
    float *b;
    ຖ້າ(!TB) b = random_matrix(k,n);
    ບໍ່ດັ່ງນັ້ນ b = random_matrix(n,k);
    ຈຳນວນເຕັມ ldb = (!TB)?n:k;

    float *c = random_matrix(m,n);
    float *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*parimaa_na(float));
    memset(c_gpu, 0, m*n*parimaa_na(float));
    ຈຳນວນເຕັມ i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    dviguu_na sse = 0;
    ສຳລັບ(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    free(a);
    free(b);
    free(c);
    free(c_gpu);
}

ຈຳນວນເຕັມ test_gpu_blas()
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

    ສົ່ງຄືນ 0;
}
#endif

