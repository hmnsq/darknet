#ifndef GEMM_H
#define GEMM_H

void gemm_bin(ကိန်းပြည့် M, ကိန်းပြည့် N, ကိန်းပြည့် K, float ALPHA, 
        var_na  *A, ကိန်းပြည့် lda, 
        float *B, ကိန်းပြည့် ldb,
        float *C, ကိန်းပြည့် ldc);
        
void gemm(ကိန်းပြည့် TA, ကိန်းပြည့် TB, ကိန်းပြည့် M, ကိန်းပြည့် N, ကိန်းပြည့် K, float ALPHA, 
                    float *A, ကိန်းပြည့် lda, 
                    float *B, ကိန်းပြည့် ldb,
                    float BETA,
                    float *C, ကိန်းပြည့် ldc);

void gemm_cpu(ကိန်းပြည့် TA, ကိန်းပြည့် TB, ကိန်းပြည့် M, ကိန်းပြည့် N, ကိန်းပြည့် K, float ALPHA, 
        float *A, ကိန်းပြည့် lda, 
        float *B, ကိန်းပြည့် ldb,
        float BETA,
        float *C, ကိန်းပြည့် ldc);

#ifdef GPU
void gemm_gpu(ကိန်းပြည့် TA, ကိန်းပြည့် TB, ကိန်းပြည့် M, ကိန်းပြည့် N, ကိန်းပြည့် K, float ALPHA, 
        float *A_gpu, ကိန်းပြည့် lda, 
        float *B_gpu, ကိန်းပြည့် ldb,
        float BETA,
        float *C_gpu, ကိန်းပြည့် ldc);

void gemm_gpu(ကိန်းပြည့် TA, ကိန်းပြည့် TB, ကိန်းပြည့် M, ကိန်းပြည့် N, ကိန်းပြည့် K, float ALPHA, 
        float *A, ကိန်းပြည့် lda, 
        float *B, ကိန်းပြည့် ldb,
        float BETA,
        float *C, ကိန်းပြည့် ldc);
#endif
#endif
