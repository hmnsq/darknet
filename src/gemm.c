#અન્તર્ભાવ "gemm.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <math.h>

શૂન્ય gemm_bin(પૂર્ણાંક M, પૂર્ણાંક N, પૂર્ણાંક K, દશાંશ ALPHA, 
        અક્ષર  *A, પૂર્ણાંક lda, 
        દશાંશ *B, પૂર્ણાંક ldb,
        દશાંશ *C, પૂર્ણાંક ldc)
{
    પૂર્ણાંક i,j,k;
    માટે(i = 0; i < M; ++i){
        માટે(k = 0; k < K; ++k){
            અક્ષર A_PART = A[i*lda+k];
            જો(A_PART){
                માટે(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } નહીંતર {
                માટે(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

દશાંશ *random_matrix(પૂર્ણાંક rows, પૂર્ણાંક cols)
{
    પૂર્ણાંક i;
    દશાંશ *m = calloc(rows*cols, કદ(દશાંશ));
    માટે(i = 0; i < rows*cols; ++i){
        m[i] = (દશાંશ)rand()/RAND_MAX;
    }
    પાછા m;
}

શૂન્ય time_random_matrix(પૂર્ણાંક TA, પૂર્ણાંક TB, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક n)
{
    દશાંશ *a;
    જો(!TA) a = random_matrix(m,k);
    નહીંતર a = random_matrix(k,m);
    પૂર્ણાંક lda = (!TA)?k:m;
    દશાંશ *b;
    જો(!TB) b = random_matrix(k,n);
    નહીંતર b = random_matrix(n,k);
    પૂર્ણાંક ldb = (!TB)?n:k;

    દશાંશ *c = random_matrix(m,n);
    પૂર્ણાંક i;
    clock_t start = clock(), end;
    માટે(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (દશાંશ)(end-start)/CLOCKS_PER_SEC);
    મુક્ત(a);
    મુક્ત(b);
    મુક્ત(c);
}


શૂન્ય gemm(પૂર્ણાંક TA, પૂર્ણાંક TB, પૂર્ણાંક M, પૂર્ણાંક N, પૂર્ણાંક K, દશાંશ ALPHA, 
        દશાંશ *A, પૂર્ણાંક lda, 
        દશાંશ *B, પૂર્ણાંક ldb,
        દશાંશ BETA,
        દશાંશ *C, પૂર્ણાંક ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

શૂન્ય gemm_nn(પૂર્ણાંક M, પૂર્ણાંક N, પૂર્ણાંક K, દશાંશ ALPHA, 
        દશાંશ *A, પૂર્ણાંક lda, 
        દશાંશ *B, પૂર્ણાંક ldb,
        દશાંશ *C, પૂર્ણાંક ldc)
{
    પૂર્ણાંક i,j,k;
    #pragma omp parallel માટે
    માટે(i = 0; i < M; ++i){
        માટે(k = 0; k < K; ++k){
            નોંધણી દશાંશ A_PART = ALPHA*A[i*lda+k];
            માટે(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

શૂન્ય gemm_nt(પૂર્ણાંક M, પૂર્ણાંક N, પૂર્ણાંક K, દશાંશ ALPHA, 
        દશાંશ *A, પૂર્ણાંક lda, 
        દશાંશ *B, પૂર્ણાંક ldb,
        દશાંશ *C, પૂર્ણાંક ldc)
{
    પૂર્ણાંક i,j,k;
    #pragma omp parallel માટે
    માટે(i = 0; i < M; ++i){
        માટે(j = 0; j < N; ++j){
            નોંધણી દશાંશ sum = 0;
            માટે(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

શૂન્ય gemm_tn(પૂર્ણાંક M, પૂર્ણાંક N, પૂર્ણાંક K, દશાંશ ALPHA, 
        દશાંશ *A, પૂર્ણાંક lda, 
        દશાંશ *B, પૂર્ણાંક ldb,
        દશાંશ *C, પૂર્ણાંક ldc)
{
    પૂર્ણાંક i,j,k;
    #pragma omp parallel માટે
    માટે(i = 0; i < M; ++i){
        માટે(k = 0; k < K; ++k){
            નોંધણી દશાંશ A_PART = ALPHA*A[k*lda+i];
            માટે(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

શૂન્ય gemm_tt(પૂર્ણાંક M, પૂર્ણાંક N, પૂર્ણાંક K, દશાંશ ALPHA, 
        દશાંશ *A, પૂર્ણાંક lda, 
        દશાંશ *B, પૂર્ણાંક ldb,
        દશાંશ *C, પૂર્ણાંક ldc)
{
    પૂર્ણાંક i,j,k;
    #pragma omp parallel માટે
    માટે(i = 0; i < M; ++i){
        માટે(j = 0; j < N; ++j){
            નોંધણી દશાંશ sum = 0;
            માટે(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


શૂન્ય gemm_cpu(પૂર્ણાંક TA, પૂર્ણાંક TB, પૂર્ણાંક M, પૂર્ણાંક N, પૂર્ણાંક K, દશાંશ ALPHA, 
        દશાંશ *A, પૂર્ણાંક lda, 
        દશાંશ *B, પૂર્ણાંક ldb,
        દશાંશ BETA,
        દશાંશ *C, પૂર્ણાંક ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    પૂર્ણાંક i, j;
    માટે(i = 0; i < M; ++i){
        માટે(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    જો(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    નહીંતર જો(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    નહીંતર જો(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    નહીંતર
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#અન્તર્ભાવ <math.h>

શૂન્ય gemm_gpu(પૂર્ણાંક TA, પૂર્ણાંક TB, પૂર્ણાંક M, પૂર્ણાંક N, પૂર્ણાંક K, દશાંશ ALPHA, 
        દશાંશ *A_gpu, પૂર્ણાંક lda, 
        દશાંશ *B_gpu, પૂર્ણાંક ldb,
        દશાંશ BETA,
        દશાંશ *C_gpu, પૂર્ણાંક ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <time.h>

શૂન્ય time_gpu_random_matrix(પૂર્ણાંક TA, પૂર્ણાંક TB, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક n)
{
    દશાંશ *a;
    જો(!TA) a = random_matrix(m,k);
    નહીંતર a = random_matrix(k,m);
    પૂર્ણાંક lda = (!TA)?k:m;
    દશાંશ *b;
    જો(!TB) b = random_matrix(k,n);
    નહીંતર b = random_matrix(n,k);
    પૂર્ણાંક ldb = (!TB)?n:k;

    દશાંશ *c = random_matrix(m,n);
    પૂર્ણાંક i;
    clock_t start = clock(), end;
    માટે(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (દશાંશ)(end-start)/CLOCKS_PER_SEC);
    મુક્ત(a);
    મુક્ત(b);
    મુક્ત(c);
}

શૂન્ય time_gpu(પૂર્ણાંક TA, પૂર્ણાંક TB, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક n)
{
    પૂર્ણાંક iter = 10;
    દશાંશ *a = random_matrix(m,k);
    દશાંશ *b = random_matrix(k,n);

    પૂર્ણાંક lda = (!TA)?k:m;
    પૂર્ણાંક ldb = (!TB)?n:k;

    દશાંશ *c = random_matrix(m,n);

    દશાંશ *a_cl = cuda_make_array(a, m*k);
    દશાંશ *b_cl = cuda_make_array(b, k*n);
    દશાંશ *c_cl = cuda_make_array(c, m*n);

    પૂર્ણાંક i;
    clock_t start = clock(), end;
    માટે(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    દ્વિગુણ flop = ((દ્વિગુણ)m)*n*(2.*k + 2.)*iter;
    દ્વિગુણ gflop = flop/pow(10., 9);
    end = clock();
    દ્વિગુણ seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    મુક્ત(a);
    મુક્ત(b);
    મુક્ત(c);
}


શૂન્ય test_gpu_accuracy(પૂર્ણાંક TA, પૂર્ણાંક TB, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક n)
{
    srand(0);
    દશાંશ *a;
    જો(!TA) a = random_matrix(m,k);
    નહીંતર a = random_matrix(k,m);
    પૂર્ણાંક lda = (!TA)?k:m;
    દશાંશ *b;
    જો(!TB) b = random_matrix(k,n);
    નહીંતર b = random_matrix(n,k);
    પૂર્ણાંક ldb = (!TB)?n:k;

    દશાંશ *c = random_matrix(m,n);
    દશાંશ *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*કદ(દશાંશ));
    memset(c_gpu, 0, m*n*કદ(દશાંશ));
    પૂર્ણાંક i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    દ્વિગુણ sse = 0;
    માટે(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    મુક્ત(a);
    મુક્ત(b);
    મુક્ત(c);
    મુક્ત(c_gpu);
}

પૂર્ણાંક test_gpu_blas()
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

    પાછા 0;
}
#endif

