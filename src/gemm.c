#అన్తర్భావ "gemm.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <math.h>

శూన్య gemm_bin(పూర్ణాంకం M, పూర్ణాంకం N, పూర్ణాంకం K, దశాంశం ALPHA, 
        అక్షరం  *A, పూర్ణాంకం lda, 
        దశాంశం *B, పూర్ణాంకం ldb,
        దశాంశం *C, పూర్ణాంకం ldc)
{
    పూర్ణాంకం i,j,k;
    కొరకు(i = 0; i < M; ++i){
        కొరకు(k = 0; k < K; ++k){
            అక్షరం A_PART = A[i*lda+k];
            ఒకవేళ(A_PART){
                కొరకు(j = 0; j < N; ++j){
                    C[i*ldc+j] += B[k*ldb+j];
                }
            } లేకపోతే {
                కొరకు(j = 0; j < N; ++j){
                    C[i*ldc+j] -= B[k*ldb+j];
                }
            }
        }
    }
}

దశాంశం *random_మాతృక(పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య)
{
    పూర్ణాంకం i;
    దశాంశం *m = సున్నా_జ్ఞాపకం_ఇవ్వు(అడ్డువరుసలు*నిలువుల_సంఖ్య, పరిమాణం(దశాంశం));
    కొరకు(i = 0; i < అడ్డువరుసలు*నిలువుల_సంఖ్య; ++i){
        m[i] = (దశాంశం)rand()/RAND_MAX;
    }
    తిరిగి m;
}

