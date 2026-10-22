#ifndef GEMM_H
#define GEMM_H

void gemm_bin(цяло M, цяло N, цяло K, float ALPHA, 
        char  *A, цяло lda, 
        float *B, цяло ldb,
        float *C, цяло ldc);
        
void gemm(цяло TA, цяло TB, цяло M, цяло N, цяло K, float ALPHA, 
                    float *A, цяло lda, 
                    float *B, цяло ldb,
                    float BETA,
                    float *C, цяло ldc);

void gemm_cpu(цяло TA, цяло TB, цяло M, цяло N, цяло K, float ALPHA, 
        float *A, цяло lda, 
        float *B, цяло ldb,
        float BETA,
        float *C, цяло ldc);

#ifdef GPU
void gemm_gpu(цяло TA, цяло TB, цяло M, цяло N, цяло K, float ALPHA, 
        float *A_gpu, цяло lda, 
        float *B_gpu, цяло ldb,
        float BETA,
        float *C_gpu, цяло ldc);

void gemm_gpu(цяло TA, цяло TB, цяло M, цяло N, цяло K, float ALPHA, 
        float *A, цяло lda, 
        float *B, цяло ldb,
        float BETA,
        float *C, цяло ldc);
#endif
#endif
