#அந்தர்பாவ "gemm.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <math.h>

வெறுமை gemm_bin(முழு M, முழு N, முழு K, தசம ALPHA, 
        எழுத்து  *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம *C, முழு ldc)
{
    முழு i,j,k;
    க்கு(i = 0; i < M; ++i){
        க்கு(k = 0; k < K; ++k){
            எழுத்து A_PART = A[i*lda+k];
            எனில்(A_PART){
                க்கு(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } இல்லையெனில் {
                க்கு(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

தசம *random_matrix(முழு rows, முழு cols)
{
    முழு i;
    தசம *m = calloc(rows*cols, அளவு(தசம));
    க்கு(i = 0; i < rows*cols; ++i){
        m[i] = (தசம)rand()/RAND_MAX;
    }
    திரும்பு m;
}

வெறுமை time_random_matrix(முழு TA, முழு TB, முழு m, முழு k, முழு n)
{
    தசம *a;
    எனில்(!TA) a = random_matrix(m,k);
    இல்லையெனில் a = random_matrix(k,m);
    முழு lda = (!TA)?k:m;
    தசம *b;
    எனில்(!TB) b = random_matrix(k,n);
    இல்லையெனில் b = random_matrix(n,k);
    முழு ldb = (!TB)?n:k;

    தசம *c = random_matrix(m,n);
    முழு i;
    clock_t start = clock(), end;
    க்கு(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (தசம)(end-start)/CLOCKS_PER_SEC);
    விடுவி(a);
    விடுவி(b);
    விடுவி(c);
}


வெறுமை gemm(முழு TA, முழு TB, முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம BETA,
        தசம *C, முழு ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

வெறுமை gemm_nn(முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம *C, முழு ldc)
{
    முழு i,j,k;
    #pragma omp parallel க்கு
    க்கு(i = 0; i < M; ++i){
        க்கு(k = 0; k < K; ++k){
            பதிவு தசம A_PART = ALPHA*A[i*lda+k];
            க்கு(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

வெறுமை gemm_nt(முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம *C, முழு ldc)
{
    முழு i,j,k;
    #pragma omp parallel க்கு
    க்கு(i = 0; i < M; ++i){
        க்கு(j = 0; j < N; ++j){
            பதிவு தசம sum = 0;
            க்கு(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

வெறுமை gemm_tn(முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம *C, முழு ldc)
{
    முழு i,j,k;
    #pragma omp parallel க்கு
    க்கு(i = 0; i < M; ++i){
        க்கு(k = 0; k < K; ++k){
            பதிவு தசம A_PART = ALPHA*A[k*lda+i];
            க்கு(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

வெறுமை gemm_tt(முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம *C, முழு ldc)
{
    முழு i,j,k;
    #pragma omp parallel க்கு
    க்கு(i = 0; i < M; ++i){
        க்கு(j = 0; j < N; ++j){
            பதிவு தசம sum = 0;
            க்கு(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


வெறுமை gemm_cpu(முழு TA, முழு TB, முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம BETA,
        தசம *C, முழு ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    முழு i, j;
    க்கு(i = 0; i < M; ++i){
        க்கு(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    எனில்(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    இல்லையெனில் எனில்(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    இல்லையெனில் எனில்(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    இல்லையெனில்
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#அந்தர்பாவ <math.h>

வெறுமை gemm_gpu(முழு TA, முழு TB, முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A_gpu, முழு lda, 
        தசம *B_gpu, முழு ldb,
        தசம BETA,
        தசம *C_gpu, முழு ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <time.h>

வெறுமை time_gpu_random_matrix(முழு TA, முழு TB, முழு m, முழு k, முழு n)
{
    தசம *a;
    எனில்(!TA) a = random_matrix(m,k);
    இல்லையெனில் a = random_matrix(k,m);
    முழு lda = (!TA)?k:m;
    தசம *b;
    எனில்(!TB) b = random_matrix(k,n);
    இல்லையெனில் b = random_matrix(n,k);
    முழு ldb = (!TB)?n:k;

    தசம *c = random_matrix(m,n);
    முழு i;
    clock_t start = clock(), end;
    க்கு(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (தசம)(end-start)/CLOCKS_PER_SEC);
    விடுவி(a);
    விடுவி(b);
    விடுவி(c);
}

வெறுமை time_gpu(முழு TA, முழு TB, முழு m, முழு k, முழு n)
{
    முழு iter = 10;
    தசம *a = random_matrix(m,k);
    தசம *b = random_matrix(k,n);

    முழு lda = (!TA)?k:m;
    முழு ldb = (!TB)?n:k;

    தசம *c = random_matrix(m,n);

    தசம *a_cl = cuda_make_array(a, m*k);
    தசம *b_cl = cuda_make_array(b, k*n);
    தசம *c_cl = cuda_make_array(c, m*n);

    முழு i;
    clock_t start = clock(), end;
    க்கு(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    இரட்டை flop = ((இரட்டை)m)*n*(2.*k + 2.)*iter;
    இரட்டை gflop = flop/pow(10., 9);
    end = clock();
    இரட்டை seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    விடுவி(a);
    விடுவி(b);
    விடுவி(c);
}


வெறுமை test_gpu_accuracy(முழு TA, முழு TB, முழு m, முழு k, முழு n)
{
    srand(0);
    தசம *a;
    எனில்(!TA) a = random_matrix(m,k);
    இல்லையெனில் a = random_matrix(k,m);
    முழு lda = (!TA)?k:m;
    தசம *b;
    எனில்(!TB) b = random_matrix(k,n);
    இல்லையெனில் b = random_matrix(n,k);
    முழு ldb = (!TB)?n:k;

    தசம *c = random_matrix(m,n);
    தசம *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*அளவு(தசம));
    memset(c_gpu, 0, m*n*அளவு(தசம));
    முழு i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    இரட்டை sse = 0;
    க்கு(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    விடுவி(a);
    விடுவி(b);
    விடுவி(c);
    விடுவி(c_gpu);
}

முழு test_gpu_blas()
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

    திரும்பு 0;
}
#endif

