#अन्तर्भाव "gemm.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <math.h>

शून्य gemm_bin(पूर्णांक M, पूर्णांक N, पूर्णांक K, दशांश ALPHA, 
        अक्षर  *A, पूर्णांक lda, 
        दशांश *B, पूर्णांक ldb,
        दशांश *C, पूर्णांक ldc)
{
    पूर्णांक i,j,k;
    साठी(i = 0; i < M; ++i){
        साठी(k = 0; k < K; ++k){
            अक्षर A_PART = A[i*lda+k];
            जर(A_PART){
                साठी(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } अन्यथा {
                साठी(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

दशांश *random_matrix(पूर्णांक rows, पूर्णांक cols)
{
    पूर्णांक i;
    दशांश *m = सुस्मृति(rows*cols, आकार(दशांश));
    साठी(i = 0; i < rows*cols; ++i){
        m[i] = (दशांश)अक्रम()/अक्रम_उच्च;
    }
    परत m;
}

शून्य time_random_matrix(पूर्णांक TA, पूर्णांक TB, पूर्णांक m, पूर्णांक k, पूर्णांक n)
{
    दशांश *a;
    जर(!TA) a = random_matrix(m,k);
    अन्यथा a = random_matrix(k,m);
    पूर्णांक lda = (!TA)?k:m;
    दशांश *b;
    जर(!TB) b = random_matrix(k,n);
    अन्यथा b = random_matrix(n,k);
    पूर्णांक ldb = (!TB)?n:k;

    दशांश *c = random_matrix(m,n);
    पूर्णांक i;
    घड़ी_प्रकार start = घड़ी(), end;
    साठी(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = घड़ी();
    म_लिखो("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (दशांश)(end-start)/CLOCKS_PER_SEC);
    मुक्त(a);
    मुक्त(b);
    मुक्त(c);
}


शून्य gemm(पूर्णांक TA, पूर्णांक TB, पूर्णांक M, पूर्णांक N, पूर्णांक K, दशांश ALPHA, 
        दशांश *A, पूर्णांक lda, 
        दशांश *B, पूर्णांक ldb,
        दशांश BETA,
        दशांश *C, पूर्णांक ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

शून्य gemm_nn(पूर्णांक M, पूर्णांक N, पूर्णांक K, दशांश ALPHA, 
        दशांश *A, पूर्णांक lda, 
        दशांश *B, पूर्णांक ldb,
        दशांश *C, पूर्णांक ldc)
{
    पूर्णांक i,j,k;
    #pragma omp parallel साठी
    साठी(i = 0; i < M; ++i){
        साठी(k = 0; k < K; ++k){
            नोंदणी दशांश A_PART = ALPHA*A[i*lda+k];
            साठी(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

शून्य gemm_nt(पूर्णांक M, पूर्णांक N, पूर्णांक K, दशांश ALPHA, 
        दशांश *A, पूर्णांक lda, 
        दशांश *B, पूर्णांक ldb,
        दशांश *C, पूर्णांक ldc)
{
    पूर्णांक i,j,k;
    #pragma omp parallel साठी
    साठी(i = 0; i < M; ++i){
        साठी(j = 0; j < N; ++j){
            नोंदणी दशांश sum = 0;
            साठी(k = 0; k < K; ++k){
                sum += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += sum;
        }
    }
}

शून्य gemm_tn(पूर्णांक M, पूर्णांक N, पूर्णांक K, दशांश ALPHA, 
        दशांश *A, पूर्णांक lda, 
        दशांश *B, पूर्णांक ldb,
        दशांश *C, पूर्णांक ldc)
{
    पूर्णांक i,j,k;
    #pragma omp parallel साठी
    साठी(i = 0; i < M; ++i){
        साठी(k = 0; k < K; ++k){
            नोंदणी दशांश A_PART = ALPHA*A[k*lda+i];
            साठी(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

शून्य gemm_tt(पूर्णांक M, पूर्णांक N, पूर्णांक K, दशांश ALPHA, 
        दशांश *A, पूर्णांक lda, 
        दशांश *B, पूर्णांक ldb,
        दशांश *C, पूर्णांक ldc)
{
    पूर्णांक i,j,k;
    #pragma omp parallel साठी
    साठी(i = 0; i < M; ++i){
        साठी(j = 0; j < N; ++j){
            नोंदणी दशांश sum = 0;
            साठी(k = 0; k < K; ++k){
                sum += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += sum;
        }
    }
}


शून्य gemm_cpu(पूर्णांक TA, पूर्णांक TB, पूर्णांक M, पूर्णांक N, पूर्णांक K, दशांश ALPHA, 
        दशांश *A, पूर्णांक lda, 
        दशांश *B, पूर्णांक ldb,
        दशांश BETA,
        दशांश *C, पूर्णांक ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    पूर्णांक i, j;
    साठी(i = 0; i < M; ++i){
        साठी(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    जर(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    अन्यथा जर(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    अन्यथा जर(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    अन्यथा
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#ifdef GPU

#अन्तर्भाव <math.h>

शून्य gemm_gpu(पूर्णांक TA, पूर्णांक TB, पूर्णांक M, पूर्णांक N, पूर्णांक K, दशांश ALPHA, 
        दशांश *A_gpu, पूर्णांक lda, 
        दशांश *B_gpu, पूर्णांक ldb,
        दशांश BETA,
        दशांश *C_gpu, पूर्णांक ldc)
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

शून्य time_gpu_random_matrix(पूर्णांक TA, पूर्णांक TB, पूर्णांक m, पूर्णांक k, पूर्णांक n)
{
    दशांश *a;
    जर(!TA) a = random_matrix(m,k);
    अन्यथा a = random_matrix(k,m);
    पूर्णांक lda = (!TA)?k:m;
    दशांश *b;
    जर(!TB) b = random_matrix(k,n);
    अन्यथा b = random_matrix(n,k);
    पूर्णांक ldb = (!TB)?n:k;

    दशांश *c = random_matrix(m,n);
    पूर्णांक i;
    घड़ी_प्रकार start = घड़ी(), end;
    साठी(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    end = घड़ी();
    म_लिखो("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (दशांश)(end-start)/CLOCKS_PER_SEC);
    मुक्त(a);
    मुक्त(b);
    मुक्त(c);
}

शून्य time_gpu(पूर्णांक TA, पूर्णांक TB, पूर्णांक m, पूर्णांक k, पूर्णांक n)
{
    पूर्णांक iter = 10;
    दशांश *a = random_matrix(m,k);
    दशांश *b = random_matrix(k,n);

    पूर्णांक lda = (!TA)?k:m;
    पूर्णांक ldb = (!TB)?n:k;

    दशांश *c = random_matrix(m,n);

    दशांश *a_cl = cuda_make_array(a, m*k);
    दशांश *b_cl = cuda_make_array(b, k*n);
    दशांश *c_cl = cuda_make_array(c, m*n);

    पूर्णांक i;
    घड़ी_प्रकार start = घड़ी(), end;
    साठी(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    द्विगुण flop = ((द्विगुण)m)*n*(2.*k + 2.)*iter;
    द्विगुण gflop = flop/घात(10., 9);
    end = घड़ी();
    द्विगुण seconds = sec(end-start);
    म_लिखो("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    मुक्त(a);
    मुक्त(b);
    मुक्त(c);
}


शून्य test_gpu_accuracy(पूर्णांक TA, पूर्णांक TB, पूर्णांक m, पूर्णांक k, पूर्णांक n)
{
    बेक्रम(0);
    दशांश *a;
    जर(!TA) a = random_matrix(m,k);
    अन्यथा a = random_matrix(k,m);
    पूर्णांक lda = (!TA)?k:m;
    दशांश *b;
    जर(!TB) b = random_matrix(k,n);
    अन्यथा b = random_matrix(n,k);
    पूर्णांक ldb = (!TB)?n:k;

    दशांश *c = random_matrix(m,n);
    दशांश *c_gpu = random_matrix(m,n);
    स_रखो(c, 0, m*n*आकार(दशांश));
    स_रखो(c_gpu, 0, m*n*आकार(दशांश));
    पूर्णांक i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    द्विगुण sse = 0;
    साठी(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += घात(c[i]-c_gpu[i], 2);
    }
    म_लिखो("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    मुक्त(a);
    मुक्त(b);
    मुक्त(c);
    मुक्त(c_gpu);
}

पूर्णांक test_gpu_blas()
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

    परत 0;
}
#endif

