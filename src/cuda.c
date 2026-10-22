पूर्णाङ्क gpu_index = 0;

#ifdef GPU

#अन्तर्भाव "cuda.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <time.h>

शून्य cuda_set_device(पूर्णाङ्क n)
{
    gpu_index = n;
    cudaError_t status = cudaSetDevice(n);
    check_error(status);
}

पूर्णाङ्क cuda_get_device()
{
    पूर्णाङ्क n = 0;
    cudaError_t status = cudaGetDevice(&n);
    check_error(status);
    फर्काऊ n;
}

शून्य check_error(cudaError_t status)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cudaGetLastError();
    यदि (status != cudaSuccess)
    {   
        अचर वर्ण *s = cudaGetErrorString(status);
        वर्ण buffer[256];
        म_लिखो("CUDA Error: %s\n", s);
        प्रतिज्ञा(0);
        snprintf(buffer, 256, "CUDA Error: %s", s);
        error(buffer);
    } 
    यदि (status2 != cudaSuccess)
    {   
        अचर वर्ण *s = cudaGetErrorString(status);
        वर्ण buffer[256];
        म_लिखो("CUDA Error Prev: %s\n", s);
        प्रतिज्ञा(0);
        snprintf(buffer, 256, "CUDA Error Prev: %s", s);
        error(buffer);
    } 
}

dim3 cuda_gridsize(माप_प्रकार n){
    माप_प्रकार k = (n-1) / BLOCK + 1;
    माप_प्रकार x = k;
    माप_प्रकार y = 1;
    यदि(x > 65535){
        x = उच्चमान(वर्ग_मूल(k));
        y = (n-1)/(x*BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    फर्काऊ d;
}

#ifdef CUDNN
cudnnHandle_t cudnn_handle()
{
    स्थिर पूर्णाङ्क init[16] = {0};
    स्थिर cudnnHandle_t handle[16];
    पूर्णाङ्क i = cuda_get_device();
    यदि(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    फर्काऊ handle[i];
}
#endif

cublasHandle_t blas_handle()
{
    स्थिर पूर्णाङ्क init[16] = {0};
    स्थिर cublasHandle_t handle[16];
    पूर्णाङ्क i = cuda_get_device();
    यदि(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    फर्काऊ handle[i];
}

भिन्न *cuda_make_array(भिन्न *x, माप_प्रकार n)
{
    भिन्न *x_gpu;
    माप_प्रकार size = परिमाण(भिन्न)*n;
    cudaError_t status = cudaMalloc((शून्य **)&x_gpu, size);
    check_error(status);
    यदि(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    } अन्यथा {
        fill_gpu(n, 0, x_gpu, 1);
    }
    यदि(!x_gpu) error("Cuda malloc failed\n");
    फर्काऊ x_gpu;
}

शून्य cuda_random(भिन्न *x_gpu, माप_प्रकार n)
{
    स्थिर curandGenerator_t gen[16];
    स्थिर पूर्णाङ्क init[16] = {0};
    पूर्णाङ्क i = cuda_get_device();
    यदि(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen[i], समय(0));
        init[i] = 1;
    }
    curandGenerateUniform(gen[i], x_gpu, n);
    check_error(cudaPeekAtLastError());
}

भिन्न cuda_compare(भिन्न *x_gpu, भिन्न *x, माप_प्रकार n, वर्ण *s)
{
    भिन्न *tmp = सुस्मृति(n, परिमाण(भिन्न));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    भिन्न err = dot_cpu(n, tmp, 1, tmp, 1);
    म_लिखो("Error %s: %f\n", s, वर्ग_मूल(err/n));
    मुक्त(tmp);
    फर्काऊ err;
}

पूर्णाङ्क *cuda_make_int_array(पूर्णाङ्क *x, माप_प्रकार n)
{
    पूर्णाङ्क *x_gpu;
    माप_प्रकार size = परिमाण(पूर्णाङ्क)*n;
    cudaError_t status = cudaMalloc((शून्य **)&x_gpu, size);
    check_error(status);
    यदि(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    }
    यदि(!x_gpu) error("Cuda malloc failed\n");
    फर्काऊ x_gpu;
}

शून्य cuda_free(भिन्न *x_gpu)
{
    cudaError_t status = cudaFree(x_gpu);
    check_error(status);
}

शून्य cuda_थप_array(भिन्न *x_gpu, भिन्न *x, माप_प्रकार n)
{
    माप_प्रकार size = परिमाण(भिन्न)*n;
    cudaError_t status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
    check_error(status);
}

शून्य cuda_pull_array(भिन्न *x_gpu, भिन्न *x, माप_प्रकार n)
{
    माप_प्रकार size = परिमाण(भिन्न)*n;
    cudaError_t status = cudaMemcpy(x, x_gpu, size, cudaMemcpyDeviceToHost);
    check_error(status);
}

भिन्न cuda_mag_array(भिन्न *x_gpu, माप_प्रकार n)
{
    भिन्न *temp = सुस्मृति(n, परिमाण(भिन्न));
    cuda_pull_array(x_gpu, temp, n);
    भिन्न m = mag_array(temp, n);
    मुक्त(temp);
    फर्काऊ m;
}
#अन्यथा
शून्य cuda_set_device(पूर्णाङ्क n){}

#endif
