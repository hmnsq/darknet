पूर्णांक gpu_index = 0;

#ifdef GPU

#अन्तर्भाव "cuda.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव <assert.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <time.h>

शून्य cuda_set_device(पूर्णांक n)
{
    gpu_index = n;
    cudaError_t status = cudaSetDevice(n);
    check_error(status);
}

पूर्णांक cuda_get_device()
{
    पूर्णांक n = 0;
    cudaError_t status = cudaGetDevice(&n);
    check_error(status);
    परत n;
}

शून्य check_error(cudaError_t status)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cudaGetLastError();
    जर (status != cudaSuccess)
    {   
        स्थिरांक अक्षर *s = cudaGetErrorString(status);
        अक्षर buffer[256];
        म_लिखो("CUDA Error: %s\n", s);
        प्रतिज्ञा(0);
        snprintf(buffer, 256, "CUDA Error: %s", s);
        error(buffer);
    } 
    जर (status2 != cudaSuccess)
    {   
        स्थिरांक अक्षर *s = cudaGetErrorString(status);
        अक्षर buffer[256];
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
    जर(x > 65535){
        x = उच्चमान(वर्ग_मूल(k));
        y = (n-1)/(x*BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    परत d;
}

#ifdef CUDNN
cudnnHandle_t cudnn_handle()
{
    स्थिर पूर्णांक init[16] = {0};
    स्थिर cudnnHandle_t handle[16];
    पूर्णांक i = cuda_get_device();
    जर(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    परत handle[i];
}
#endif

cublasHandle_t blas_handle()
{
    स्थिर पूर्णांक init[16] = {0};
    स्थिर cublasHandle_t handle[16];
    पूर्णांक i = cuda_get_device();
    जर(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    परत handle[i];
}

दशांश *cuda_make_array(दशांश *x, माप_प्रकार n)
{
    दशांश *x_gpu;
    माप_प्रकार size = आकार(दशांश)*n;
    cudaError_t status = cudaMalloc((शून्य **)&x_gpu, size);
    check_error(status);
    जर(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    } अन्यथा {
        fill_gpu(n, 0, x_gpu, 1);
    }
    जर(!x_gpu) error("Cuda malloc failed\n");
    परत x_gpu;
}

शून्य cuda_random(दशांश *x_gpu, माप_प्रकार n)
{
    स्थिर curandGenerator_t gen[16];
    स्थिर पूर्णांक init[16] = {0};
    पूर्णांक i = cuda_get_device();
    जर(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen[i], समय(0));
        init[i] = 1;
    }
    curandGenerateUniform(gen[i], x_gpu, n);
    check_error(cudaPeekAtLastError());
}

दशांश cuda_compare(दशांश *x_gpu, दशांश *x, माप_प्रकार n, अक्षर *s)
{
    दशांश *tmp = सुस्मृति(n, आकार(दशांश));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    दशांश err = dot_cpu(n, tmp, 1, tmp, 1);
    म_लिखो("Error %s: %f\n", s, वर्ग_मूल(err/n));
    मुक्त(tmp);
    परत err;
}

पूर्णांक *cuda_make_int_array(पूर्णांक *x, माप_प्रकार n)
{
    पूर्णांक *x_gpu;
    माप_प्रकार size = आकार(पूर्णांक)*n;
    cudaError_t status = cudaMalloc((शून्य **)&x_gpu, size);
    check_error(status);
    जर(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    }
    जर(!x_gpu) error("Cuda malloc failed\n");
    परत x_gpu;
}

शून्य cuda_free(दशांश *x_gpu)
{
    cudaError_t status = cudaFree(x_gpu);
    check_error(status);
}

शून्य cuda_ढकला_array(दशांश *x_gpu, दशांश *x, माप_प्रकार n)
{
    माप_प्रकार size = आकार(दशांश)*n;
    cudaError_t status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
    check_error(status);
}

शून्य cuda_pull_array(दशांश *x_gpu, दशांश *x, माप_प्रकार n)
{
    माप_प्रकार size = आकार(दशांश)*n;
    cudaError_t status = cudaMemcpy(x, x_gpu, size, cudaMemcpyDeviceToHost);
    check_error(status);
}

दशांश cuda_mag_array(दशांश *x_gpu, माप_प्रकार n)
{
    दशांश *temp = सुस्मृति(n, आकार(दशांश));
    cuda_pull_array(x_gpu, temp, n);
    दशांश m = mag_array(temp, n);
    मुक्त(temp);
    परत m;
}
#अन्यथा
शून्य cuda_set_device(पूर्णांक n){}

#endif
