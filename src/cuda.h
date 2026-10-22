#నిర్వచితం_కాకపోతే CUDA_H
#నిర్వచించు CUDA_H

#అన్తర్భావ "darknet.h"

#నిర్వచితమైతే GPU

శూన్య check_error(cudaError_t స్థితి_2);
cublasHandle_t blas_handle();
పూర్ణాంకం *cuda_make_int_array(పూర్ణాంకం *x, size_t n);
శూన్య cuda_random(దశాంశం *x_gpu, size_t n);
దశాంశం cuda_compare(దశాంశం *x_gpu, దశాంశం *x, size_t n, అక్షరం *s);
dim3 cuda_gridsize(size_t n);

#నిర్వచితమైతే CUDNN
cudnnHandle_t cudnn_handle();
#అయితే_ముగింపు

#అయితే_ముగింపు
#అయితే_ముగింపు
