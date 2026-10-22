#ifndef CUDA_H
#define CUDA_H

#अन्तर्भाव "darknet.h"

#ifdef GPU

शून्य check_error(cudaError_t status);
cublasHandle_t blas_handle();
पूर्णाङ्क *cuda_make_int_array(पूर्णाङ्क *x, माप_प्रकार n);
शून्य cuda_random(भिन्न *x_gpu, माप_प्रकार n);
भिन्न cuda_compare(भिन्न *x_gpu, भिन्न *x, माप_प्रकार n, वर्ण *s);
dim3 cuda_gridsize(माप_प्रकार n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
