#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CUDA_H
#ವ್ಯಾಖ್ಯಾನಿಸು CUDA_H

#ಅನ್ತರ್ಭಾವ "darknet.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ check_error(cudaError_t ಸ್ಥಿತಿ_2);
cublasHandle_t blas_handle();
ಪೂರ್ಣಾಂಕ *cuda_make_int_array(ಪೂರ್ಣಾಂಕ *x, size_t n);
ಶೂನ್ಯ cuda_random(ದಶಮಾಂಶ *x_gpu, size_t n);
ದಶಮಾಂಶ cuda_compare(ದಶಮಾಂಶ *x_gpu, ದಶಮಾಂಶ *x, size_t n, ಅಕ್ಷರ *s);
dim3 cuda_gridsize(size_t n);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
cudnnHandle_t cudnn_handle();
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
