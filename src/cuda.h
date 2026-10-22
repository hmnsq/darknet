#ifndef CUDA_H
#define CUDA_H

#شامل "darknet.h"

#ifdef GPU

تهی check_error(cudaError_t status);
cublasHandle_t blas_handle();
صحیح *cuda_make_int_array(صحیح *x, size_t n);
تهی cuda_random(اعشاری *x_gpu, size_t n);
اعشاری cuda_compare(اعشاری *x_gpu, اعشاری *x, size_t n, نویسه *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
