#അന്തര്ഭാവ "gemm.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <math.h>

ശൂന്യം gemm_bin(പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, പൂർണ്ണസംഖ്യ K, ദശാംശം ALPHA, 
        അക്ഷരം  *A, പൂർണ്ണസംഖ്യ lda, 
        ദശാംശം *B, പൂർണ്ണസംഖ്യ ldb,
        ദശാംശം *C, പൂർണ്ണസംഖ്യ ldc)
{
    പൂർണ്ണസംഖ്യ i,j,k;
    വേണ്ടി(i = 0; i < M; ++i){
        വേണ്ടി(k = 0; k < K; ++k){
            അക്ഷരം A_PART = A[i*lda+k];
            എങ്കിൽ(A_PART){
                വേണ്ടി(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } അല്ലെങ്കിൽ {
                വേണ്ടി(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

ദശാംശം *random_matrix(പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം *m = calloc(rows*cols, വലുപ്പം(ദശാംശം));
    വേണ്ടി(i = 0; i < rows*cols; ++i){
        m[i] = (ദശാംശം)rand()/RAND_MAX;
    }
    തിരികെ m;
}

ശൂന്യം time_random_matrix(പൂർണ്ണസംഖ്യ TA, പൂർണ്ണസംഖ്യ TB, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ n)
{
    ദശാംശം *a;
    എങ്കിൽ(!TA) a = random_matrix(m,k);
    അല്ലെങ്കിൽ a = random_matrix(k,m);
    പൂർണ്ണസംഖ്യ lda = (!TA)?k:m;
    ദശാംശം *b;
    എങ്കിൽ(!TB) b = random_matrix(k,n);
    അല്ലെങ്കിൽ b = random_matrix(n,k);
    പൂർണ്ണസംഖ്യ ldb = (!TB)?n:k;

    ദശാംശം *c = random_matrix(m,n);
    പൂർണ്ണസംഖ്യ i;
    clock_t start = clock(), end;
    വേണ്ടി(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (ദശാംശം)(end-start)/CLOCKS_PER_SEC);
    സ്വതന്ത്രം(a);
    സ്വതന്ത്രം(b);
    സ്വതന്ത്രം(c);
}


ശൂന്യം gemm(പൂർണ്ണസംഖ്യ TA, പൂർണ്ണസംഖ്യ TB, പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, പൂർണ്ണസംഖ്യ K, ദശാംശം ALPHA, 
        ദശാംശം *A, പൂർണ്ണസംഖ്യ lda, 
        ദശാംശം *B, പൂർണ്ണസംഖ്യ ldb,
        ദശാംശം BETA,
        ദശാംശം *C, പൂർണ്ണസംഖ്യ ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

ശൂന്യം gemm_nn(പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, പൂർണ്ണസംഖ്യ K, ദശാംശം ALPHA, 
        ദശാംശം *A, പൂർണ്ണസംഖ്യ lda, 
        ദശാംശം *B, പൂർണ്ണസംഖ്യ ldb,
        ദശാംശം *C, പൂർണ്ണസംഖ്യ ldc)
{
    പൂർണ്ണസംഖ്യ i,j,k;
    #pragma omp parallel വേണ്ടി
    വേണ്ടി(i = 0; i < M; ++i){
        വേണ്ടി(k = 0; k < K; ++k){
            രജിസ്റ്റർ ദശാംശം A_PART = ALPHA*A[i*lda+k];
            വേണ്ടി(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ശൂന്യം gemm_nt(പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, പൂർണ്ണസംഖ്യ K, ദശാംശം ALPHA, 
        ദശാംശം *A, പൂർണ്ണസംഖ്യ lda, 
        ദശാംശം *B, പൂർണ്ണസംഖ്യ ldb,
        ദശാംശം *C, പൂർണ്ണസംഖ്യ ldc)
{
    പൂർണ്ണസംഖ്യ i,j,k;
    #pragma omp parallel വേണ്ടി
    വേണ്ടി(i = 0; i < M; ++i){
        വേണ്ടി(j = 0; j < N; ++j){
            രജിസ്റ്റർ ദശാംശം sum = 0;
            വേണ്ടി(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

ശൂന്യം gemm_tn(പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, പൂർണ്ണസംഖ്യ K, ദശാംശം ALPHA, 
        ദശാംശം *A, പൂർണ്ണസംഖ്യ lda, 
        ദശാംശം *B, പൂർണ്ണസംഖ്യ ldb,
        ദശാംശം *C, പൂർണ്ണസംഖ്യ ldc)
{
    പൂർണ്ണസംഖ്യ i,j,k;
    #pragma omp parallel വേണ്ടി
    വേണ്ടി(i = 0; i < M; ++i){
        വേണ്ടി(k = 0; k < K; ++k){
            രജിസ്റ്റർ ദശാംശം A_PART = ALPHA*A[k*lda+i];
            വേണ്ടി(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

ശൂന്യം gemm_tt(പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, പൂർണ്ണസംഖ്യ K, ദശാംശം ALPHA, 
        ദശാംശം *A, പൂർണ്ണസംഖ്യ lda, 
        ദശാംശം *B, പൂർണ്ണസംഖ്യ ldb,
        ദശാംശം *C, പൂർണ്ണസംഖ്യ ldc)
{
    പൂർണ്ണസംഖ്യ i,j,k;
    #pragma omp parallel വേണ്ടി
    വേണ്ടി(i = 0; i < M; ++i){
        വേണ്ടി(j = 0; j < N; ++j){
            രജിസ്റ്റർ ദശാംശം sum = 0;
            വേണ്ടി(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


ശൂന്യം gemm_cpu(പൂർണ്ണസംഖ്യ TA, പൂർണ്ണസംഖ്യ TB, പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, പൂർണ്ണസംഖ്യ K, ദശാംശം ALPHA, 
        ദശാംശം *A, പൂർണ്ണസംഖ്യ lda, 
        ദശാംശം *B, പൂർണ്ണസംഖ്യ ldb,
        ദശാംശം BETA,
        ദശാംശം *C, പൂർണ്ണസംഖ്യ ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    പൂർണ്ണസംഖ്യ i, j;
    വേണ്ടി(i = 0; i < M; ++i){
        വേണ്ടി(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    എങ്കിൽ(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    അല്ലെങ്കിൽ എങ്കിൽ(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    അല്ലെങ്കിൽ എങ്കിൽ(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    അല്ലെങ്കിൽ
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#അന്തര്ഭാവ <math.h>

ശൂന്യം gemm_gpu(പൂർണ്ണസംഖ്യ TA, പൂർണ്ണസംഖ്യ TB, പൂർണ്ണസംഖ്യ M, പൂർണ്ണസംഖ്യ N, പൂർണ്ണസംഖ്യ K, ദശാംശം ALPHA, 
        ദശാംശം *A_gpu, പൂർണ്ണസംഖ്യ lda, 
        ദശാംശം *B_gpu, പൂർണ്ണസംഖ്യ ldb,
        ദശാംശം BETA,
        ദശാംശം *C_gpu, പൂർണ്ണസംഖ്യ ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <time.h>

ശൂന്യം time_gpu_random_matrix(പൂർണ്ണസംഖ്യ TA, പൂർണ്ണസംഖ്യ TB, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ n)
{
    ദശാംശം *a;
    എങ്കിൽ(!TA) a = random_matrix(m,k);
    അല്ലെങ്കിൽ a = random_matrix(k,m);
    പൂർണ്ണസംഖ്യ lda = (!TA)?k:m;
    ദശാംശം *b;
    എങ്കിൽ(!TB) b = random_matrix(k,n);
    അല്ലെങ്കിൽ b = random_matrix(n,k);
    പൂർണ്ണസംഖ്യ ldb = (!TB)?n:k;

    ദശാംശം *c = random_matrix(m,n);
    പൂർണ്ണസംഖ്യ i;
    clock_t start = clock(), end;
    വേണ്ടി(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (ദശാംശം)(end-start)/CLOCKS_PER_SEC);
    സ്വതന്ത്രം(a);
    സ്വതന്ത്രം(b);
    സ്വതന്ത്രം(c);
}

ശൂന്യം time_gpu(പൂർണ്ണസംഖ്യ TA, പൂർണ്ണസംഖ്യ TB, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ iter = 10;
    ദശാംശം *a = random_matrix(m,k);
    ദശാംശം *b = random_matrix(k,n);

    പൂർണ്ണസംഖ്യ lda = (!TA)?k:m;
    പൂർണ്ണസംഖ്യ ldb = (!TB)?n:k;

    ദശാംശം *c = random_matrix(m,n);

    ദശാംശം *a_cl = cuda_make_array(a, m*k);
    ദശാംശം *b_cl = cuda_make_array(b, k*n);
    ദശാംശം *c_cl = cuda_make_array(c, m*n);

    പൂർണ്ണസംഖ്യ i;
    clock_t start = clock(), end;
    വേണ്ടി(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    ഇരട്ട flop = ((ഇരട്ട)m)*n*(2.*k + 2.)*iter;
    ഇരട്ട gflop = flop/pow(10., 9);
    end = clock();
    ഇരട്ട seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    സ്വതന്ത്രം(a);
    സ്വതന്ത്രം(b);
    സ്വതന്ത്രം(c);
}


ശൂന്യം test_gpu_accuracy(പൂർണ്ണസംഖ്യ TA, പൂർണ്ണസംഖ്യ TB, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ n)
{
    srand(0);
    ദശാംശം *a;
    എങ്കിൽ(!TA) a = random_matrix(m,k);
    അല്ലെങ്കിൽ a = random_matrix(k,m);
    പൂർണ്ണസംഖ്യ lda = (!TA)?k:m;
    ദശാംശം *b;
    എങ്കിൽ(!TB) b = random_matrix(k,n);
    അല്ലെങ്കിൽ b = random_matrix(n,k);
    പൂർണ്ണസംഖ്യ ldb = (!TB)?n:k;

    ദശാംശം *c = random_matrix(m,n);
    ദശാംശം *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*വലുപ്പം(ദശാംശം));
    memset(c_gpu, 0, m*n*വലുപ്പം(ദശാംശം));
    പൂർണ്ണസംഖ്യ i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    ഇരട്ട sse = 0;
    വേണ്ടി(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    സ്വതന്ത്രം(a);
    സ്വതന്ത്രം(b);
    സ്വതന്ത്രം(c);
    സ്വതന്ത്രം(c_gpu);
}

പൂർണ്ണസംഖ്യ test_gpu_blas()
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

    തിരികെ 0;
}
#endif

