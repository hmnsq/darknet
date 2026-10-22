#ifndef GEMM_H
#define GEMM_H

void gemm_bin(整数 M, 整数 N, 整数 K, float ALPHA, 
        char  *A, 整数 lda, 
        float *B, 整数 ldb,
        float *C, 整数 ldc);
        
void gemm(整数 TA, 整数 TB, 整数 M, 整数 N, 整数 K, float ALPHA, 
                    float *A, 整数 lda, 
                    float *B, 整数 ldb,
                    float BETA,
                    float *C, 整数 ldc);

void gemm_cpu(整数 TA, 整数 TB, 整数 M, 整数 N, 整数 K, float ALPHA, 
        float *A, 整数 lda, 
        float *B, 整数 ldb,
        float BETA,
        float *C, 整数 ldc);

#ifdef GPU
void gemm_gpu(整数 TA, 整数 TB, 整数 M, 整数 N, 整数 K, float ALPHA, 
        float *A_gpu, 整数 lda, 
        float *B_gpu, 整数 ldb,
        float BETA,
        float *C_gpu, 整数 ldc);

void gemm_gpu(整数 TA, 整数 TB, 整数 M, 整数 N, 整数 K, float ALPHA, 
        float *A, 整数 lda, 
        float *B, 整数 ldb,
        float BETA,
        float *C, 整数 ldc);
#endif
#endif
