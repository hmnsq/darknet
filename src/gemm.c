#شامل "gemm.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل <stdlib.h>
#شامل <stdio.h>
#شامل <math.h>

تهی gemm_bin(صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        نویسه  *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری *C, صحیح ldc)
{
    صحیح i,j,k;
    برای(i = 0; i < M; ++i){
        برای(k = 0; k < K; ++k){
            نویسه A_PART = A[i*lda+k];
            اگر(A_PART){
                برای(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } وگرنه {
                برای(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

اعشاری *random_matrix(صحیح rows, صحیح cols)
{
    صحیح i;
    اعشاری *m = calloc(rows*cols, اندازه(اعشاری));
    برای(i = 0; i < rows*cols; ++i){
        m[i] = (اعشاری)rand()/RAND_MAX;
    }
    بازگشت m;
}

تهی time_random_matrix(صحیح TA, صحیح TB, صحیح m, صحیح k, صحیح n)
{
    اعشاری *a;
    اگر(!TA) a = random_matrix(m,k);
    وگرنه a = random_matrix(k,m);
    صحیح lda = (!TA)?k:m;
    اعشاری *b;
    اگر(!TB) b = random_matrix(k,n);
    وگرنه b = random_matrix(n,k);
    صحیح ldb = (!TB)?n:k;

    اعشاری *c = random_matrix(m,n);
    صحیح i;
    clock_t start = clock(), end;
    برای(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (اعشاری)(end-start)/CLOCKS_PER_SEC);
    آزاد(a);
    آزاد(b);
    آزاد(c);
}


تهی gemm(صحیح TA, صحیح TB, صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری BETA,
        اعشاری *C, صحیح ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

تهی gemm_nn(صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری *C, صحیح ldc)
{
    صحیح i,j,k;
    #pragma omp parallel برای
    برای(i = 0; i < M; ++i){
        برای(k = 0; k < K; ++k){
            ثبات اعشاری A_PART = ALPHA*A[i*lda+k];
            برای(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

تهی gemm_nt(صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری *C, صحیح ldc)
{
    صحیح i,j,k;
    #pragma omp parallel برای
    برای(i = 0; i < M; ++i){
        برای(j = 0; j < N; ++j){
            ثبات اعشاری sum = 0;
            برای(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

تهی gemm_tn(صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری *C, صحیح ldc)
{
    صحیح i,j,k;
    #pragma omp parallel برای
    برای(i = 0; i < M; ++i){
        برای(k = 0; k < K; ++k){
            ثبات اعشاری A_PART = ALPHA*A[k*lda+i];
            برای(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

تهی gemm_tt(صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری *C, صحیح ldc)
{
    صحیح i,j,k;
    #pragma omp parallel برای
    برای(i = 0; i < M; ++i){
        برای(j = 0; j < N; ++j){
            ثبات اعشاری sum = 0;
            برای(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


تهی gemm_cpu(صحیح TA, صحیح TB, صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری BETA,
        اعشاری *C, صحیح ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    صحیح i, j;
    برای(i = 0; i < M; ++i){
        برای(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    اگر(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    وگرنه اگر(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    وگرنه اگر(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    وگرنه
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#شامل <math.h>

تهی gemm_gpu(صحیح TA, صحیح TB, صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A_gpu, صحیح lda, 
        اعشاری *B_gpu, صحیح ldb,
        اعشاری BETA,
        اعشاری *C_gpu, صحیح ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <time.h>

تهی time_gpu_random_matrix(صحیح TA, صحیح TB, صحیح m, صحیح k, صحیح n)
{
    اعشاری *a;
    اگر(!TA) a = random_matrix(m,k);
    وگرنه a = random_matrix(k,m);
    صحیح lda = (!TA)?k:m;
    اعشاری *b;
    اگر(!TB) b = random_matrix(k,n);
    وگرنه b = random_matrix(n,k);
    صحیح ldb = (!TB)?n:k;

    اعشاری *c = random_matrix(m,n);
    صحیح i;
    clock_t start = clock(), end;
    برای(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (اعشاری)(end-start)/CLOCKS_PER_SEC);
    آزاد(a);
    آزاد(b);
    آزاد(c);
}

تهی time_gpu(صحیح TA, صحیح TB, صحیح m, صحیح k, صحیح n)
{
    صحیح iter = 10;
    اعشاری *a = random_matrix(m,k);
    اعشاری *b = random_matrix(k,n);

    صحیح lda = (!TA)?k:m;
    صحیح ldb = (!TB)?n:k;

    اعشاری *c = random_matrix(m,n);

    اعشاری *a_cl = cuda_make_array(a, m*k);
    اعشاری *b_cl = cuda_make_array(b, k*n);
    اعشاری *c_cl = cuda_make_array(c, m*n);

    صحیح i;
    clock_t start = clock(), end;
    برای(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    دوگانه flop = ((دوگانه)m)*n*(2.*k + 2.)*iter;
    دوگانه gflop = flop/pow(10., 9);
    end = clock();
    دوگانه seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    آزاد(a);
    آزاد(b);
    آزاد(c);
}


تهی test_gpu_accuracy(صحیح TA, صحیح TB, صحیح m, صحیح k, صحیح n)
{
    srand(0);
    اعشاری *a;
    اگر(!TA) a = random_matrix(m,k);
    وگرنه a = random_matrix(k,m);
    صحیح lda = (!TA)?k:m;
    اعشاری *b;
    اگر(!TB) b = random_matrix(k,n);
    وگرنه b = random_matrix(n,k);
    صحیح ldb = (!TB)?n:k;

    اعشاری *c = random_matrix(m,n);
    اعشاری *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*اندازه(اعشاری));
    memset(c_gpu, 0, m*n*اندازه(اعشاری));
    صحیح i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    دوگانه sse = 0;
    برای(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    آزاد(a);
    آزاد(b);
    آزاد(c);
    آزاد(c_gpu);
}

صحیح test_gpu_blas()
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

    بازگشت 0;
}
#endif

