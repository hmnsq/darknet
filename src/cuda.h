#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CUDA_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CUDA_H

#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ check_error(cudaError_t ਸਥਿਤੀ);
cublasHandle_t blas_handle();
ਪੂਰਨ *cuda_make_int_array(ਪੂਰਨ *x, size_t n);
ਖਾਲੀ cuda_random(ਦਸ਼ਮਲਵ *x_gpu, size_t n);
ਦਸ਼ਮਲਵ cuda_compare(ਦਸ਼ਮਲਵ *x_gpu, ਦਸ਼ਮਲਵ *x, size_t n, ਅੱਖਰ *s);
dim3 cuda_gridsize(size_t n);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
cudnnHandle_t cudnn_handle();
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
