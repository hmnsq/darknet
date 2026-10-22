#ifndef CUDA_H
#define CUDA_H

#અન્તર્ભાવ "darknet.h"

#ifdef GPU

શૂન્ય check_error(cudaError_t status);
cublasHandle_t blas_handle();
પૂર્ણાંક *cuda_make_int_array(પૂર્ણાંક *x, size_t n);
શૂન્ય cuda_random(દશાંશ *x_gpu, size_t n);
દશાંશ cuda_compare(દશાંશ *x_gpu, દશાંશ *x, size_t n, અક્ષર *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
