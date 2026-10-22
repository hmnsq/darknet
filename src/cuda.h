#अगर_अघोषित CUDA_H
#घोषणा CUDA_H

#समावेश "darknet.h"

#अगर_घोषित GPU

व्योम check_error(cudaError_t status);
cublasHandle_t blas_handle();
पूर्णांक *cuda_make_int_array(पूर्णांक *x, माप_प्रकार n);
व्योम cuda_random(भग्न *x_gpu, माप_प्रकार n);
भग्न cuda_compare(भग्न *x_gpu, भग्न *x, माप_प्रकार n, अक्षर *s);
dim3 cuda_gridsize(माप_प्रकार n);

#अगर_घोषित CUDNN
cudnnHandle_t cudnn_handle();
#पूर्ण_अगर

#पूर्ण_अगर
#पूर्ण_अगर
