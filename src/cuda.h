#ifndef CUDA_H
#define CUDA_H

#অন্তর্ভাৱ "darknet.h"

#ifdef GPU

শূন্য check_error(cudaError_t status);
cublasHandle_t blas_handle();
পূর্ণাঙ্ক *cuda_make_int_array(পূর্ণাঙ্ক *x, size_t n);
শূন্য cuda_random(ভিন্ন *x_gpu, size_t n);
ভিন্ন cuda_compare(ভিন্ন *x_gpu, ভিন্ন *x, size_t n, ৱর্ণ *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
