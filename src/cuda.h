#यदि_अपरिभाषितम् CUDA_H
#परिभाषय CUDA_H

#अन्तर्भाव "darknet.h"

#यदि_परिभाषितम् GPU

शून्य check_error(cudaError_t स्थितिः);
cublasHandle_t blas_handle();
पूर्णाङ्क *cuda_make_int_array(पूर्णाङ्क *x, size_t n);
शून्य cuda_random(भिन्न *x_gpu, size_t n);
भिन्न cuda_compare(भिन्न *x_gpu, भिन्न *x, size_t n, वर्ण *s);
dim3 cuda_gridsize(size_t n);

#यदि_परिभाषितम् CUDNN
cudnnHandle_t cudnn_handle();
#यदि_अन्तः

#यदि_अन्तः
#यदि_अन्तः
