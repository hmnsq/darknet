#ifndef GEMM_H
#define GEMM_H

void gemm_bin(ཧྲིལ M, ཧྲིལ N, ཧྲིལ K, float ALPHA, 
        var_na  *A, ཧྲིལ lda, 
        float *B, ཧྲིལ ldb,
        float *C, ཧྲིལ ldc);
        
void gemm(ཧྲིལ TA, ཧྲིལ TB, ཧྲིལ M, ཧྲིལ N, ཧྲིལ K, float ALPHA, 
                    float *A, ཧྲིལ lda, 
                    float *B, ཧྲིལ ldb,
                    float BETA,
                    float *C, ཧྲིལ ldc);

void gemm_cpu(ཧྲིལ TA, ཧྲིལ TB, ཧྲིལ M, ཧྲིལ N, ཧྲིལ K, float ALPHA, 
        float *A, ཧྲིལ lda, 
        float *B, ཧྲིལ ldb,
        float BETA,
        float *C, ཧྲིལ ldc);

#ifdef GPU
void gemm_gpu(ཧྲིལ TA, ཧྲིལ TB, ཧྲིལ M, ཧྲིལ N, ཧྲིལ K, float ALPHA, 
        float *A_gpu, ཧྲིལ lda, 
        float *B_gpu, ཧྲིལ ldb,
        float BETA,
        float *C_gpu, ཧྲིལ ldc);

void gemm_gpu(ཧྲིལ TA, ཧྲིལ TB, ཧྲིལ M, ཧྲིལ N, ཧྲིལ K, float ALPHA, 
        float *A, ཧྲིལ lda, 
        float *B, ཧྲིལ ldb,
        float BETA,
        float *C, ཧྲིལ ldc);
#endif
#endif
