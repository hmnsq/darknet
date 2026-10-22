#ifndef GEMM_H
#define GEMM_H

تهی gemm_bin(صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        نویسه  *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری *C, صحیح ldc);
        
تهی gemm(صحیح TA, صحیح TB, صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
                    اعشاری *A, صحیح lda, 
                    اعشاری *B, صحیح ldb,
                    اعشاری BETA,
                    اعشاری *C, صحیح ldc);

تهی gemm_cpu(صحیح TA, صحیح TB, صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری BETA,
        اعشاری *C, صحیح ldc);

#ifdef GPU
تهی gemm_gpu(صحیح TA, صحیح TB, صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A_gpu, صحیح lda, 
        اعشاری *B_gpu, صحیح ldb,
        اعشاری BETA,
        اعشاری *C_gpu, صحیح ldc);

تهی gemm_gpu(صحیح TA, صحیح TB, صحیح M, صحیح N, صحیح K, اعشاری ALPHA, 
        اعشاری *A, صحیح lda, 
        اعشاری *B, صحیح ldb,
        اعشاری BETA,
        اعشاری *C, صحیح ldc);
#endif
#endif
