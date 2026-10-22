#ifndef CUDA_H
#define CUDA_H

#ಅನ್ತರ್ಭಾವ "darknet.h"

#ifdef GPU

ಶೂನ್ಯ check_error(cudaError_t status);
cublasHandle_t blas_handle();
ಪೂರ್ಣಾಙ್ಕ *cuda_make_int_array(ಪೂರ್ಣಾಙ್ಕ *x, size_t n);
ಶೂನ್ಯ cuda_random(ಭಿನ್ನ *x_gpu, size_t n);
ಭಿನ್ನ cuda_compare(ಭಿನ್ನ *x_gpu, ಭಿನ್ನ *x, size_t n, ವರ್ಣ *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
