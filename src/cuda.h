#ifndef CUDA_H
#define CUDA_H

#அந்தர்பாவ "darknet.h"

#ifdef GPU

வெறுமை check_error(cudaError_t status);
cublasHandle_t blas_handle();
முழு *cuda_make_int_array(முழு *x, size_t n);
வெறுமை cuda_random(தசம *x_gpu, size_t n);
தசம cuda_compare(தசம *x_gpu, தசம *x, size_t n, எழுத்து *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
