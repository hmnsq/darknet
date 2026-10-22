#ifndef GEMM_H
#define GEMM_H

வெறுமை gemm_bin(முழு M, முழு N, முழு K, தசம ALPHA, 
        எழுத்து  *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம *C, முழு ldc);
        
வெறுமை gemm(முழு TA, முழு TB, முழு M, முழு N, முழு K, தசம ALPHA, 
                    தசம *A, முழு lda, 
                    தசம *B, முழு ldb,
                    தசம BETA,
                    தசம *C, முழு ldc);

வெறுமை gemm_cpu(முழு TA, முழு TB, முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம BETA,
        தசம *C, முழு ldc);

#ifdef GPU
வெறுமை gemm_gpu(முழு TA, முழு TB, முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A_gpu, முழு lda, 
        தசம *B_gpu, முழு ldb,
        தசம BETA,
        தசம *C_gpu, முழு ldc);

வெறுமை gemm_gpu(முழு TA, முழு TB, முழு M, முழு N, முழு K, தசம ALPHA, 
        தசம *A, முழு lda, 
        தசம *B, முழு ldb,
        தசம BETA,
        தசம *C, முழு ldc);
#endif
#endif
