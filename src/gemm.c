#ಅನ್ತರ್ಭಾವ "gemm.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <math.h>

ಶೂನ್ಯ gemm_bin(ಪೂರ್ಣಾಂಕ M, ಪೂರ್ಣಾಂಕ N, ಪೂರ್ಣಾಂಕ K, ದಶಮಾಂಶ ALPHA, 
        ಅಕ್ಷರ  *A, ಪೂರ್ಣಾಂಕ lda, 
        ದಶಮಾಂಶ *B, ಪೂರ್ಣಾಂಕ ldb,
        ದಶಮಾಂಶ *C, ಪೂರ್ಣಾಂಕ ldc)
{
    ಪೂರ್ಣಾಂಕ i,j,k;
    ಗಾಗಿ(i = 0; i < M; ++i){
        ಗಾಗಿ(k = 0; k < K; ++k){
            ಅಕ್ಷರ A_PART = A[i*lda+k];
            ಆದರೆ(A_PART){
                ಗಾಗಿ(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಗಾಗಿ(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

ದಶಮಾಂಶ *random_ಮಾತೃಕೆ(ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ)
{
    ಪೂರ್ಣಾಂಕ i;
    ದಶಮಾಂಶ *m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಅಡ್ಡಸಾಲುಗಳು*ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಗಾಗಿ(i = 0; i < ಅಡ್ಡಸಾಲುಗಳು*ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ++i){
        m[i] = (ದಶಮಾಂಶ)rand()/RAND_MAX;
    }
    ಹಿಂತಿರುಗಿ m;
}

ಶೂನ್ಯ ಸಮಯ_random_ಮಾತೃಕೆ(ಪೂರ್ಣಾಂಕ TA, ಪೂರ್ಣಾಂಕ TB, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ n)
{
    ದಶಮಾಂಶ *a;
    ಆದರೆ(!TA) a = random_ಮಾತೃಕೆ(m,k);
    ಇಲ್ಲದಿದ್ದರೆ a = random_ಮಾತೃಕೆ(k,m);
    ಪೂರ್ಣಾಂಕ lda = (!TA)?k:m;
    ದಶಮಾಂಶ *b;
    ಆದರೆ(!TB) b = random_ಮಾತೃಕೆ(k,n);
    ಇಲ್ಲದಿದ್ದರೆ b = random_ಮಾತೃಕೆ(n,k);
    ಪೂರ್ಣಾಂಕ ldb = (!TB)?n:k;

    ದಶಮಾಂಶ *c = random_ಮಾತೃಕೆ(m,n);
    ಪೂರ್ಣಾಂಕ i;
    clock_t ಆರಂಭ = clock(), ಕೊನೆ;
    ಗಾಗಿ(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    ಕೊನೆ = clock();
    ಬರೆ("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (ದಶಮಾಂಶ)(ಕೊನೆ-ಆರಂಭ)/CLOCKS_PER_SEC);
    ಬಿಡುಗಡೆ(a);
    ಬಿಡುಗಡೆ(b);
    ಬಿಡುಗಡೆ(c);
}


ಶೂನ್ಯ gemm(ಪೂರ್ಣಾಂಕ TA, ಪೂರ್ಣಾಂಕ TB, ಪೂರ್ಣಾಂಕ M, ಪೂರ್ಣಾಂಕ N, ಪೂರ್ಣಾಂಕ K, ದಶಮಾಂಶ ALPHA, 
        ದಶಮಾಂಶ *A, ಪೂರ್ಣಾಂಕ lda, 
        ದಶಮಾಂಶ *B, ಪೂರ್ಣಾಂಕ ldb,
        ದಶಮಾಂಶ BETA,
        ದಶಮಾಂಶ *C, ಪೂರ್ಣಾಂಕ ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

ಶೂನ್ಯ gemm_nn(ಪೂರ್ಣಾಂಕ M, ಪೂರ್ಣಾಂಕ N, ಪೂರ್ಣಾಂಕ K, ದಶಮಾಂಶ ALPHA, 
        ದಶಮಾಂಶ *A, ಪೂರ್ಣಾಂಕ lda, 
        ದಶಮಾಂಶ *B, ಪೂರ್ಣಾಂಕ ldb,
        ದಶಮಾಂಶ *C, ಪೂರ್ಣಾಂಕ ldc)
{
    ಪೂರ್ಣಾಂಕ i,j,k;
    #ಸೂಚನೆ omp parallel ಗಾಗಿ
    ಗಾಗಿ(i = 0; i < M; ++i){
        ಗಾಗಿ(k = 0; k < K; ++k){
            ನೋಂದಣಿ ದಶಮಾಂಶ A_PART = ALPHA*A[i*lda+k];
            ಗಾಗಿ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ಶೂನ್ಯ gemm_nt(ಪೂರ್ಣಾಂಕ M, ಪೂರ್ಣಾಂಕ N, ಪೂರ್ಣಾಂಕ K, ದಶಮಾಂಶ ALPHA, 
        ದಶಮಾಂಶ *A, ಪೂರ್ಣಾಂಕ lda, 
        ದಶಮಾಂಶ *B, ಪೂರ್ಣಾಂಕ ldb,
        ದಶಮಾಂಶ *C, ಪೂರ್ಣಾಂಕ ldc)
{
    ಪೂರ್ಣಾಂಕ i,j,k;
    #ಸೂಚನೆ omp parallel ಗಾಗಿ
    ಗಾಗಿ(i = 0; i < M; ++i){
        ಗಾಗಿ(j = 0; j < N; ++j){
            ನೋಂದಣಿ ದಶಮಾಂಶ ಮೊತ್ತ = 0;
            ಗಾಗಿ(k = 0; k < K; ++k){
                ಮೊತ್ತ += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += ಮೊತ್ತ;
        }
    }
}

ಶೂನ್ಯ gemm_tn(ಪೂರ್ಣಾಂಕ M, ಪೂರ್ಣಾಂಕ N, ಪೂರ್ಣಾಂಕ K, ದಶಮಾಂಶ ALPHA, 
        ದಶಮಾಂಶ *A, ಪೂರ್ಣಾಂಕ lda, 
        ದಶಮಾಂಶ *B, ಪೂರ್ಣಾಂಕ ldb,
        ದಶಮಾಂಶ *C, ಪೂರ್ಣಾಂಕ ldc)
{
    ಪೂರ್ಣಾಂಕ i,j,k;
    #ಸೂಚನೆ omp parallel ಗಾಗಿ
    ಗಾಗಿ(i = 0; i < M; ++i){
        ಗಾಗಿ(k = 0; k < K; ++k){
            ನೋಂದಣಿ ದಶಮಾಂಶ A_PART = ALPHA*A[k*lda+i];
            ಗಾಗಿ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ಶೂನ್ಯ gemm_tt(ಪೂರ್ಣಾಂಕ M, ಪೂರ್ಣಾಂಕ N, ಪೂರ್ಣಾಂಕ K, ದಶಮಾಂಶ ALPHA, 
        ದಶಮಾಂಶ *A, ಪೂರ್ಣಾಂಕ lda, 
        ದಶಮಾಂಶ *B, ಪೂರ್ಣಾಂಕ ldb,
        ದಶಮಾಂಶ *C, ಪೂರ್ಣಾಂಕ ldc)
{
    ಪೂರ್ಣಾಂಕ i,j,k;
    #ಸೂಚನೆ omp parallel ಗಾಗಿ
    ಗಾಗಿ(i = 0; i < M; ++i){
        ಗಾಗಿ(j = 0; j < N; ++j){
            ನೋಂದಣಿ ದಶಮಾಂಶ ಮೊತ್ತ = 0;
            ಗಾಗಿ(k = 0; k < K; ++k){
                ಮೊತ್ತ += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += ಮೊತ್ತ;
        }
    }
}


ಶೂನ್ಯ gemm_cpu(ಪೂರ್ಣಾಂಕ TA, ಪೂರ್ಣಾಂಕ TB, ಪೂರ್ಣಾಂಕ M, ಪೂರ್ಣಾಂಕ N, ಪೂರ್ಣಾಂಕ K, ದಶಮಾಂಶ ALPHA, 
        ದಶಮಾಂಶ *A, ಪೂರ್ಣಾಂಕ lda, 
        ದಶಮಾಂಶ *B, ಪೂರ್ಣಾಂಕ ldb,
        ದಶಮಾಂಶ BETA,
        ದಶಮಾಂಶ *C, ಪೂರ್ಣಾಂಕ ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    ಪೂರ್ಣಾಂಕ i, j;
    ಗಾಗಿ(i = 0; i < M; ++i){
        ಗಾಗಿ(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    ಆದರೆ(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ಇಲ್ಲದಿದ್ದರೆ
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

#ಅನ್ತರ್ಭಾವ <math.h>

ಶೂನ್ಯ gemm_gpu(ಪೂರ್ಣಾಂಕ TA, ಪೂರ್ಣಾಂಕ TB, ಪೂರ್ಣಾಂಕ M, ಪೂರ್ಣಾಂಕ N, ಪೂರ್ಣಾಂಕ K, ದಶಮಾಂಶ ALPHA, 
        ದಶಮಾಂಶ *A_gpu, ಪೂರ್ಣಾಂಕ lda, 
        ದಶಮಾಂಶ *B_gpu, ಪೂರ್ಣಾಂಕ ldb,
        ದಶಮಾಂಶ BETA,
        ದಶಮಾಂಶ *C_gpu, ಪೂರ್ಣಾಂಕ ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t ಸ್ಥಿತಿ_2 = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(ಸ್ಥಿತಿ_2);
}

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <time.h>

ಶೂನ್ಯ ಸಮಯ_gpu_random_ಮಾತೃಕೆ(ಪೂರ್ಣಾಂಕ TA, ಪೂರ್ಣಾಂಕ TB, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ n)
{
    ದಶಮಾಂಶ *a;
    ಆದರೆ(!TA) a = random_ಮಾತೃಕೆ(m,k);
    ಇಲ್ಲದಿದ್ದರೆ a = random_ಮಾತೃಕೆ(k,m);
    ಪೂರ್ಣಾಂಕ lda = (!TA)?k:m;
    ದಶಮಾಂಶ *b;
    ಆದರೆ(!TB) b = random_ಮಾತೃಕೆ(k,n);
    ಇಲ್ಲದಿದ್ದರೆ b = random_ಮಾತೃಕೆ(n,k);
    ಪೂರ್ಣಾಂಕ ldb = (!TB)?n:k;

    ದಶಮಾಂಶ *c = random_ಮಾತೃಕೆ(m,n);
    ಪೂರ್ಣಾಂಕ i;
    clock_t ಆರಂಭ = clock(), ಕೊನೆ;
    ಗಾಗಿ(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    ಕೊನೆ = clock();
    ಬರೆ("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (ದಶಮಾಂಶ)(ಕೊನೆ-ಆರಂಭ)/CLOCKS_PER_SEC);
    ಬಿಡುಗಡೆ(a);
    ಬಿಡುಗಡೆ(b);
    ಬಿಡುಗಡೆ(c);
}

ಶೂನ್ಯ ಸಮಯ_gpu(ಪೂರ್ಣಾಂಕ TA, ಪೂರ್ಣಾಂಕ TB, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ iter = 10;
    ದಶಮಾಂಶ *a = random_ಮಾತೃಕೆ(m,k);
    ದಶಮಾಂಶ *b = random_ಮಾತೃಕೆ(k,n);

    ಪೂರ್ಣಾಂಕ lda = (!TA)?k:m;
    ಪೂರ್ಣಾಂಕ ldb = (!TB)?n:k;

    ದಶಮಾಂಶ *c = random_ಮಾತೃಕೆ(m,n);

    ದಶಮಾಂಶ *a_cl = cuda_make_array(a, m*k);
    ದಶಮಾಂಶ *b_cl = cuda_make_array(b, k*n);
    ದಶಮಾಂಶ *c_cl = cuda_make_array(c, m*n);

    ಪೂರ್ಣಾಂಕ i;
    clock_t ಆರಂಭ = clock(), ಕೊನೆ;
    ಗಾಗಿ(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    ದ್ವಿಗುಣ flop = ((ದ್ವಿಗುಣ)m)*n*(2.*k + 2.)*iter;
    ದ್ವಿಗುಣ gflop = flop/ಘಾತ(10., 9);
    ಕೊನೆ = clock();
    ದ್ವಿಗುಣ seconds = sec(ಕೊನೆ-ಆರಂಭ);
    ಬರೆ("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    ಬಿಡುಗಡೆ(a);
    ಬಿಡುಗಡೆ(b);
    ಬಿಡುಗಡೆ(c);
}


ಶೂನ್ಯ test_gpu_accuracy(ಪೂರ್ಣಾಂಕ TA, ಪೂರ್ಣಾಂಕ TB, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ n)
{
    srand(0);
    ದಶಮಾಂಶ *a;
    ಆದರೆ(!TA) a = random_ಮಾತೃಕೆ(m,k);
    ಇಲ್ಲದಿದ್ದರೆ a = random_ಮಾತೃಕೆ(k,m);
    ಪೂರ್ಣಾಂಕ lda = (!TA)?k:m;
    ದಶಮಾಂಶ *b;
    ಆದರೆ(!TB) b = random_ಮಾತೃಕೆ(k,n);
    ಇಲ್ಲದಿದ್ದರೆ b = random_ಮಾತೃಕೆ(n,k);
    ಪೂರ್ಣಾಂಕ ldb = (!TB)?n:k;

    ದಶಮಾಂಶ *c = random_ಮಾತೃಕೆ(m,n);
    ದಶಮಾಂಶ *c_gpu = random_ಮಾತೃಕೆ(m,n);
    memset(c, 0, m*n*ಗಾತ್ರ(ದಶಮಾಂಶ));
    memset(c_gpu, 0, m*n*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಪೂರ್ಣಾಂಕ i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    ದ್ವಿಗುಣ sse = 0;
    ಗಾಗಿ(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += ಘಾತ(c[i]-c_gpu[i], 2);
    }
    ಬರೆ("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    ಬಿಡುಗಡೆ(a);
    ಬಿಡುಗಡೆ(b);
    ಬಿಡುಗಡೆ(c);
    ಬಿಡುಗಡೆ(c_gpu);
}

ಪೂರ್ಣಾಂಕ test_gpu_blas()
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
    ಸಮಯ_gpu(0,0,64,75,12544); 
    ಸಮಯ_gpu(0,0,64,75,12544); 
    ಸಮಯ_gpu(0,0,64,75,12544); 
    ಸಮಯ_gpu(0,0,64,576,12544); 
    ಸಮಯ_gpu(0,0,256,2304,784); 
    ಸಮಯ_gpu(1,1,2304,256,784); 
    ಸಮಯ_gpu(0,0,512,4608,196); 
    ಸಮಯ_gpu(1,1,4608,512,196); 

    ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ

