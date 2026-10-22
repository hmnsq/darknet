#ifndef CUDA_H
#define CUDA_H

#ضمن "darknet.h"

#ifdef GPU

فراغ check_error(cudaError_t status);
cublasHandle_t blas_handle();
صحيح *cuda_make_int_array(صحيح *x, size_t n);
فراغ cuda_random(عائم *x_gpu, size_t n);
عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
