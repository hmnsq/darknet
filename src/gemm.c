#অন্তর্ভাব "gemm.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <math.h>

শূন্য gemm_bin(পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, পূর্ণসংখ্যা K, দশমিক ALPHA, 
        অক্ষর  *A, পূর্ণসংখ্যা lda, 
        দশমিক *B, পূর্ণসংখ্যা ldb,
        দশমিক *C, পূর্ণসংখ্যা ldc)
{
    পূর্ণসংখ্যা i,j,k;
    জন্য(i = 0; i < M; ++i){
        জন্য(k = 0; k < K; ++k){
            অক্ষর A_PART = A[i*lda+k];
            যদি(A_PART){
                জন্য(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } অন্যথা {
                জন্য(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

দশমিক *random_matrix(পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols)
{
    পূর্ণসংখ্যা i;
    দশমিক *m = calloc(rows*cols, আকার(দশমিক));
    জন্য(i = 0; i < rows*cols; ++i){
        m[i] = (দশমিক)rand()/RAND_MAX;
    }
    ফেরত m;
}

শূন্য time_random_matrix(পূর্ণসংখ্যা TA, পূর্ণসংখ্যা TB, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা n)
{
    দশমিক *a;
    যদি(!TA) a = random_matrix(m,k);
    অন্যথা a = random_matrix(k,m);
    পূর্ণসংখ্যা lda = (!TA)?k:m;
    দশমিক *b;
    যদি(!TB) b = random_matrix(k,n);
    অন্যথা b = random_matrix(n,k);
    পূর্ণসংখ্যা ldb = (!TB)?n:k;

    দশমিক *c = random_matrix(m,n);
    পূর্ণসংখ্যা i;
    clock_t start = clock(), end;
    জন্য(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (দশমিক)(end-start)/CLOCKS_PER_SEC);
    মুক্ত(a);
    মুক্ত(b);
    মুক্ত(c);
}


শূন্য gemm(পূর্ণসংখ্যা TA, পূর্ণসংখ্যা TB, পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, পূর্ণসংখ্যা K, দশমিক ALPHA, 
        দশমিক *A, পূর্ণসংখ্যা lda, 
        দশমিক *B, পূর্ণসংখ্যা ldb,
        দশমিক BETA,
        দশমিক *C, পূর্ণসংখ্যা ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

শূন্য gemm_nn(পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, পূর্ণসংখ্যা K, দশমিক ALPHA, 
        দশমিক *A, পূর্ণসংখ্যা lda, 
        দশমিক *B, পূর্ণসংখ্যা ldb,
        দশমিক *C, পূর্ণসংখ্যা ldc)
{
    পূর্ণসংখ্যা i,j,k;
    #pragma omp parallel জন্য
    জন্য(i = 0; i < M; ++i){
        জন্য(k = 0; k < K; ++k){
            নিবন্ধ দশমিক A_PART = ALPHA*A[i*lda+k];
            জন্য(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

শূন্য gemm_nt(পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, পূর্ণসংখ্যা K, দশমিক ALPHA, 
        দশমিক *A, পূর্ণসংখ্যা lda, 
        দশমিক *B, পূর্ণসংখ্যা ldb,
        দশমিক *C, পূর্ণসংখ্যা ldc)
{
    পূর্ণসংখ্যা i,j,k;
    #pragma omp parallel জন্য
    জন্য(i = 0; i < M; ++i){
        জন্য(j = 0; j < N; ++j){
            নিবন্ধ দশমিক যোগ = 0;
            জন্য(k = 0; k < K; ++k){
                যোগ += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += যোগ;
        }
    }
}

শূন্য gemm_tn(পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, পূর্ণসংখ্যা K, দশমিক ALPHA, 
        দশমিক *A, পূর্ণসংখ্যা lda, 
        দশমিক *B, পূর্ণসংখ্যা ldb,
        দশমিক *C, পূর্ণসংখ্যা ldc)
{
    পূর্ণসংখ্যা i,j,k;
    #pragma omp parallel জন্য
    জন্য(i = 0; i < M; ++i){
        জন্য(k = 0; k < K; ++k){
            নিবন্ধ দশমিক A_PART = ALPHA*A[k*lda+i];
            জন্য(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

শূন্য gemm_tt(পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, পূর্ণসংখ্যা K, দশমিক ALPHA, 
        দশমিক *A, পূর্ণসংখ্যা lda, 
        দশমিক *B, পূর্ণসংখ্যা ldb,
        দশমিক *C, পূর্ণসংখ্যা ldc)
{
    পূর্ণসংখ্যা i,j,k;
    #pragma omp parallel জন্য
    জন্য(i = 0; i < M; ++i){
        জন্য(j = 0; j < N; ++j){
            নিবন্ধ দশমিক যোগ = 0;
            জন্য(k = 0; k < K; ++k){
                যোগ += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += যোগ;
        }
    }
}


শূন্য gemm_cpu(পূর্ণসংখ্যা TA, পূর্ণসংখ্যা TB, পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, পূর্ণসংখ্যা K, দশমিক ALPHA, 
        দশমিক *A, পূর্ণসংখ্যা lda, 
        দশমিক *B, পূর্ণসংখ্যা ldb,
        দশমিক BETA,
        দশমিক *C, পূর্ণসংখ্যা ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    পূর্ণসংখ্যা i, j;
    জন্য(i = 0; i < M; ++i){
        জন্য(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    যদি(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    অন্যথা যদি(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    অন্যথা যদি(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    অন্যথা
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#অন্তর্ভাব <math.h>

শূন্য gemm_gpu(পূর্ণসংখ্যা TA, পূর্ণসংখ্যা TB, পূর্ণসংখ্যা M, পূর্ণসংখ্যা N, পূর্ণসংখ্যা K, দশমিক ALPHA, 
        দশমিক *A_gpu, পূর্ণসংখ্যা lda, 
        দশমিক *B_gpu, পূর্ণসংখ্যা ldb,
        দশমিক BETA,
        দশমিক *C_gpu, পূর্ণসংখ্যা ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <time.h>

শূন্য time_gpu_random_matrix(পূর্ণসংখ্যা TA, পূর্ণসংখ্যা TB, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা n)
{
    দশমিক *a;
    যদি(!TA) a = random_matrix(m,k);
    অন্যথা a = random_matrix(k,m);
    পূর্ণসংখ্যা lda = (!TA)?k:m;
    দশমিক *b;
    যদি(!TB) b = random_matrix(k,n);
    অন্যথা b = random_matrix(n,k);
    পূর্ণসংখ্যা ldb = (!TB)?n:k;

    দশমিক *c = random_matrix(m,n);
    পূর্ণসংখ্যা i;
    clock_t start = clock(), end;
    জন্য(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (দশমিক)(end-start)/CLOCKS_PER_SEC);
    মুক্ত(a);
    মুক্ত(b);
    মুক্ত(c);
}

শূন্য time_gpu(পূর্ণসংখ্যা TA, পূর্ণসংখ্যা TB, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা iter = 10;
    দশমিক *a = random_matrix(m,k);
    দশমিক *b = random_matrix(k,n);

    পূর্ণসংখ্যা lda = (!TA)?k:m;
    পূর্ণসংখ্যা ldb = (!TB)?n:k;

    দশমিক *c = random_matrix(m,n);

    দশমিক *a_cl = cuda_make_array(a, m*k);
    দশমিক *b_cl = cuda_make_array(b, k*n);
    দশমিক *c_cl = cuda_make_array(c, m*n);

    পূর্ণসংখ্যা i;
    clock_t start = clock(), end;
    জন্য(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    দ্বিগুণ flop = ((দ্বিগুণ)m)*n*(2.*k + 2.)*iter;
    দ্বিগুণ gflop = flop/pow(10., 9);
    end = clock();
    দ্বিগুণ seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    মুক্ত(a);
    মুক্ত(b);
    মুক্ত(c);
}


শূন্য test_gpu_accuracy(পূর্ণসংখ্যা TA, পূর্ণসংখ্যা TB, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা n)
{
    srand(0);
    দশমিক *a;
    যদি(!TA) a = random_matrix(m,k);
    অন্যথা a = random_matrix(k,m);
    পূর্ণসংখ্যা lda = (!TA)?k:m;
    দশমিক *b;
    যদি(!TB) b = random_matrix(k,n);
    অন্যথা b = random_matrix(n,k);
    পূর্ণসংখ্যা ldb = (!TB)?n:k;

    দশমিক *c = random_matrix(m,n);
    দশমিক *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*আকার(দশমিক));
    memset(c_gpu, 0, m*n*আকার(দশমিক));
    পূর্ণসংখ্যা i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    দ্বিগুণ sse = 0;
    জন্য(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    মুক্ত(a);
    মুক্ত(b);
    মুক্ত(c);
    মুক্ত(c_gpu);
}

পূর্ণসংখ্যা test_gpu_blas()
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

    ফেরত 0;
}
#endif

