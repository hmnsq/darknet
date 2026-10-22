#اگر_تعریف_نشده GEMM_H
#تعریف_کن GEMM_H

فراغ gemm_bin(صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        حرف  *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم *C, صحيح ldc);
        
فراغ gemm(صحيح TA, صحيح TB, صحيح M, صحيح N, صحيح K, عائم ALPHA, 
                    عائم *A, صحيح lda, 
                    عائم *B, صحيح ldb,
                    عائم BETA,
                    عائم *C, صحيح ldc);

فراغ gemm_cpu(صحيح TA, صحيح TB, صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم BETA,
        عائم *C, صحيح ldc);

#اگر_تعریف_شده GPU
فراغ gemm_gpu(صحيح TA, صحيح TB, صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A_gpu, صحيح lda, 
        عائم *B_gpu, صحيح ldb,
        عائم BETA,
        عائم *C_gpu, صحيح ldc);

فراغ gemm_gpu(صحيح TA, صحيح TB, صحيح M, صحيح N, صحيح K, عائم ALPHA, 
        عائم *A, صحيح lda, 
        عائم *B, صحيح ldb,
        عائم BETA,
        عائم *C, صحيح ldc);
#پایان_اگر
#پایان_اگر
