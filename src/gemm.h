#ifndef GEMM_H
#define GEMM_H

void gemm_bin(정수 M, 정수 N, 정수 K, float ALPHA, 
        char  *A, 정수 lda, 
        float *B, 정수 ldb,
        float *C, 정수 ldc);
        
void gemm(정수 TA, 정수 TB, 정수 M, 정수 N, 정수 K, float ALPHA, 
                    float *A, 정수 lda, 
                    float *B, 정수 ldb,
                    float BETA,
                    float *C, 정수 ldc);

void gemm_cpu(정수 TA, 정수 TB, 정수 M, 정수 N, 정수 K, float ALPHA, 
        float *A, 정수 lda, 
        float *B, 정수 ldb,
        float BETA,
        float *C, 정수 ldc);

#ifdef GPU
void gemm_gpu(정수 TA, 정수 TB, 정수 M, 정수 N, 정수 K, float ALPHA, 
        float *A_gpu, 정수 lda, 
        float *B_gpu, 정수 ldb,
        float BETA,
        float *C_gpu, 정수 ldc);

void gemm_gpu(정수 TA, 정수 TB, 정수 M, 정수 N, 정수 K, float ALPHA, 
        float *A, 정수 lda, 
        float *B, 정수 ldb,
        float BETA,
        float *C, 정수 ldc);
#endif
#endif
