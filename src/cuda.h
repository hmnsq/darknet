#جيڪڏهن_وصف_نه_ٿيل CUDA_H
#وصف_ڪريو CUDA_H

#ضمن "darknet.h"

#جيڪڏهن_وصف_ٿيل GPU

فراغ check_error(cudaError_t حالت);
cublasHandle_t blas_handle();
صحيح *cuda_make_int_array(صحيح *x, size_t n);
فراغ cuda_random(عائم *x_gpu, size_t n);
عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s);
dim3 cuda_gridsize(size_t n);

#جيڪڏهن_وصف_ٿيل CUDNN
cudnnHandle_t cudnn_handle();
#جيڪڏهن_پورو

#جيڪڏهن_پورو
#جيڪڏهن_پورو
