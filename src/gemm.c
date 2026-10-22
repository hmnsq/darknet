#ಅನ್ತರ್ಭಾವ "gemm.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <math.h>

ಶೂನ್ಯ gemm_bin(ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಪೂರ್ಣಾಙ್ಕ K, ಭಿನ್ನ ALPHA, 
        ವರ್ಣ  *A, ಪೂರ್ಣಾಙ್ಕ lda, 
        ಭಿನ್ನ *B, ಪೂರ್ಣಾಙ್ಕ ldb,
        ಭಿನ್ನ *C, ಪೂರ್ಣಾಙ್ಕ ldc)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    ಪ್ರತಿ(i = 0; i < M; ++i){
        ಪ್ರತಿ(k = 0; k < K; ++k){
            ವರ್ಣ A_PART = A[i*lda+k];
            ಯದಿ(A_PART){
                ಪ್ರತಿ(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } ಅನ್ಯಥಾ {
                ಪ್ರತಿ(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

ಭಿನ್ನ *random_matrix(ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ *m = calloc(rows*cols, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪ್ರತಿ(i = 0; i < rows*cols; ++i){
        m[i] = (ಭಿನ್ನ)rand()/RAND_MAX;
    }
    ಪ್ರತ್ಯಾಗಮ m;
}

ಶೂನ್ಯ time_random_matrix(ಪೂರ್ಣಾಙ್ಕ TA, ಪೂರ್ಣಾಙ್ಕ TB, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಭಿನ್ನ *a;
    ಯದಿ(!TA) a = random_matrix(m,k);
    ಅನ್ಯಥಾ a = random_matrix(k,m);
    ಪೂರ್ಣಾಙ್ಕ lda = (!TA)?k:m;
    ಭಿನ್ನ *b;
    ಯದಿ(!TB) b = random_matrix(k,n);
    ಅನ್ಯಥಾ b = random_matrix(n,k);
    ಪೂರ್ಣಾಙ್ಕ ldb = (!TB)?n:k;

    ಭಿನ್ನ *c = random_matrix(m,n);
    ಪೂರ್ಣಾಙ್ಕ i;
    clock_t start = clock(), end;
    ಪ್ರತಿ(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (ಭಿನ್ನ)(end-start)/CLOCKS_PER_SEC);
    ಮುಕ್ತ(a);
    ಮುಕ್ತ(b);
    ಮುಕ್ತ(c);
}


ಶೂನ್ಯ gemm(ಪೂರ್ಣಾಙ್ಕ TA, ಪೂರ್ಣಾಙ್ಕ TB, ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಪೂರ್ಣಾಙ್ಕ K, ಭಿನ್ನ ALPHA, 
        ಭಿನ್ನ *A, ಪೂರ್ಣಾಙ್ಕ lda, 
        ಭಿನ್ನ *B, ಪೂರ್ಣಾಙ್ಕ ldb,
        ಭಿನ್ನ BETA,
        ಭಿನ್ನ *C, ಪೂರ್ಣಾಙ್ಕ ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

ಶೂನ್ಯ gemm_nn(ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಪೂರ್ಣಾಙ್ಕ K, ಭಿನ್ನ ALPHA, 
        ಭಿನ್ನ *A, ಪೂರ್ಣಾಙ್ಕ lda, 
        ಭಿನ್ನ *B, ಪೂರ್ಣಾಙ್ಕ ldb,
        ಭಿನ್ನ *C, ಪೂರ್ಣಾಙ್ಕ ldc)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    #pragma omp parallel ಪ್ರತಿ
    ಪ್ರತಿ(i = 0; i < M; ++i){
        ಪ್ರತಿ(k = 0; k < K; ++k){
            ಪಞ್ಜಿಕಾ ಭಿನ್ನ A_PART = ALPHA*A[i*lda+k];
            ಪ್ರತಿ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ಶೂನ್ಯ gemm_nt(ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಪೂರ್ಣಾಙ್ಕ K, ಭಿನ್ನ ALPHA, 
        ಭಿನ್ನ *A, ಪೂರ್ಣಾಙ್ಕ lda, 
        ಭಿನ್ನ *B, ಪೂರ್ಣಾಙ್ಕ ldb,
        ಭಿನ್ನ *C, ಪೂರ್ಣಾಙ್ಕ ldc)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    #pragma omp parallel ಪ್ರತಿ
    ಪ್ರತಿ(i = 0; i < M; ++i){
        ಪ್ರತಿ(j = 0; j < N; ++j){
            ಪಞ್ಜಿಕಾ ಭಿನ್ನ sum = 0;
            ಪ್ರತಿ(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

ಶೂನ್ಯ gemm_tn(ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಪೂರ್ಣಾಙ್ಕ K, ಭಿನ್ನ ALPHA, 
        ಭಿನ್ನ *A, ಪೂರ್ಣಾಙ್ಕ lda, 
        ಭಿನ್ನ *B, ಪೂರ್ಣಾಙ್ಕ ldb,
        ಭಿನ್ನ *C, ಪೂರ್ಣಾಙ್ಕ ldc)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    #pragma omp parallel ಪ್ರತಿ
    ಪ್ರತಿ(i = 0; i < M; ++i){
        ಪ್ರತಿ(k = 0; k < K; ++k){
            ಪಞ್ಜಿಕಾ ಭಿನ್ನ A_PART = ALPHA*A[k*lda+i];
            ಪ್ರತಿ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ಶೂನ್ಯ gemm_tt(ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಪೂರ್ಣಾಙ್ಕ K, ಭಿನ್ನ ALPHA, 
        ಭಿನ್ನ *A, ಪೂರ್ಣಾಙ್ಕ lda, 
        ಭಿನ್ನ *B, ಪೂರ್ಣಾಙ್ಕ ldb,
        ಭಿನ್ನ *C, ಪೂರ್ಣಾಙ್ಕ ldc)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    #pragma omp parallel ಪ್ರತಿ
    ಪ್ರತಿ(i = 0; i < M; ++i){
        ಪ್ರತಿ(j = 0; j < N; ++j){
            ಪಞ್ಜಿಕಾ ಭಿನ್ನ sum = 0;
            ಪ್ರತಿ(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


ಶೂನ್ಯ gemm_cpu(ಪೂರ್ಣಾಙ್ಕ TA, ಪೂರ್ಣಾಙ್ಕ TB, ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಪೂರ್ಣಾಙ್ಕ K, ಭಿನ್ನ ALPHA, 
        ಭಿನ್ನ *A, ಪೂರ್ಣಾಙ್ಕ lda, 
        ಭಿನ್ನ *B, ಪೂರ್ಣಾಙ್ಕ ldb,
        ಭಿನ್ನ BETA,
        ಭಿನ್ನ *C, ಪೂರ್ಣಾಙ್ಕ ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪ್ರತಿ(i = 0; i < M; ++i){
        ಪ್ರತಿ(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    ಯದಿ(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ಅನ್ಯಥಾ ಯದಿ(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ಅನ್ಯಥಾ ಯದಿ(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ಅನ್ಯಥಾ
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#ಅನ್ತರ್ಭಾವ <math.h>

ಶೂನ್ಯ gemm_gpu(ಪೂರ್ಣಾಙ್ಕ TA, ಪೂರ್ಣಾಙ್ಕ TB, ಪೂರ್ಣಾಙ್ಕ M, ಪೂರ್ಣಾಙ್ಕ N, ಪೂರ್ಣಾಙ್ಕ K, ಭಿನ್ನ ALPHA, 
        ಭಿನ್ನ *A_gpu, ಪೂರ್ಣಾಙ್ಕ lda, 
        ಭಿನ್ನ *B_gpu, ಪೂರ್ಣಾಙ್ಕ ldb,
        ಭಿನ್ನ BETA,
        ಭಿನ್ನ *C_gpu, ಪೂರ್ಣಾಙ್ಕ ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

ಶೂನ್ಯ time_gpu_random_matrix(ಪೂರ್ಣಾಙ್ಕ TA, ಪೂರ್ಣಾಙ್ಕ TB, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಭಿನ್ನ *a;
    ಯದಿ(!TA) a = random_matrix(m,k);
    ಅನ್ಯಥಾ a = random_matrix(k,m);
    ಪೂರ್ಣಾಙ್ಕ lda = (!TA)?k:m;
    ಭಿನ್ನ *b;
    ಯದಿ(!TB) b = random_matrix(k,n);
    ಅನ್ಯಥಾ b = random_matrix(n,k);
    ಪೂರ್ಣಾಙ್ಕ ldb = (!TB)?n:k;

    ಭಿನ್ನ *c = random_matrix(m,n);
    ಪೂರ್ಣಾಙ್ಕ i;
    clock_t start = clock(), end;
    ಪ್ರತಿ(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (ಭಿನ್ನ)(end-start)/CLOCKS_PER_SEC);
    ಮುಕ್ತ(a);
    ಮುಕ್ತ(b);
    ಮುಕ್ತ(c);
}

ಶೂನ್ಯ time_gpu(ಪೂರ್ಣಾಙ್ಕ TA, ಪೂರ್ಣಾಙ್ಕ TB, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ iter = 10;
    ಭಿನ್ನ *a = random_matrix(m,k);
    ಭಿನ್ನ *b = random_matrix(k,n);

    ಪೂರ್ಣಾಙ್ಕ lda = (!TA)?k:m;
    ಪೂರ್ಣಾಙ್ಕ ldb = (!TB)?n:k;

    ಭಿನ್ನ *c = random_matrix(m,n);

    ಭಿನ್ನ *a_cl = cuda_make_array(a, m*k);
    ಭಿನ್ನ *b_cl = cuda_make_array(b, k*n);
    ಭಿನ್ನ *c_cl = cuda_make_array(c, m*n);

    ಪೂರ್ಣಾಙ್ಕ i;
    clock_t start = clock(), end;
    ಪ್ರತಿ(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    ದ್ವಿಗುಣ flop = ((ದ್ವಿಗುಣ)m)*n*(2.*k + 2.)*iter;
    ದ್ವಿಗುಣ gflop = flop/pow(10., 9);
    end = clock();
    ದ್ವಿಗುಣ seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    ಮುಕ್ತ(a);
    ಮುಕ್ತ(b);
    ಮುಕ್ತ(c);
}


ಶೂನ್ಯ test_gpu_accuracy(ಪೂರ್ಣಾಙ್ಕ TA, ಪೂರ್ಣಾಙ್ಕ TB, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ n)
{
    srand(0);
    ಭಿನ್ನ *a;
    ಯದಿ(!TA) a = random_matrix(m,k);
    ಅನ್ಯಥಾ a = random_matrix(k,m);
    ಪೂರ್ಣಾಙ್ಕ lda = (!TA)?k:m;
    ಭಿನ್ನ *b;
    ಯದಿ(!TB) b = random_matrix(k,n);
    ಅನ್ಯಥಾ b = random_matrix(n,k);
    ಪೂರ್ಣಾಙ್ಕ ldb = (!TB)?n:k;

    ಭಿನ್ನ *c = random_matrix(m,n);
    ಭಿನ್ನ *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*ಪರಿಮಾಣ(ಭಿನ್ನ));
    memset(c_gpu, 0, m*n*ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪೂರ್ಣಾಙ್ಕ i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    ದ್ವಿಗುಣ sse = 0;
    ಪ್ರತಿ(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    ಮುಕ್ತ(a);
    ಮುಕ್ತ(b);
    ಮುಕ್ತ(c);
    ಮುಕ್ತ(c_gpu);
}

ಪೂರ್ಣಾಙ್ಕ test_gpu_blas()
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

    ಪ್ರತ್ಯಾಗಮ 0;
}
#endif

