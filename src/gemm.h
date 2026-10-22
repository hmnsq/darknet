#ifndef GEMM_H
#define GEMM_H

void gemm_bin(целое M, целое N, целое K, float ALPHA, 
        char  *A, целое lda, 
        float *B, целое ldb,
        float *C, целое ldc);
        
void gemm(целое TA, целое TB, целое M, целое N, целое K, float ALPHA, 
                    float *A, целое lda, 
                    float *B, целое ldb,
                    float BETA,
                    float *C, целое ldc);

void gemm_cpu(целое TA, целое TB, целое M, целое N, целое K, float ALPHA, 
        float *A, целое lda, 
        float *B, целое ldb,
        float BETA,
        float *C, целое ldc);

#ifdef GPU
void gemm_gpu(целое TA, целое TB, целое M, целое N, целое K, float ALPHA, 
        float *A_gpu, целое lda, 
        float *B_gpu, целое ldb,
        float BETA,
        float *C_gpu, целое ldc);

void gemm_gpu(целое TA, целое TB, целое M, целое N, целое K, float ALPHA, 
        float *A, целое lda, 
        float *B, целое ldb,
        float BETA,
        float *C, целое ldc);
#endif
#endif
