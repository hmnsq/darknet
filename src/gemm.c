#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>

ਖਾਲੀ gemm_bin(ਪੂਰਨ M, ਪੂਰਨ N, ਪੂਰਨ K, ਦਸ਼ਮਲਵ ALPHA, 
        ਅੱਖਰ  *A, ਪੂਰਨ lda, 
        ਦਸ਼ਮਲਵ *B, ਪੂਰਨ ldb,
        ਦਸ਼ਮਲਵ *C, ਪੂਰਨ ldc)
{
    ਪੂਰਨ i,j,k;
    ਲਈ(i = 0; i < M; ++i){
        ਲਈ(k = 0; k < K; ++k){
            ਅੱਖਰ A_PART = A[i*lda+k];
            ਜੇ(A_PART){
                ਲਈ(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } ਨਹੀਂਤਾਂ {
                ਲਈ(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

ਦਸ਼ਮਲਵ *random_ਮੈਟ੍ਰਿਕਸ(ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ *m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਪੰਕਤੀਆਂ*ਖਾਨੇ_ਗਿਣਤੀ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਲਈ(i = 0; i < ਪੰਕਤੀਆਂ*ਖਾਨੇ_ਗਿਣਤੀ; ++i){
        m[i] = (ਦਸ਼ਮਲਵ)rand()/RAND_MAX;
    }
    ਵਾਪਸ m;
}

ਖਾਲੀ ਸਮਾਂ_random_ਮੈਟ੍ਰਿਕਸ(ਪੂਰਨ TA, ਪੂਰਨ TB, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ n)
{
    ਦਸ਼ਮਲਵ *a;
    ਜੇ(!TA) a = random_ਮੈਟ੍ਰਿਕਸ(m,k);
    ਨਹੀਂਤਾਂ a = random_ਮੈਟ੍ਰਿਕਸ(k,m);
    ਪੂਰਨ lda = (!TA)?k:m;
    ਦਸ਼ਮਲਵ *b;
    ਜੇ(!TB) b = random_ਮੈਟ੍ਰਿਕਸ(k,n);
    ਨਹੀਂਤਾਂ b = random_ਮੈਟ੍ਰਿਕਸ(n,k);
    ਪੂਰਨ ldb = (!TB)?n:k;

    ਦਸ਼ਮਲਵ *c = random_ਮੈਟ੍ਰਿਕਸ(m,n);
    ਪੂਰਨ i;
    clock_t ਸ਼ੁਰੂ = clock(), ਸਿਰਾ;
    ਲਈ(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    ਸਿਰਾ = clock();
    ਲਿਖੋ("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (ਦਸ਼ਮਲਵ)(ਸਿਰਾ-ਸ਼ੁਰੂ)/CLOCKS_PER_SEC);
    ਮੁਕਤ(a);
    ਮੁਕਤ(b);
    ਮੁਕਤ(c);
}


ਖਾਲੀ gemm(ਪੂਰਨ TA, ਪੂਰਨ TB, ਪੂਰਨ M, ਪੂਰਨ N, ਪੂਰਨ K, ਦਸ਼ਮਲਵ ALPHA, 
        ਦਸ਼ਮਲਵ *A, ਪੂਰਨ lda, 
        ਦਸ਼ਮਲਵ *B, ਪੂਰਨ ldb,
        ਦਸ਼ਮਲਵ BETA,
        ਦਸ਼ਮਲਵ *C, ਪੂਰਨ ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

ਖਾਲੀ gemm_nn(ਪੂਰਨ M, ਪੂਰਨ N, ਪੂਰਨ K, ਦਸ਼ਮਲਵ ALPHA, 
        ਦਸ਼ਮਲਵ *A, ਪੂਰਨ lda, 
        ਦਸ਼ਮਲਵ *B, ਪੂਰਨ ldb,
        ਦਸ਼ਮਲਵ *C, ਪੂਰਨ ldc)
{
    ਪੂਰਨ i,j,k;
    #ਹਦਾਇਤ omp parallel ਲਈ
    ਲਈ(i = 0; i < M; ++i){
        ਲਈ(k = 0; k < K; ++k){
            ਰਜਿਸਟਰ ਦਸ਼ਮਲਵ A_PART = ALPHA*A[i*lda+k];
            ਲਈ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ਖਾਲੀ gemm_nt(ਪੂਰਨ M, ਪੂਰਨ N, ਪੂਰਨ K, ਦਸ਼ਮਲਵ ALPHA, 
        ਦਸ਼ਮਲਵ *A, ਪੂਰਨ lda, 
        ਦਸ਼ਮਲਵ *B, ਪੂਰਨ ldb,
        ਦਸ਼ਮਲਵ *C, ਪੂਰਨ ldc)
{
    ਪੂਰਨ i,j,k;
    #ਹਦਾਇਤ omp parallel ਲਈ
    ਲਈ(i = 0; i < M; ++i){
        ਲਈ(j = 0; j < N; ++j){
            ਰਜਿਸਟਰ ਦਸ਼ਮਲਵ ਜੋੜ = 0;
            ਲਈ(k = 0; k < K; ++k){
                ਜੋੜ += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += ਜੋੜ;
        }
    }
}

ਖਾਲੀ gemm_tn(ਪੂਰਨ M, ਪੂਰਨ N, ਪੂਰਨ K, ਦਸ਼ਮਲਵ ALPHA, 
        ਦਸ਼ਮਲਵ *A, ਪੂਰਨ lda, 
        ਦਸ਼ਮਲਵ *B, ਪੂਰਨ ldb,
        ਦਸ਼ਮਲਵ *C, ਪੂਰਨ ldc)
{
    ਪੂਰਨ i,j,k;
    #ਹਦਾਇਤ omp parallel ਲਈ
    ਲਈ(i = 0; i < M; ++i){
        ਲਈ(k = 0; k < K; ++k){
            ਰਜਿਸਟਰ ਦਸ਼ਮਲਵ A_PART = ALPHA*A[k*lda+i];
            ਲਈ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ਖਾਲੀ gemm_tt(ਪੂਰਨ M, ਪੂਰਨ N, ਪੂਰਨ K, ਦਸ਼ਮਲਵ ALPHA, 
        ਦਸ਼ਮਲਵ *A, ਪੂਰਨ lda, 
        ਦਸ਼ਮਲਵ *B, ਪੂਰਨ ldb,
        ਦਸ਼ਮਲਵ *C, ਪੂਰਨ ldc)
{
    ਪੂਰਨ i,j,k;
    #ਹਦਾਇਤ omp parallel ਲਈ
    ਲਈ(i = 0; i < M; ++i){
        ਲਈ(j = 0; j < N; ++j){
            ਰਜਿਸਟਰ ਦਸ਼ਮਲਵ ਜੋੜ = 0;
            ਲਈ(k = 0; k < K; ++k){
                ਜੋੜ += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += ਜੋੜ;
        }
    }
}


ਖਾਲੀ gemm_cpu(ਪੂਰਨ TA, ਪੂਰਨ TB, ਪੂਰਨ M, ਪੂਰਨ N, ਪੂਰਨ K, ਦਸ਼ਮਲਵ ALPHA, 
        ਦਸ਼ਮਲਵ *A, ਪੂਰਨ lda, 
        ਦਸ਼ਮਲਵ *B, ਪੂਰਨ ldb,
        ਦਸ਼ਮਲਵ BETA,
        ਦਸ਼ਮਲਵ *C, ਪੂਰਨ ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    ਪੂਰਨ i, j;
    ਲਈ(i = 0; i < M; ++i){
        ਲਈ(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    ਜੇ(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ਨਹੀਂਤਾਂ ਜੇ(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ਨਹੀਂਤਾਂ ਜੇ(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ਨਹੀਂਤਾਂ
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>

ਖਾਲੀ gemm_gpu(ਪੂਰਨ TA, ਪੂਰਨ TB, ਪੂਰਨ M, ਪੂਰਨ N, ਪੂਰਨ K, ਦਸ਼ਮਲਵ ALPHA, 
        ਦਸ਼ਮਲਵ *A_gpu, ਪੂਰਨ lda, 
        ਦਸ਼ਮਲਵ *B_gpu, ਪੂਰਨ ldb,
        ਦਸ਼ਮਲਵ BETA,
        ਦਸ਼ਮਲਵ *C_gpu, ਪੂਰਨ ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t ਸਥਿਤੀ = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(ਸਥਿਤੀ);
}

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

ਖਾਲੀ ਸਮਾਂ_gpu_random_ਮੈਟ੍ਰਿਕਸ(ਪੂਰਨ TA, ਪੂਰਨ TB, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ n)
{
    ਦਸ਼ਮਲਵ *a;
    ਜੇ(!TA) a = random_ਮੈਟ੍ਰਿਕਸ(m,k);
    ਨਹੀਂਤਾਂ a = random_ਮੈਟ੍ਰਿਕਸ(k,m);
    ਪੂਰਨ lda = (!TA)?k:m;
    ਦਸ਼ਮਲਵ *b;
    ਜੇ(!TB) b = random_ਮੈਟ੍ਰਿਕਸ(k,n);
    ਨਹੀਂਤਾਂ b = random_ਮੈਟ੍ਰਿਕਸ(n,k);
    ਪੂਰਨ ldb = (!TB)?n:k;

    ਦਸ਼ਮਲਵ *c = random_ਮੈਟ੍ਰਿਕਸ(m,n);
    ਪੂਰਨ i;
    clock_t ਸ਼ੁਰੂ = clock(), ਸਿਰਾ;
    ਲਈ(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    ਸਿਰਾ = clock();
    ਲਿਖੋ("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (ਦਸ਼ਮਲਵ)(ਸਿਰਾ-ਸ਼ੁਰੂ)/CLOCKS_PER_SEC);
    ਮੁਕਤ(a);
    ਮੁਕਤ(b);
    ਮੁਕਤ(c);
}

ਖਾਲੀ ਸਮਾਂ_gpu(ਪੂਰਨ TA, ਪੂਰਨ TB, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ n)
{
    ਪੂਰਨ iter = 10;
    ਦਸ਼ਮਲਵ *a = random_ਮੈਟ੍ਰਿਕਸ(m,k);
    ਦਸ਼ਮਲਵ *b = random_ਮੈਟ੍ਰਿਕਸ(k,n);

    ਪੂਰਨ lda = (!TA)?k:m;
    ਪੂਰਨ ldb = (!TB)?n:k;

    ਦਸ਼ਮਲਵ *c = random_ਮੈਟ੍ਰਿਕਸ(m,n);

    ਦਸ਼ਮਲਵ *a_cl = cuda_make_array(a, m*k);
    ਦਸ਼ਮਲਵ *b_cl = cuda_make_array(b, k*n);
    ਦਸ਼ਮਲਵ *c_cl = cuda_make_array(c, m*n);

    ਪੂਰਨ i;
    clock_t ਸ਼ੁਰੂ = clock(), ਸਿਰਾ;
    ਲਈ(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    ਦੁੱਗਣਾ flop = ((ਦੁੱਗਣਾ)m)*n*(2.*k + 2.)*iter;
    ਦੁੱਗਣਾ gflop = flop/ਘਾਤ(10., 9);
    ਸਿਰਾ = clock();
    ਦੁੱਗਣਾ seconds = sec(ਸਿਰਾ-ਸ਼ੁਰੂ);
    ਲਿਖੋ("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    ਮੁਕਤ(a);
    ਮੁਕਤ(b);
    ਮੁਕਤ(c);
}


ਖਾਲੀ test_gpu_accuracy(ਪੂਰਨ TA, ਪੂਰਨ TB, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ n)
{
    srand(0);
    ਦਸ਼ਮਲਵ *a;
    ਜੇ(!TA) a = random_ਮੈਟ੍ਰਿਕਸ(m,k);
    ਨਹੀਂਤਾਂ a = random_ਮੈਟ੍ਰਿਕਸ(k,m);
    ਪੂਰਨ lda = (!TA)?k:m;
    ਦਸ਼ਮਲਵ *b;
    ਜੇ(!TB) b = random_ਮੈਟ੍ਰਿਕਸ(k,n);
    ਨਹੀਂਤਾਂ b = random_ਮੈਟ੍ਰਿਕਸ(n,k);
    ਪੂਰਨ ldb = (!TB)?n:k;

    ਦਸ਼ਮਲਵ *c = random_ਮੈਟ੍ਰਿਕਸ(m,n);
    ਦਸ਼ਮਲਵ *c_gpu = random_ਮੈਟ੍ਰਿਕਸ(m,n);
    memset(c, 0, m*n*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    memset(c_gpu, 0, m*n*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਪੂਰਨ i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    ਦੁੱਗਣਾ sse = 0;
    ਲਈ(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += ਘਾਤ(c[i]-c_gpu[i], 2);
    }
    ਲਿਖੋ("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    ਮੁਕਤ(a);
    ਮੁਕਤ(b);
    ਮੁਕਤ(c);
    ਮੁਕਤ(c_gpu);
}

ਪੂਰਨ test_gpu_blas()
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
    ਸਮਾਂ_gpu(0,0,64,75,12544); 
    ਸਮਾਂ_gpu(0,0,64,75,12544); 
    ਸਮਾਂ_gpu(0,0,64,75,12544); 
    ਸਮਾਂ_gpu(0,0,64,576,12544); 
    ਸਮਾਂ_gpu(0,0,256,2304,784); 
    ਸਮਾਂ_gpu(1,1,2304,256,784); 
    ਸਮਾਂ_gpu(0,0,512,4608,196); 
    ਸਮਾਂ_gpu(1,1,4608,512,196); 

    ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ

