#ضمن "gemm.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <math.h>

فراغ gemm_bin(صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        حرف  *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم *C, صحيح ldc)
{
    صحيح i,j,k;
    لكل(i = 0; i < M; ++i){
        لكل(k = 0; k < K; ++k){
            حرف A_PART = A[i*lda+k];
            إذا(A_PART){
                لكل(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } وإلا {
                لكل(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

عائم *random_ميٽرڪس(صحيح صفون, صحيح کانن_ڳڻپ)
{
    صحيح i;
    عائم *m = صفر_يادگيري_ڏيو(صفون*کانن_ڳڻپ, حجم(عائم));
    لكل(i = 0; i < صفون*کانن_ڳڻپ; ++i){
        m[i] = (عائم)rand()/RAND_MAX;
    }
    أرجع m;
}

فراغ وقت_random_ميٽرڪس(صحيح TA, صحيح TB, صحيح m, صحيح k, صحيح n)
{
    عائم *a;
    إذا(!TA) a = random_ميٽرڪس(m,k);
    وإلا a = random_ميٽرڪس(k,m);
    صحيح lda = (!TA)?k:m;
    عائم *b;
    إذا(!TB) b = random_ميٽرڪس(k,n);
    وإلا b = random_ميٽرڪس(n,k);
    صحيح ldb = (!TB)?n:k;

    عائم *c = random_ميٽرڪس(m,n);
    صحيح i;
    clock_t شروعات = clock(), پڇاڙي;
    لكل(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    پڇاڙي = clock();
    لکو("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (عائم)(پڇاڙي-شروعات)/CLOCKS_PER_SEC);
    حرر(a);
    حرر(b);
    حرر(c);
}


فراغ gemm(صحيح TA, صحيح TB, صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم BETA,
        عائم *C, صحيح ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

فراغ gemm_nn(صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم *C, صحيح ldc)
{
    صحيح i,j,k;
    #هدايت omp parallel لكل
    لكل(i = 0; i < M; ++i){
        لكل(k = 0; k < K; ++k){
            سجل عائم A_PART = ALPHA*A[i*lda+k];
            لكل(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

فراغ gemm_nt(صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم *C, صحيح ldc)
{
    صحيح i,j,k;
    #هدايت omp parallel لكل
    لكل(i = 0; i < M; ++i){
        لكل(j = 0; j < N; ++j){
            سجل عائم جوڙ = 0;
            لكل(k = 0; k < K; ++k){
                جوڙ += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += جوڙ;
        }
    }
}

فراغ gemm_tn(صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم *C, صحيح ldc)
{
    صحيح i,j,k;
    #هدايت omp parallel لكل
    لكل(i = 0; i < M; ++i){
        لكل(k = 0; k < K; ++k){
            سجل عائم A_PART = ALPHA*A[k*lda+i];
            لكل(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

فراغ gemm_tt(صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم *C, صحيح ldc)
{
    صحيح i,j,k;
    #هدايت omp parallel لكل
    لكل(i = 0; i < M; ++i){
        لكل(j = 0; j < N; ++j){
            سجل عائم جوڙ = 0;
            لكل(k = 0; k < K; ++k){
                جوڙ += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += جوڙ;
        }
    }
}


فراغ gemm_cpu(صحيح TA, صحيح TB, صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم BETA,
        عائم *C, صحيح ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    صحيح i, j;
    لكل(i = 0; i < M; ++i){
        لكل(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    إذا(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    وإلا إذا(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    وإلا إذا(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    وإلا
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#جيڪڏهن_وصف_ٿيل GPU

#ضمن <math.h>

فراغ gemm_gpu(صحيح TA, صحيح TB, صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A_gpu, صحيح lda, 
        عائم *B_gpu, صحيح ldb,
        عائم BETA,
        عائم *C_gpu, صحيح ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t حالت = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(حالت);
}

#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <time.h>

فراغ وقت_gpu_random_ميٽرڪس(صحيح TA, صحيح TB, صحيح m, صحيح k, صحيح n)
{
    عائم *a;
    إذا(!TA) a = random_ميٽرڪس(m,k);
    وإلا a = random_ميٽرڪس(k,m);
    صحيح lda = (!TA)?k:m;
    عائم *b;
    إذا(!TB) b = random_ميٽرڪس(k,n);
    وإلا b = random_ميٽرڪس(n,k);
    صحيح ldb = (!TB)?n:k;

    عائم *c = random_ميٽرڪس(m,n);
    صحيح i;
    clock_t شروعات = clock(), پڇاڙي;
    لكل(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    پڇاڙي = clock();
    لکو("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (عائم)(پڇاڙي-شروعات)/CLOCKS_PER_SEC);
    حرر(a);
    حرر(b);
    حرر(c);
}

فراغ وقت_gpu(صحيح TA, صحيح TB, صحيح m, صحيح k, صحيح n)
{
    صحيح iter = 10;
    عائم *a = random_ميٽرڪس(m,k);
    عائم *b = random_ميٽرڪس(k,n);

    صحيح lda = (!TA)?k:m;
    صحيح ldb = (!TB)?n:k;

    عائم *c = random_ميٽرڪس(m,n);

    عائم *a_cl = cuda_make_array(a, m*k);
    عائم *b_cl = cuda_make_array(b, k*n);
    عائم *c_cl = cuda_make_array(c, m*n);

    صحيح i;
    clock_t شروعات = clock(), پڇاڙي;
    لكل(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    مزدوج flop = ((مزدوج)m)*n*(2.*k + 2.)*iter;
    مزدوج gflop = flop/قوت(10., 9);
    پڇاڙي = clock();
    مزدوج seconds = sec(پڇاڙي-شروعات);
    لکو("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    حرر(a);
    حرر(b);
    حرر(c);
}


فراغ test_gpu_accuracy(صحيح TA, صحيح TB, صحيح m, صحيح k, صحيح n)
{
    srand(0);
    عائم *a;
    إذا(!TA) a = random_ميٽرڪس(m,k);
    وإلا a = random_ميٽرڪس(k,m);
    صحيح lda = (!TA)?k:m;
    عائم *b;
    إذا(!TB) b = random_ميٽرڪس(k,n);
    وإلا b = random_ميٽرڪس(n,k);
    صحيح ldb = (!TB)?n:k;

    عائم *c = random_ميٽرڪس(m,n);
    عائم *c_gpu = random_ميٽرڪس(m,n);
    memset(c, 0, m*n*حجم(عائم));
    memset(c_gpu, 0, m*n*حجم(عائم));
    صحيح i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    مزدوج sse = 0;
    لكل(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += قوت(c[i]-c_gpu[i], 2);
    }
    لکو("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    حرر(a);
    حرر(b);
    حرر(c);
    حرر(c_gpu);
}

صحيح test_gpu_blas()
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
    وقت_gpu(0,0,64,75,12544); 
    وقت_gpu(0,0,64,75,12544); 
    وقت_gpu(0,0,64,75,12544); 
    وقت_gpu(0,0,64,576,12544); 
    وقت_gpu(0,0,256,2304,784); 
    وقت_gpu(1,1,2304,256,784); 
    وقت_gpu(0,0,512,4608,196); 
    وقت_gpu(1,1,4608,512,196); 

    أرجع 0;
}
#جيڪڏهن_پورو

