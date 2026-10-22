#اگر_غیر_تعریف_شدہ CUDA_H
#تعریف_کرو CUDA_H

#شامل "darknet.h"

#اگر_تعریف_شدہ GPU

خالی check_error(cudaError_t حالت_2);
cublasHandle_t blas_handle();
صحیح *cuda_make_int_array(صحیح *x, size_t n);
خالی cuda_random(اعشاری *x_gpu, size_t n);
اعشاری cuda_compare(اعشاری *x_gpu, اعشاری *x, size_t n, حرف *s);
dim3 cuda_gridsize(size_t n);

#اگر_تعریف_شدہ CUDNN
cudnnHandle_t cudnn_handle();
#اگر_ختم

#اگر_ختم
#اگر_ختم
