#ifndef GEMM_H
#define GEMM_H

void gemm_bin(පූර්ණ_සංඛ්‍යා M, පූර්ණ_සංඛ්‍යා N, පූර්ණ_සංඛ්‍යා K, float ALPHA, 
        var_na  *A, පූර්ණ_සංඛ්‍යා lda, 
        float *B, පූර්ණ_සංඛ්‍යා ldb,
        float *C, පූර්ණ_සංඛ්‍යා ldc);
        
void gemm(පූර්ණ_සංඛ්‍යා TA, පූර්ණ_සංඛ්‍යා TB, පූර්ණ_සංඛ්‍යා M, පූර්ණ_සංඛ්‍යා N, පූර්ණ_සංඛ්‍යා K, float ALPHA, 
                    float *A, පූර්ණ_සංඛ්‍යා lda, 
                    float *B, පූර්ණ_සංඛ්‍යා ldb,
                    float BETA,
                    float *C, පූර්ණ_සංඛ්‍යා ldc);

void gemm_cpu(පූර්ණ_සංඛ්‍යා TA, පූර්ණ_සංඛ්‍යා TB, පූර්ණ_සංඛ්‍යා M, පූර්ණ_සංඛ්‍යා N, පූර්ණ_සංඛ්‍යා K, float ALPHA, 
        float *A, පූර්ණ_සංඛ්‍යා lda, 
        float *B, පූර්ණ_සංඛ්‍යා ldb,
        float BETA,
        float *C, පූර්ණ_සංඛ්‍යා ldc);

#ifdef GPU
void gemm_gpu(පූර්ණ_සංඛ්‍යා TA, පූර්ණ_සංඛ්‍යා TB, පූර්ණ_සංඛ්‍යා M, පූර්ණ_සංඛ්‍යා N, පූර්ණ_සංඛ්‍යා K, float ALPHA, 
        float *A_gpu, පූර්ණ_සංඛ්‍යා lda, 
        float *B_gpu, පූර්ණ_සංඛ්‍යා ldb,
        float BETA,
        float *C_gpu, පූර්ණ_සංඛ්‍යා ldc);

void gemm_gpu(පූර්ණ_සංඛ්‍යා TA, පූර්ණ_සංඛ්‍යා TB, පූර්ණ_සංඛ්‍යා M, පූර්ණ_සංඛ්‍යා N, පූර්ණ_සංඛ්‍යා K, float ALPHA, 
        float *A, පූර්ණ_සංඛ්‍යා lda, 
        float *B, පූර්ණ_සංඛ්‍යා ldb,
        float BETA,
        float *C, පූර්ණ_සංඛ්‍යා ldc);
#endif
#endif
