#ifndef GEMM_H
#define GEMM_H

void gemm_bin(ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        var_na  *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float *C, ຈຳນວນເຕັມ ldc);
        
void gemm(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
                    float *A, ຈຳນວນເຕັມ lda, 
                    float *B, ຈຳນວນເຕັມ ldb,
                    float BETA,
                    float *C, ຈຳນວນເຕັມ ldc);

void gemm_cpu(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float BETA,
        float *C, ຈຳນວນເຕັມ ldc);

#ifdef GPU
void gemm_gpu(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A_gpu, ຈຳນວນເຕັມ lda, 
        float *B_gpu, ຈຳນວນເຕັມ ldb,
        float BETA,
        float *C_gpu, ຈຳນວນເຕັມ ldc);

void gemm_gpu(ຈຳນວນເຕັມ TA, ຈຳນວນເຕັມ TB, ຈຳນວນເຕັມ M, ຈຳນວນເຕັມ N, ຈຳນວນເຕັມ K, float ALPHA, 
        float *A, ຈຳນວນເຕັມ lda, 
        float *B, ຈຳນວນເຕັມ ldb,
        float BETA,
        float *C, ຈຳນວນເຕັມ ldc);
#endif
#endif
