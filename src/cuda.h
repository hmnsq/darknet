#ifndef CUDA_H
#define CUDA_H

#അന്തര്ഭാവ "darknet.h"

#ifdef GPU

ശൂന്യം check_error(cudaError_t status);
cublasHandle_t blas_handle();
പൂർണ്ണസംഖ്യ *cuda_make_int_array(പൂർണ്ണസംഖ്യ *x, size_t n);
ശൂന്യം cuda_random(ദശാംശം *x_gpu, size_t n);
ദശാംശം cuda_compare(ദശാംശം *x_gpu, ദശാംശം *x, size_t n, അക്ഷരം *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