శూన్య సమయం_random_మాతృక(పూర్ణాంకం TA, పూర్ణాంకం TB, పూర్ణాంకం m, పూర్ణాంకం k, పూర్ణాంకం n)
{
    దశాంశం *a;
    ఒకవేళ(!TA) a = random_మాతృక(m,k);
    లేకపోతే a = random_మాతృక(k,m);
    పూర్ణాంకం lda = (!TA)?k:m;
    దశాంశం *b;
    ఒకవేళ(!TB) b = random_మాతృక(k,n);
    లేకపోతే b = random_మాతృక(n,k);
    పూర్ణాంకం ldb = (!TB)?n:k;

    దశాంశం *c = random_మాతృక(m,n);
    పూర్ణాంకం i;
    clock_t ప్రారంభం = clock(), చివర;
    కొరకు(i = 0; i<10; ++i){
        gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    చివర = clock();
    రాయి("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf ms\n",m,k,k,n, TA, TB, (దశాంశం)(చివర-ప్రారంభం)/CLOCKS_PER_SEC);
    విడుదల(a);
    విడుదల(b);
    విడుదల(c);
}


శూన్య gemm(పూర్ణాంకం TA, పూర్ణాంకం TB, పూర్ణాంకం M, పూర్ణాంకం N, పూర్ణాంకం K, దశాంశం ALPHA, 
        దశాంశం *A, పూర్ణాంకం lda, 
        దశాంశం *B, పూర్ణాంకం ldb,
        దశాంశం BETA,
        దశాంశం *C, పూర్ణాంకం ldc)
{
    gemm_cpu( TA,  TB,  M, N, K, ALPHA,A,lda, B, ldb,BETA,C,ldc);
}

శూన్య gemm_nn(పూర్ణాంకం M, పూర్ణాంకం N, పూర్ణాంకం K, దశాంశం ALPHA, 
        దశాంశం *A, పూర్ణాంకం lda, 
        దశాంశం *B, పూర్ణాంకం ldb,
        దశాంశం *C, పూర్ణాంకం ldc)
{
    పూర్ణాంకం i,j,k;
    #సూచన omp parallel కొరకు
    కొరకు(i = 0; i < M; ++i){
        కొరకు(k = 0; k < K; ++k){
            నమోదు దశాంశం A_PART = ALPHA*A[i*lda+k];
            కొరకు(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

శూన్య gemm_nt(పూర్ణాంకం M, పూర్ణాంకం N, పూర్ణాంకం K, దశాంశం ALPHA, 
        దశాంశం *A, పూర్ణాంకం lda, 
        దశాంశం *B, పూర్ణాంకం ldb,
        దశాంశం *C, పూర్ణాంకం ldc)
{
    పూర్ణాంకం i,j,k;
    #సూచన omp parallel కొరకు
    కొరకు(i = 0; i < M; ++i){
        కొరకు(j = 0; j < N; ++j){
            నమోదు దశాంశం కూడిక = 0;
            కొరకు(k = 0; k < K; ++k){
                కూడిక += ALPHA*A[i*lda+k]*B[j*ldb + k];
            }
            C[i*ldc+j] += కూడిక;
        }
    }
}

శూన్య gemm_tn(పూర్ణాంకం M, పూర్ణాంకం N, పూర్ణాంకం K, దశాంశం ALPHA, 
        దశాంశం *A, పూర్ణాంకం lda, 
        దశాంశం *B, పూర్ణాంకం ldb,
        దశాంశం *C, పూర్ణాంకం ldc)
{
    పూర్ణాంకం i,j,k;
    #సూచన omp parallel కొరకు
    కొరకు(i = 0; i < M; ++i){
        కొరకు(k = 0; k < K; ++k){
            నమోదు దశాంశం A_PART = ALPHA*A[k*lda+i];
            కొరకు(j = 0; j < N; ++j){
                C[i*ldc+j] += A_PART*B[k*ldb+j];
            }
        }
    }
}

శూన్య gemm_tt(పూర్ణాంకం M, పూర్ణాంకం N, పూర్ణాంకం K, దశాంశం ALPHA, 
        దశాంశం *A, పూర్ణాంకం lda, 
        దశాంశం *B, పూర్ణాంకం ldb,
        దశాంశం *C, పూర్ణాంకం ldc)
{
    పూర్ణాంకం i,j,k;
    #సూచన omp parallel కొరకు
    కొరకు(i = 0; i < M; ++i){
        కొరకు(j = 0; j < N; ++j){
            నమోదు దశాంశం కూడిక = 0;
            కొరకు(k = 0; k < K; ++k){
                కూడిక += ALPHA*A[i+k*lda]*B[k+j*ldb];
            }
            C[i*ldc+j] += కూడిక;
        }
    }
}


శూన్య gemm_cpu(పూర్ణాంకం TA, పూర్ణాంకం TB, పూర్ణాంకం M, పూర్ణాంకం N, పూర్ణాంకం K, దశాంశం ALPHA, 
        దశాంశం *A, పూర్ణాంకం lda, 
        దశాంశం *B, పూర్ణాంకం ldb,
        దశాంశం BETA,
        దశాంశం *C, పూర్ణాంకం ldc)
{
    //printf("cpu: %d %d %d %d %d %f %d %d %f %d\n",TA, TB, M, N, K, ALPHA, lda, ldb, BETA, ldc);
    పూర్ణాంకం i, j;
    కొరకు(i = 0; i < M; ++i){
        కొరకు(j = 0; j < N; ++j){
            C[i*ldc + j] *= BETA;
        }
    }
    ఒకవేళ(!TA && !TB)
        gemm_nn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    లేకపోతే ఒకవేళ(TA && !TB)
        gemm_tn(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    లేకపోతే ఒకవేళ(!TA && TB)
        gemm_nt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
    లేకపోతే
        gemm_tt(M, N, K, ALPHA,A,lda, B, ldb,C,ldc);
}

#నిర్వచితమైతే GPU

#అన్తర్భావ <math.h>

శూన్య gemm_gpu(పూర్ణాంకం TA, పూర్ణాంకం TB, పూర్ణాంకం M, పూర్ణాంకం N, పూర్ణాంకం K, దశాంశం ALPHA, 
        దశాంశం *A_gpu, పూర్ణాంకం lda, 
        దశాంశం *B_gpu, పూర్ణాంకం ldb,
        దశాంశం BETA,
        దశాంశం *C_gpu, పూర్ణాంకం ldc)
{
    cublasHandle_t handle = blas_handle();
    cudaError_t స్థితి_2 = cublasSgemm(handle, (TB ? CUBLAS_OP_T : CUBLAS_OP_N), 
            (TA ? CUBLAS_OP_T : CUBLAS_OP_N), N, M, K, &ALPHA, B_gpu, ldb, A_gpu, lda, &BETA, C_gpu, ldc);
    check_error(స్థితి_2);
}

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <time.h>

శూన్య సమయం_gpu_random_మాతృక(పూర్ణాంకం TA, పూర్ణాంకం TB, పూర్ణాంకం m, పూర్ణాంకం k, పూర్ణాంకం n)
{
    దశాంశం *a;
    ఒకవేళ(!TA) a = random_మాతృక(m,k);
    లేకపోతే a = random_మాతృక(k,m);
    పూర్ణాంకం lda = (!TA)?k:m;
    దశాంశం *b;
    ఒకవేళ(!TB) b = random_మాతృక(k,n);
    లేకపోతే b = random_మాతృక(n,k);
    పూర్ణాంకం ldb = (!TB)?n:k;

    దశాంశం *c = random_మాతృక(m,n);
    పూర్ణాంకం i;
    clock_t ప్రారంభం = clock(), చివర;
    కొరకు(i = 0; i<32; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    }
    చివర = clock();
    రాయి("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s\n",m,k,k,n, TA, TB, (దశాంశం)(చివర-ప్రారంభం)/CLOCKS_PER_SEC);
    విడుదల(a);
    విడుదల(b);
    విడుదల(c);
}

శూన్య సమయం_gpu(పూర్ణాంకం TA, పూర్ణాంకం TB, పూర్ణాంకం m, పూర్ణాంకం k, పూర్ణాంకం n)
{
    పూర్ణాంకం iter = 10;
    దశాంశం *a = random_మాతృక(m,k);
    దశాంశం *b = random_మాతృక(k,n);

    పూర్ణాంకం lda = (!TA)?k:m;
    పూర్ణాంకం ldb = (!TB)?n:k;

    దశాంశం *c = random_మాతృక(m,n);

    దశాంశం *a_cl = cuda_make_array(a, m*k);
    దశాంశం *b_cl = cuda_make_array(b, k*n);
    దశాంశం *c_cl = cuda_make_array(c, m*n);

    పూర్ణాంకం i;
    clock_t ప్రారంభం = clock(), చివర;
    కొరకు(i = 0; i<iter; ++i){
        gemm_gpu(TA,TB,m,n,k,1,a_cl,lda,b_cl,ldb,1,c_cl,n);
        cudaThreadSynchronize();
    }
    ద్విగుణ flop = ((ద్విగుణ)m)*n*(2.*k + 2.)*iter;
    ద్విగుణ gflop = flop/ఘాతం(10., 9);
    చివర = clock();
    ద్విగుణ seconds = sec(చివర-ప్రారంభం);
    రాయి("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %lf s, %lf GFLOPS\n",m,k,k,n, TA, TB, seconds, gflop/seconds);
    cuda_free(a_cl);
    cuda_free(b_cl);
    cuda_free(c_cl);
    విడుదల(a);
    విడుదల(b);
    విడుదల(c);
}


శూన్య test_gpu_accuracy(పూర్ణాంకం TA, పూర్ణాంకం TB, పూర్ణాంకం m, పూర్ణాంకం k, పూర్ణాంకం n)
{
    srand(0);
    దశాంశం *a;
    ఒకవేళ(!TA) a = random_మాతృక(m,k);
    లేకపోతే a = random_మాతృక(k,m);
    పూర్ణాంకం lda = (!TA)?k:m;
    దశాంశం *b;
    ఒకవేళ(!TB) b = random_మాతృక(k,n);
    లేకపోతే b = random_మాతృక(n,k);
    పూర్ణాంకం ldb = (!TB)?n:k;

    దశాంశం *c = random_మాతృక(m,n);
    దశాంశం *c_gpu = random_మాతృక(m,n);
    memset(c, 0, m*n*పరిమాణం(దశాంశం));
    memset(c_gpu, 0, m*n*పరిమాణం(దశాంశం));
    పూర్ణాంకం i;
    //pm(m,k,b);
    gemm_gpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c_gpu,n);
    //printf("GPU\n");
    //pm(m, n, c_gpu);

    gemm_cpu(TA,TB,m,n,k,1,a,lda,b,ldb,1,c,n);
    //printf("\n\nCPU\n");
    //pm(m, n, c);
    ద్విగుణ sse = 0;
    కొరకు(i = 0; i < m*n; ++i) {
        //printf("%f %f\n", c[i], c_gpu[i]);
        sse += ఘాతం(c[i]-c_gpu[i], 2);
    }
    రాయి("Matrix Multiplication %dx%d * %dx%d, TA=%d, TB=%d: %g SSE\n",m,k,k,n, TA, TB, sse/(m*n));
    విడుదల(a);
    విడుదల(b);
    విడుదల(c);
    విడుదల(c_gpu);
}

పూర్ణాంకం test_gpu_blas()
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
    సమయం_gpu(0,0,64,75,12544); 
    సమయం_gpu(0,0,64,75,12544); 
    సమయం_gpu(0,0,64,75,12544); 
    సమయం_gpu(0,0,64,576,12544); 
    సమయం_gpu(0,0,256,2304,784); 
    సమయం_gpu(1,1,2304,256,784); 
    సమయం_gpu(0,0,512,4608,196); 
    సమయం_gpu(1,1,4608,512,196); 

    తిరిగి 0;
}
#అయితే_ముగింపు

