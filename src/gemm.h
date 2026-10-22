#ifndef GEMM_H
#define GEMM_H

void gemm_bin(цео M, цео N, цео K, float ALPHA, 
        char  *A, цео lda, 
        float *B, цео ldb,
        float *C, цео ldc);
        
void gemm(цео TA, цео TB, цео M, цео N, цео K, float ALPHA, 
                    float *A, цео lda, 
                    float *B, цео ldb,
                    float BETA,
                    float *C, цео ldc);

void gemm_cpu(цео TA, цео TB, цео M, цео N, цео K, float ALPHA, 
        float *A, цео lda, 
        float *B, цео ldb,
        float BETA,
        float *C, цео ldc);

#ifdef GPU
void gemm_gpu(цео TA, цео TB, цео M, цео N, цео K, float ALPHA, 
        float *A_gpu, цео lda, 
        float *B_gpu, цео ldb,
        float BETA,
        float *C_gpu, цео ldc);

void gemm_gpu(цео TA, цео TB, цео M, цео N, цео K, float ALPHA, 
        float *A, цео lda, 
        float *B, цео ldb,
        float BETA,
        float *C, цео ldc);
#endif
#endif
