#ifndef GEMM_H
#define GEMM_H

void gemm_bin(ціле M, ціле N, ціле K, float ALPHA, 
        char  *A, ціле lda, 
        float *B, ціле ldb,
        float *C, ціле ldc);
        
void gemm(ціле TA, ціле TB, ціле M, ціле N, ціле K, float ALPHA, 
                    float *A, ціле lda, 
                    float *B, ціле ldb,
                    float BETA,
                    float *C, ціле ldc);

void gemm_cpu(ціле TA, ціле TB, ціле M, ціле N, ціле K, float ALPHA, 
        float *A, ціле lda, 
        float *B, ціле ldb,
        float BETA,
        float *C, ціле ldc);

#ifdef GPU
void gemm_gpu(ціле TA, ціле TB, ціле M, ціле N, ціле K, float ALPHA, 
        float *A_gpu, ціле lda, 
        float *B_gpu, ціле ldb,
        float BETA,
        float *C_gpu, ціле ldc);

void gemm_gpu(ціле TA, ціле TB, ціле M, ціле N, ціле K, float ALPHA, 
        float *A, ціле lda, 
        float *B, ціле ldb,
        float BETA,
        float *C, ціле ldc);
#endif
#endif
