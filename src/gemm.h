#ifndef GEMM_H
#define GEMM_H

void gemm_bin(จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        var_na  *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float *C, จำนวนเต็ม ldc);
        
void gemm(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
                    float *A, จำนวนเต็ม lda, 
                    float *B, จำนวนเต็ม ldb,
                    float BETA,
                    float *C, จำนวนเต็ม ldc);

void gemm_cpu(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float BETA,
        float *C, จำนวนเต็ม ldc);

#ifdef GPU
void gemm_gpu(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A_gpu, จำนวนเต็ม lda, 
        float *B_gpu, จำนวนเต็ม ldb,
        float BETA,
        float *C_gpu, จำนวนเต็ม ldc);

void gemm_gpu(จำนวนเต็ม TA, จำนวนเต็ม TB, จำนวนเต็ม M, จำนวนเต็ม N, จำนวนเต็ม K, float ALPHA, 
        float *A, จำนวนเต็ม lda, 
        float *B, จำนวนเต็ม ldb,
        float BETA,
        float *C, จำนวนเต็ม ldc);
#endif
#endif
