#جے_بنا_تعریف CUDA_H
#تعریف_کرو CUDA_H

#ضمن "darknet.h"

#جے_تعریف_شدہ GPU

فراغ check_error(cudaError_t حالت);
cublasHandle_t blas_handle();
صحيح *cuda_make_int_array(صحيح *x, size_t n);
فراغ cuda_random(عائم *x_gpu, size_t n);
عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s);
dim3 cuda_gridsize(size_t n);

#جے_تعریف_شدہ CUDNN
cudnnHandle_t cudnn_handle();
#جے_انت

#جے_انت
#جے_انت
