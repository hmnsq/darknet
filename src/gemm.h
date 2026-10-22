#ifndef GEMM_H
#define GEMM_H

void gemm_bin(puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        var_na  *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float *C, puur_naa_nka ldc);
        
void gemm(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
                    float *A, puur_naa_nka lda, 
                    float *B, puur_naa_nka ldb,
                    float BETA,
                    float *C, puur_naa_nka ldc);

void gemm_cpu(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float BETA,
        float *C, puur_naa_nka ldc);

#ifdef GPU
void gemm_gpu(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A_gpu, puur_naa_nka lda, 
        float *B_gpu, puur_naa_nka ldb,
        float BETA,
        float *C_gpu, puur_naa_nka ldc);

void gemm_gpu(puur_naa_nka TA, puur_naa_nka TB, puur_naa_nka M, puur_naa_nka N, puur_naa_nka K, float ALPHA, 
        float *A, puur_naa_nka lda, 
        float *B, puur_naa_nka ldb,
        float BETA,
        float *C, puur_naa_nka ldc);
#endif
#endif
