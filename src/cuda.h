#ifndef CUDA_H
#define CUDA_H

#अन्तर्भाव "darknet.h"

#ifdef GPU

शून्य check_error(cudaError_t status);
cublasHandle_t blas_handle();
पूर्णाङ्क *cuda_make_int_array(पूर्णाङ्क *x, size_t n);
शून्य cuda_random(भिन्न *x_gpu, size_t n);
भिन्न cuda_compare(भिन्न *x_gpu, भिन्न *x, size_t n, वर्ण *s);
dim3 cuda_gridsize(size_t n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
