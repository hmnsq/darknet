#ଅନ୍ତର୍ଭାଵ "gemm.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <math.h>

ଶୂନ୍ୟ gemm_bin(ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ଦଶମିକ ALPHA, 
        ଅକ୍ଷର  *A, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda, 
        ଦଶମିକ *B, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb,
        ଦଶମିକ *C, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldc)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    ପାଇଁ(i = 0; i < M; ++i){
        ପାଇଁ(k = 0; k < K; ++k){
            ଅକ୍ଷର A_PART = A[i*lda+k];
            ଯଦି(A_PART){
                ପାଇଁ(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } ଅନ୍ୟଥା {
                ପାଇଁ(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

ଦଶମିକ *random_matrix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ *m = calloc(rows*cols, ଆକାର(ଦଶମିକ));
    ପାଇଁ(i = 0; i < rows*cols; ++i){
        m[i] = (ଦଶମିକ)rand()/RAND_MAX;
    }
    ଫେରନ୍ତୁ m;
}

ଶୂନ୍ୟ time_random_matrix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା TA, ପୂର୍ଣ୍ଣସଂଖ୍ୟା TB, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଦଶମିକ *a;
    ଯଦି(!TA) a = random_matrix(m,k);
    ଅନ୍ୟଥା a = random_matrix(k,m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (!TA)?k:m;
    ଦଶମିକ *b;
    ଯଦି(!TB) b = random_matrix(k,n);
    ଅନ୍ୟଥା b = random_matrix(n,k);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb = (!TB)?n:k;

    ଦଶମିକ *c = random_matrix(m,n);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    clock_t start = clock(), end;
    ପାଇଁ(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (ଦଶମିକ)(end-start)/CLOCKS_PER_SEC);
    ମୁକ୍ତ(a);
    ମୁକ୍ତ(b);
    ମୁକ୍ତ(c);
}


ଶୂନ୍ୟ gemm(ପୂର୍ଣ୍ଣସଂଖ୍ୟା TA, ପୂର୍ଣ୍ଣସଂଖ୍ୟା TB, ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ଦଶମିକ ALPHA, 
        ଦଶମିକ *A, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda, 
        ଦଶମିକ *B, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb,
        ଦଶମିକ BETA,
        ଦଶମିକ *C, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

ଶୂନ୍ୟ gemm_nn(ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ଦଶମିକ ALPHA, 
        ଦଶମିକ *A, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda, 
        ଦଶମିକ *B, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb,
        ଦଶମିକ *C, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldc)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    #pragma omp parallel ପାଇଁ
    ପାଇଁ(i = 0; i < M; ++i){
        ପାଇଁ(k = 0; k < K; ++k){
            ପଞ୍ଜିକରଣ ଦଶମିକ A_PART = ALPHA*A[i*lda+k];
            ପାଇଁ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ଶୂନ୍ୟ gemm_nt(ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ଦଶମିକ ALPHA, 
        ଦଶମିକ *A, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda, 
        ଦଶମିକ *B, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb,
        ଦଶମିକ *C, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldc)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    #pragma omp parallel ପାଇଁ
    ପାଇଁ(i = 0; i < M; ++i){
        ପାଇଁ(j = 0; j < N; ++j){
            ପଞ୍ଜିକରଣ ଦଶମିକ sum = 0;
            ପାଇଁ(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

ଶୂନ୍ୟ gemm_tn(ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ଦଶମିକ ALPHA, 
        ଦଶମିକ *A, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda, 
        ଦଶମିକ *B, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb,
        ଦଶମିକ *C, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldc)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    #pragma omp parallel ପାଇଁ
    ପାଇଁ(i = 0; i < M; ++i){
        ପାଇଁ(k = 0; k < K; ++k){
            ପଞ୍ଜିକରଣ ଦଶମିକ A_PART = ALPHA*A[k*lda+i];
            ପାଇଁ(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ଶୂନ୍ୟ gemm_tt(ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ଦଶମିକ ALPHA, 
        ଦଶମିକ *A, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda, 
        ଦଶମିକ *B, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb,
        ଦଶମିକ *C, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldc)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    #pragma omp parallel ପାଇଁ
    ପାଇଁ(i = 0; i < M; ++i){
        ପାଇଁ(j = 0; j < N; ++j){
            ପଞ୍ଜିକରଣ ଦଶମିକ sum = 0;
            ପାଇଁ(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


ଶୂନ୍ୟ gemm_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା TA, ପୂର୍ଣ୍ଣସଂଖ୍ୟା TB, ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ଦଶମିକ ALPHA, 
        ଦଶମିକ *A, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda, 
        ଦଶମିକ *B, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb,
        ଦଶମିକ BETA,
        ଦଶମିକ *C, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପାଇଁ(i = 0; i < M; ++i){
        ପାଇଁ(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    ଯଦି(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ଅନ୍ୟଥା ଯଦି(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ଅନ୍ୟଥା ଯଦି(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    ଅନ୍ୟଥା
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#ଅନ୍ତର୍ଭାଵ <math.h>

ଶୂନ୍ୟ gemm_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା TA, ପୂର୍ଣ୍ଣସଂଖ୍ୟା TB, ପୂର୍ଣ୍ଣସଂଖ୍ୟା M, ପୂର୍ଣ୍ଣସଂଖ୍ୟା N, ପୂର୍ଣ୍ଣସଂଖ୍ୟା K, ଦଶମିକ ALPHA, 
        ଦଶମିକ *A_gpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda, 
        ଦଶମିକ *B_gpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb,
        ଦଶମିକ BETA,
        ଦଶମିକ *C_gpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

ଶୂନ୍ୟ time_gpu_random_matrix(ପୂର୍ଣ୍ଣସଂଖ୍ୟା TA, ପୂର୍ଣ୍ଣସଂଖ୍ୟା TB, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ଦଶମିକ *a;
    ଯଦି(!TA) a = random_matrix(m,k);
    ଅନ୍ୟଥା a = random_matrix(k,m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (!TA)?k:m;
    ଦଶମିକ *b;
    ଯଦି(!TB) b = random_matrix(k,n);
    ଅନ୍ୟଥା b = random_matrix(n,k);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb = (!TB)?n:k;

    ଦଶମିକ *c = random_matrix(m,n);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    clock_t start = clock(), end;
    ପାଇଁ(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (ଦଶମିକ)(end-start)/CLOCKS_PER_SEC);
    ମୁକ୍ତ(a);
    ମୁକ୍ତ(b);
    ମୁକ୍ତ(c);
}

ଶୂନ୍ୟ time_gpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା TA, ପୂର୍ଣ୍ଣସଂଖ୍ୟା TB, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iter = 10;
    ଦଶମିକ *a = random_matrix(m,k);
    ଦଶମିକ *b = random_matrix(k,n);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (!TA)?k:m;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb = (!TB)?n:k;

    ଦଶମିକ *c = random_matrix(m,n);

    ଦଶମିକ *a_cl = cuda_make_array(a, m*k);
    ଦଶମିକ *b_cl = cuda_make_array(b, k*n);
    ଦଶମିକ *c_cl = cuda_make_array(c, m*n);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    clock_t start = clock(), end;
    ପାଇଁ(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    ଦ୍ୱିଗୁଣ flop = ((ଦ୍ୱିଗୁଣ)m)*n*(2.*k + 2.)*iter;
    ଦ୍ୱିଗୁଣ gflop = flop/pow(10., 9);
    end = clock();
    ଦ୍ୱିଗୁଣ seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    ମୁକ୍ତ(a);
    ମୁକ୍ତ(b);
    ମୁକ୍ତ(c);
}


ଶୂନ୍ୟ test_gpu_accuracy(ପୂର୍ଣ୍ଣସଂଖ୍ୟା TA, ପୂର୍ଣ୍ଣସଂଖ୍ୟା TB, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    srand(0);
    ଦଶମିକ *a;
    ଯଦି(!TA) a = random_matrix(m,k);
    ଅନ୍ୟଥା a = random_matrix(k,m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା lda = (!TA)?k:m;
    ଦଶମିକ *b;
    ଯଦି(!TB) b = random_matrix(k,n);
    ଅନ୍ୟଥା b = random_matrix(n,k);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ldb = (!TB)?n:k;

    ଦଶମିକ *c = random_matrix(m,n);
    ଦଶମିକ *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*ଆକାର(ଦଶମିକ));
    memset(c_gpu, 0, m*n*ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    ଦ୍ୱିଗୁଣ sse = 0;
    ପାଇଁ(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    ମୁକ୍ତ(a);
    ମୁକ୍ତ(b);
    ମୁକ୍ତ(c);
    ମୁକ୍ତ(c_gpu);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_gpu_blas()
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

    ଫେରନ୍ତୁ 0;
}
#endif

