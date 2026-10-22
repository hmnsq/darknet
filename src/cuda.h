#ifndef CUDA_H
#define CUDA_H

#अन्तर्भाव "darknet.h"

#ifdef GPU

शून्य check_error(cudaError_t status);
cublasHandle_t blas_handle();
पूर्णांक *cuda_make_int_array(पूर्णांक *x, माप_प्रकार n);
शून्य cuda_random(दशांश *x_gpu, माप_प्रकार n);
दशांश cuda_compare(दशांश *x_gpu, दशांश *x, माप_प्रकार n, अक्षर *s);
dim3 cuda_gridsize(माप_प्रकार n);

#ifdef CUDNN
cudnnHandle_t cudnn_handle();
#endif

#endif
#endif
