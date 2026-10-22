#अन्तर्भाव "gemm.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <math.h>

शून्य gemm_bin(पूर्णाङ्क M, पूर्णाङ्क N, पूर्णाङ्क K, भिन्न ALPHA, 
        वर्ण  *A, पूर्णाङ्क lda, 
        भिन्न *B, पूर्णाङ्क ldb,
        भिन्न *C, पूर्णाङ्क ldc)
{
    पूर्णाङ्क i,j,k;
    प्रति(i = 0; i < M; ++i){
        प्रति(k = 0; k < K; ++k){
            वर्ण A_PART = A[i*lda+k];
            यदि(A_PART){
                प्रति(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } अन्यथा {
                प्रति(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

भिन्न *random_matrix(पूर्णाङ्क rows, पूर्णाङ्क cols)
{
    पूर्णाङ्क i;
    भिन्न *m = calloc(rows*cols, परिमाण(भिन्न));
    प्रति(i = 0; i < rows*cols; ++i){
        m[i] = (भिन्न)rand()/RAND_MAX;
    }
    प्रत्यागम m;
}

शून्य time_random_matrix(पूर्णाङ्क TA, पूर्णाङ्क TB, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क n)
{
    भिन्न *a;
    यदि(!TA) a = random_matrix(m,k);
    अन्यथा a = random_matrix(k,m);
    पूर्णाङ्क lda = (!TA)?k:m;
    भिन्न *b;
    यदि(!TB) b = random_matrix(k,n);
    अन्यथा b = random_matrix(n,k);
    पूर्णाङ्क ldb = (!TB)?n:k;

    भिन्न *c = random_matrix(m,n);
    पूर्णाङ्क i;
    clock_t start = clock(), end;
    प्रति(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (भिन्न)(end-start)/CLOCKS_PER_SEC);
    मुक्त(a);
    मुक्त(b);
    मुक्त(c);
}


शून्य gemm(पूर्णाङ्क TA, पूर्णाङ्क TB, पूर्णाङ्क M, पूर्णाङ्क N, पूर्णाङ्क K, भिन्न ALPHA, 
        भिन्न *A, पूर्णाङ्क lda, 
        भिन्न *B, पूर्णाङ्क ldb,
        भिन्न BETA,
        भिन्न *C, पूर्णाङ्क ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

शून्य gemm_nn(पूर्णाङ्क M, पूर्णाङ्क N, पूर्णाङ्क K, भिन्न ALPHA, 
        भिन्न *A, पूर्णाङ्क lda, 
        भिन्न *B, पूर्णाङ्क ldb,
        भिन्न *C, पूर्णाङ्क ldc)
{
    पूर्णाङ्क i,j,k;
    #pragma omp parallel प्रति
    प्रति(i = 0; i < M; ++i){
        प्रति(k = 0; k < K; ++k){
            पञ्जिका भिन्न A_PART = ALPHA*A[i*lda+k];
            प्रति(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

शून्य gemm_nt(पूर्णाङ्क M, पूर्णाङ्क N, पूर्णाङ्क K, भिन्न ALPHA, 
        भिन्न *A, पूर्णाङ्क lda, 
        भिन्न *B, पूर्णाङ्क ldb,
        भिन्न *C, पूर्णाङ्क ldc)
{
    पूर्णाङ्क i,j,k;
    #pragma omp parallel प्रति
    प्रति(i = 0; i < M; ++i){
        प्रति(j = 0; j < N; ++j){
            पञ्जिका भिन्न sum = 0;
            प्रति(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

शून्य gemm_tn(पूर्णाङ्क M, पूर्णाङ्क N, पूर्णाङ्क K, भिन्न ALPHA, 
        भिन्न *A, पूर्णाङ्क lda, 
        भिन्न *B, पूर्णाङ्क ldb,
        भिन्न *C, पूर्णाङ्क ldc)
{
    पूर्णाङ्क i,j,k;
    #pragma omp parallel प्रति
    प्रति(i = 0; i < M; ++i){
        प्रति(k = 0; k < K; ++k){
            पञ्जिका भिन्न A_PART = ALPHA*A[k*lda+i];
            प्रति(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

शून्य gemm_tt(पूर्णाङ्क M, पूर्णाङ्क N, पूर्णाङ्क K, भिन्न ALPHA, 
        भिन्न *A, पूर्णाङ्क lda, 
        भिन्न *B, पूर्णाङ्क ldb,
        भिन्न *C, पूर्णाङ्क ldc)
{
    पूर्णाङ्क i,j,k;
    #pragma omp parallel प्रति
    प्रति(i = 0; i < M; ++i){
        प्रति(j = 0; j < N; ++j){
            पञ्जिका भिन्न sum = 0;
            प्रति(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


शून्य gemm_cpu(पूर्णाङ्क TA, पूर्णाङ्क TB, पूर्णाङ्क M, पूर्णाङ्क N, पूर्णाङ्क K, भिन्न ALPHA, 
        भिन्न *A, पूर्णाङ्क lda, 
        भिन्न *B, पूर्णाङ्क ldb,
        भिन्न BETA,
        भिन्न *C, पूर्णाङ्क ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    पूर्णाङ्क i, j;
    प्रति(i = 0; i < M; ++i){
        प्रति(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    यदि(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    अन्यथा यदि(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    अन्यथा यदि(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    अन्यथा
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#अन्तर्भाव <math.h>

शून्य gemm_gpu(पूर्णाङ्क TA, पूर्णाङ्क TB, पूर्णाङ्क M, पूर्णाङ्क N, पूर्णाङ्क K, भिन्न ALPHA, 
        भिन्न *A_gpu, पूर्णाङ्क lda, 
        भिन्न *B_gpu, पूर्णाङ्क ldb,
        भिन्न BETA,
        भिन्न *C_gpu, पूर्णाङ्क ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t status = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(status);
}

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <time.h>

शून्य time_gpu_random_matrix(पूर्णाङ्क TA, पूर्णाङ्क TB, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क n)
{
    भिन्न *a;
    यदि(!TA) a = random_matrix(m,k);
    अन्यथा a = random_matrix(k,m);
    पूर्णाङ्क lda = (!TA)?k:m;
    भिन्न *b;
    यदि(!TB) b = random_matrix(k,n);
    अन्यथा b = random_matrix(n,k);
    पूर्णाङ्क ldb = (!TB)?n:k;

    भिन्न *c = random_matrix(m,n);
    पूर्णाङ्क i;
    clock_t start = clock(), end;
    प्रति(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = clock();
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (भिन्न)(end-start)/CLOCKS_PER_SEC);
    मुक्त(a);
    मुक्त(b);
    मुक्त(c);
}

शून्य time_gpu(पूर्णाङ्क TA, पूर्णाङ्क TB, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क n)
{
    पूर्णाङ्क iter = 10;
    भिन्न *a = random_matrix(m,k);
    भिन्न *b = random_matrix(k,n);

    पूर्णाङ्क lda = (!TA)?k:m;
    पूर्णाङ्क ldb = (!TB)?n:k;

    भिन्न *c = random_matrix(m,n);

    भिन्न *a_cl = cuda_make_array(a, m*k);
    भिन्न *b_cl = cuda_make_array(b, k*n);
    भिन्न *c_cl = cuda_make_array(c, m*n);

    पूर्णाङ्क i;
    clock_t start = clock(), end;
    प्रति(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    द्विगुण flop = ((द्विगुण)m)*n*(2.*k + 2.)*iter;
    द्विगुण gflop = flop/pow(10., 9);
    end = clock();
    द्विगुण seconds = sec(end-start);
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    मुक्त(a);
    मुक्त(b);
    मुक्त(c);
}


शून्य test_gpu_accuracy(पूर्णाङ्क TA, पूर्णाङ्क TB, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क n)
{
    srand(0);
    भिन्न *a;
    यदि(!TA) a = random_matrix(m,k);
    अन्यथा a = random_matrix(k,m);
    पूर्णाङ्क lda = (!TA)?k:m;
    भिन्न *b;
    यदि(!TB) b = random_matrix(k,n);
    अन्यथा b = random_matrix(n,k);
    पूर्णाङ्क ldb = (!TB)?n:k;

    भिन्न *c = random_matrix(m,n);
    भिन्न *c_gpu = random_matrix(m,n);
    memset(c, 0, m*n*परिमाण(भिन्न));
    memset(c_gpu, 0, m*n*परिमाण(भिन्न));
    पूर्णाङ्क i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    द्विगुण sse = 0;
    प्रति(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += pow(c[i]-c_gpu[i], 2);
    }
    printf("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    मुक्त(a);
    मुक्त(b);
    मुक्त(c);
    मुक्त(c_gpu);
}

पूर्णाङ्क test_gpu_blas()
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

    प्रत्यागम 0;
}
#endif

