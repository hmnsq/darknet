#ifndef GEMM_H
#define GEMM_H

void gemm_bin(số_nguyên M, số_nguyên N, số_nguyên K, float ALPHA, 
        char  *A, số_nguyên lda, 
        float *B, số_nguyên ldb,
        float *C, số_nguyên ldc);
        
void gemm(số_nguyên TA, số_nguyên TB, số_nguyên M, số_nguyên N, số_nguyên K, float ALPHA, 
                    float *A, số_nguyên lda, 
                    float *B, số_nguyên ldb,
                    float BETA,
                    float *C, số_nguyên ldc);

void gemm_cpu(số_nguyên TA, số_nguyên TB, số_nguyên M, số_nguyên N, số_nguyên K, float ALPHA, 
        float *A, số_nguyên lda, 
        float *B, số_nguyên ldb,
        float BETA,
        float *C, số_nguyên ldc);

#ifdef GPU
void gemm_gpu(số_nguyên TA, số_nguyên TB, số_nguyên M, số_nguyên N, số_nguyên K, float ALPHA, 
        float *A_gpu, số_nguyên lda, 
        float *B_gpu, số_nguyên ldb,
        float BETA,
        float *C_gpu, số_nguyên ldc);

void gemm_gpu(số_nguyên TA, số_nguyên TB, số_nguyên M, số_nguyên N, số_nguyên K, float ALPHA, 
        float *A, số_nguyên lda, 
        float *B, số_nguyên ldb,
        float BETA,
        float *C, số_nguyên ldc);
#endif
#endif
