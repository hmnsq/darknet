#ifndef GEMM_H
#define GEMM_H

void gemm_bin(ចំនួនគត់ M, ចំនួនគត់ N, ចំនួនគត់ K, float ALPHA, 
        var_na  *A, ចំនួនគត់ lda, 
        float *B, ចំនួនគត់ ldb,
        float *C, ចំនួនគត់ ldc);
        
void gemm(ចំនួនគត់ TA, ចំនួនគត់ TB, ចំនួនគត់ M, ចំនួនគត់ N, ចំនួនគត់ K, float ALPHA, 
                    float *A, ចំនួនគត់ lda, 
                    float *B, ចំនួនគត់ ldb,
                    float BETA,
                    float *C, ចំនួនគត់ ldc);

void gemm_cpu(ចំនួនគត់ TA, ចំនួនគត់ TB, ចំនួនគត់ M, ចំនួនគត់ N, ចំនួនគត់ K, float ALPHA, 
        float *A, ចំនួនគត់ lda, 
        float *B, ចំនួនគត់ ldb,
        float BETA,
        float *C, ចំនួនគត់ ldc);

#ifdef GPU
void gemm_gpu(ចំនួនគត់ TA, ចំនួនគត់ TB, ចំនួនគត់ M, ចំនួនគត់ N, ចំនួនគត់ K, float ALPHA, 
        float *A_gpu, ចំនួនគត់ lda, 
        float *B_gpu, ចំនួនគត់ ldb,
        float BETA,
        float *C_gpu, ចំនួនគត់ ldc);

void gemm_gpu(ចំនួនគត់ TA, ចំនួនគត់ TB, ចំនួនគត់ M, ចំនួនគត់ N, ចំនួនគត់ K, float ALPHA, 
        float *A, ចំនួនគត់ lda, 
        float *B, ចំនួនគត់ ldb,
        float BETA,
        float *C, ចំនួនគត់ ldc);
#endif
#endif
