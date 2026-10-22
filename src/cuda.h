#اگر_تعریف_نشده CUDA_H
#تعریف_کن CUDA_H

#ضمن "darknet.h"

#اگر_تعریف_شده GPU

فراغ check_error(cudaError_t وضعیت);
cublasHandle_t blas_handle();
صحيح *cuda_make_int_array(صحيح *x, size_t n);
فراغ cuda_random(عائم *x_gpu, size_t n);
عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s);
dim3 cuda_gridsize(size_t n);

#اگر_تعریف_شده CUDNN
cudnnHandle_t cudnn_handle();
#پایان_اگر

#پایان_اگر
#پایان_اگر
