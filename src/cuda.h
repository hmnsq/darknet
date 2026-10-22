#ifndef CUDA_H
#define CUDA_H

#অন্তর্ভাব "darknet.h"

#ifdef GPU

শূন্য check_error(cudaError_t status);
cublasHandle_t blas_handle();
পূর্ণসংখ্যা *cuda_make_int_array(পূর্ণসংখ্যা *x, size_t n);
শূন্য cuda_random(দশমিক *x_gpu, size_t n);
দশমিক cuda_compare(দশমিক *x_gpu, দশমিক *x, size_t n, অক্ষর *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
