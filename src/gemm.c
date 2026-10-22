#অন্তর্ভাব "gemm.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <math.h>

শূন্য gemm_bin(পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, পূর্ণাঙ্ক K, ভিন্ন ALPHA, 
        বর্ণ  *A, পূর্ণাঙ্ক lda, 
        ভিন্ন *B, পূর্ণাঙ্ক ldb,
        ভিন্ন *C, পূর্ণাঙ্ক ldc)
{
    পূর্ণাঙ্ক i,j,k;
    প্রতি(i = 0; i < M; ++i){
        প্রতি(k = 0; k < K; ++k){
            বর্ণ A_PART = A[i*lda+k];
            যদি(A_PART){
                প্রতি(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } অন্যথা {
                প্রতি(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

ভিন্ন *random_matrix(পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols)
{
    পূর্ণাঙ্ক i;
    ভিন্ন *m = calloc(rows*cols, পরিমাণ(ভিন্ন));
    প্রতি(i = 0; i < rows*cols; ++i){
        m[i] = (ভিন্ন)rand()/RAND_MAX;
    }
    প্রত্যাগম m;
}

শূন্য time_random_matrix(পূর্ণাঙ্ক TA, পূর্ণাঙ্ক TB, পূর্ণাঙ্ক m, পূর্ণাঙ্ক k, পূর্ণাঙ্ক n)
{
    ভিন্ন *a;
    যদি(!TA) a = random_matrix(m,k);
    অন্যথা a = random_matrix(k,m);
    পূর্ণাঙ্ক lda = (!TA)?k:m;
    ভিন্ন *b;
    যদি(!TB) b = random_matrix(k,n);
    অন্যথা b = random_matrix(n,k);
    পূর্ণাঙ্ক ldb = (!TB)?n:k;

    ভিন্ন *c = random_matrix(m,n);
    পূর্ণাঙ্ক i;
    clock_t start = clock(), end;
    প্রতি(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (ভিন্ন)(end-start)/CLOCKS_PER_SEC);
    মুক্ত(a);
    মুক্ত(b);
    মুক্ত(c);
}


শূন্য gemm(পূর্ণাঙ্ক TA, পূর্ণাঙ্ক TB, পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, পূর্ণাঙ্ক K, ভিন্ন ALPHA, 
        ভিন্ন *A, পূর্ণাঙ্ক lda, 
        ভিন্ন *B, পূর্ণাঙ্ক ldb,
        ভিন্ন BETA,
        ভিন্ন *C, পূর্ণাঙ্ক ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

শূন্য gemm_nn(পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, পূর্ণাঙ্ক K, ভিন্ন ALPHA, 
        ভিন্ন *A, পূর্ণাঙ্ক lda, 
        ভিন্ন *B, পূর্ণাঙ্ক ldb,
        ভিন্ন *C, পূর্ণাঙ্ক ldc)
{
    পূর্ণাঙ্ক i,j,k;
    #pragma omp parallel প্রতি
    প্রতি(i = 0; i < M; ++i){
        প্রতি(k = 0; k < K; ++k){
            পঞ্জিকা ভিন্ন A_PART = ALPHA*A[i*lda+k];
            প্রতি(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

শূন্য gemm_nt(পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, পূর্ণাঙ্ক K, ভিন্ন ALPHA, 
        ভিন্ন *A, পূর্ণাঙ্ক lda, 
        ভিন্ন *B, পূর্ণাঙ্ক ldb,
        ভিন্ন *C, পূর্ণাঙ্ক ldc)
{
    পূর্ণাঙ্ক i,j,k;
    #pragma omp parallel প্রতি
    প্রতি(i = 0; i < M; ++i){
        প্রতি(j = 0; j < N; ++j){
            পঞ্জিকা ভিন্ন sum = 0;
            প্রতি(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

শূন্য gemm_tn(পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, পূর্ণাঙ্ক K, ভিন্ন ALPHA, 
        ভিন্ন *A, পূর্ণাঙ্ক lda, 
        ভিন্ন *B, পূর্ণাঙ্ক ldb,
        ভিন্ন *C, পূর্ণাঙ্ক ldc)
{
    পূর্ণাঙ্ক i,j,k;
    #pragma omp parallel প্রতি
    প্রতি(i = 0; i < M; ++i){
        প্রতি(k = 0; k < K; ++k){
            পঞ্জিকা ভিন্ন A_PART = ALPHA*A[k*lda+i];
            প্রতি(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

শূন্য gemm_tt(পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, পূর্ণাঙ্ক K, ভিন্ন ALPHA, 
        ভিন্ন *A, পূর্ণাঙ্ক lda, 
        ভিন্ন *B, পূর্ণাঙ্ক ldb,
        ভিন্ন *C, পূর্ণাঙ্ক ldc)
{
    পূর্ণাঙ্ক i,j,k;
    #pragma omp parallel প্রতি
    প্রতি(i = 0; i < M; ++i){
        প্রতি(j = 0; j < N; ++j){
            পঞ্জিকা ভিন্ন sum = 0;
            প্রতি(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


শূন্য gemm_cpu(পূর্ণাঙ্ক TA, পূর্ণাঙ্ক TB, পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, পূর্ণাঙ্ক K, ভিন্ন ALPHA, 
        ভিন্ন *A, পূর্ণাঙ্ক lda, 
        ভিন্ন *B, পূর্ণাঙ্ক ldb,
        ভিন্ন BETA,
        ভিন্ন *C, পূর্ণাঙ্ক ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    পূর্ণাঙ্ক i, j;
    প্রতি(i = 0; i < M; ++i){
        প্রতি(j = 0; j < N; ++j){
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

শূন্য gemm_gpu(পূর্ণাঙ্ক TA, পূর্ণাঙ্ক TB, পূর্ণাঙ্ক M, পূর্ণাঙ্ক N, পূর্ণাঙ্ক K, ভিন্ন ALPHA, 
        ভিন্ন *A_gpu, পূর্ণাঙ্ক lda, 
        ভিন্ন *B_gpu, পূর্ণাঙ্ক ldb,
        ভিন্ন BETA,
        ভিন্ন *C_gpu, পূর্ণাঙ্ক ldc)
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

শূন্য time_gpu_random_matrix(পূর্ণাঙ্ক TA, পূর্ণাঙ্ক TB, পূর্ণাঙ্ক m, পূর্ণাঙ্ক k, পূর্ণাঙ্ক n)
{
    ভিন্ন *a;
    যদি(!TA) a = random_matrix(m,k);
    অন্যথা a = random_matrix(k,m);
    পূর্ণাঙ্ক lda = (!TA)?k:m;
    ভিন্ন *b;
    যদি(!TB) b = random_matrix(k,n);
    অন্যথা b = random_matrix(n,k);
    পূর্ণাঙ্ক ldb = (!TB)?n:k;

    ভিন্ন *c = random_matrix(m,n);
    পূর্ণাঙ্ক i;
    clock_t start = clock(), end;
    প্রতি(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (ভিন্ন)(end-start)/CLOCKS_PER_SEC);
    মুক্ত(a);
    মুক্ত(b);
    মুক্ত(c);
}

শূন্য time_gpu(পূর্ণাঙ্ক TA, পূর্ণাঙ্ক TB, পূর্ণাঙ্ক m, পূর্ণাঙ্ক k, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক iter = 10;
    ভিন্ন *a = random_matrix(m,k);
    ভিন্ন *b = random_matrix(k,n);

    পূর্ণাঙ্ক lda = (!TA)?k:m;
    পূর্ণাঙ্ক ldb = (!TB)?n:k;

    ভিন্ন *c = random_matrix(m,n);

    ভিন্ন *a_cl = cuda_make_array(a, m*k);
    ভিন্ন *b_cl = cuda_make_array(b, k*n);
    ভিন্ন *c_cl = cuda_make_array(c, m*n);

    পূর্ণাঙ্ক i;
    clock_t start = clock(), end;
    প্রতি(i = 0; i<iter; ++i){
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


শূন্য test_gpu_accuracy(পূর্ণাঙ্ক TA, পূর্ণাঙ্ক TB, পূর্ণাঙ্ক m, পূর্ণাঙ্ক k, পূর্ণাঙ্ক n)
{
    srand(0);
    ভিন্ন *a;
    যদি(!TA) a = random_matrix(m,k);
    অন্যথা a = random_matrix(k,m);
    পূর্ণাঙ্ক lda = (!TA)?k:m;
    ভিন্ন *b;
    যদি(!TB) b = random_matrix(k,n);
    অন্যথা b = random_matrix(n,k);
    পূর্ণাঙ্ক ldb = (!TB)?n:k;

    ভিন্ন *c = random_matrix(m,n);
    ভিন্ন *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*পরিমাণ(ভিন্ন));
    memset(c_gpu, 0, m*n*পরিমাণ(ভিন্ন));
    পূর্ণাঙ্ক i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    দ্বিগুণ sse = 0;
    প্রতি(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    মুক্ত(a);
    মুক্ত(b);
    মুক্ত(c);
    মুক্ত(c_gpu);
}

পূর্ণাঙ্ক test_gpu_blas()
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

    প্রত্যাগম 0;
}
#endif

