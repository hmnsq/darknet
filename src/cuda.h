#که_نه_تعریف_شوی CUDA_H
#تعریف_کړه CUDA_H

#ضمن "darknet.h"

#که_تعریف_شوی GPU

فراغ check_error(cudaError_t حالت);
cublasHandle_t blas_handle();
صحيح *cuda_make_int_array(صحيح *x, size_t n);
فراغ cuda_random(عائم *x_gpu, size_t n);
عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s);
dim3 cuda_gridsize(size_t n);

#که_تعریف_شوی CUDNN
cudnnHandle_t cudnn_handle();
#د_که_پای

#د_که_پای
#د_که_پای
