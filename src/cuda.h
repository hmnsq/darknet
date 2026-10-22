#ifndef CUDA_H
#define CUDA_H

#ଅନ୍ତର୍ଭାଵ "darknet.h"

#ifdef GPU

ଶୂନ୍ୟ check_error(cudaError_t status);
cublasHandle_t blas_handle();
ପୂର୍ଣ୍ଣସଂଖ୍ୟା *cuda_make_int_array(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *x, size_t n);
ଶୂନ୍ୟ cuda_random(ଦଶମିକ *x_gpu, size_t n);
ଦଶମିକ cuda_compare(ଦଶମିକ *x_gpu, ଦଶମିକ *x, size_t n, ଅକ୍ଷର *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
