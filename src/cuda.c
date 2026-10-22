صحيح gpu_index = 0;

#ifdef GPU

#ضمن "cuda.h"
#ضمن "utils.h"
#ضمن "blas.h"
#ضمن <assert.h>
#ضمن <stdlib.h>
#ضمن <time.h>

فراغ cuda_set_device(صحيح n)
{
    gpu_index = n;
    cudaError_t status = cudaSetDevice(n);
    check_error(status);
}

صحيح cuda_get_device()
{
    صحيح n = 0;
    cudaError_t status = cudaGetDevice(&n);
    check_error(status);
    أرجع n;
}

فراغ check_error(cudaError_t status)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cudaGetLastError();
    إذا (status != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(status);
        حرف buffer[256];
        printf("CUDA Error: %s\n", s);
        أكد(0);
        snprintf(buffer, 256, "CUDA Error: %s", s);
        error(buffer);
    } 
    إذا (status2 != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(status);
        حرف buffer[256];
        printf("CUDA Error Prev: %s\n", s);
        أكد(0);
        snprintf(buffer, 256, "CUDA Error Prev: %s", s);
        error(buffer);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / BLOCK + 1;
    size_t x = k;
    size_t y = 1;
    إذا(x > 65535){
        x = ceil(sqrt(k));
        y = (n-1)/(x*BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    أرجع d;
}

#ifdef CUDNN
cudnnHandle_t cudnn_handle()
{
    ساكن صحيح init[16] = {0};
    ساكن cudnnHandle_t handle[16];
    صحيح i = cuda_get_device();
    إذا(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    أرجع handle[i];
}
#endif

cublasHandle_t blas_handle()
{
    ساكن صحيح init[16] = {0};
    ساكن cublasHandle_t handle[16];
    صحيح i = cuda_get_device();
    إذا(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    أرجع handle[i];
}

عائم *cuda_make_array(عائم *x, size_t n)
{
    عائم *x_gpu;
    size_t size = حجم(عائم)*n;
    cudaError_t status = cudaMalloc((فراغ **)&x_gpu, size);
    check_error(status);
    إذا(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    } وإلا {
        fill_gpu(n, 0, x_gpu, 1);
    }
    إذا(!x_gpu) error("Cuda malloc failed\n");
    أرجع x_gpu;
}

فراغ cuda_random(عائم *x_gpu, size_t n)
{
    ساكن curandGenerator_t gen[16];
    ساكن صحيح init[16] = {0};
    صحيح i = cuda_get_device();
    إذا(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen[i], time(0));
        init[i] = 1;
    }
    curandGenerateUniform(gen[i], x_gpu, n);
    check_error(cudaPeekAtLastError());
}

عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s)
{
    عائم *tmp = calloc(n, حجم(عائم));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    عائم err = dot_cpu(n, tmp, 1, tmp, 1);
    printf("Error %s: %f\n", s, sqrt(err/n));
    حرر(tmp);
    أرجع err;
}

صحيح *cuda_make_int_array(صحيح *x, size_t n)
{
    صحيح *x_gpu;
    size_t size = حجم(صحيح)*n;
    cudaError_t status = cudaMalloc((فراغ **)&x_gpu, size);
    check_error(status);
    إذا(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    }
    إذا(!x_gpu) error("Cuda malloc failed\n");
    أرجع x_gpu;
}

فراغ cuda_free(عائم *x_gpu)
{
    cudaError_t status = cudaFree(x_gpu);
    check_error(status);
}

فراغ cuda_تور_array(عائم *x_gpu, عائم *x, size_t n)
{
    size_t size = حجم(عائم)*n;
    cudaError_t status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
    check_error(status);
}

فراغ cuda_pull_array(عائم *x_gpu, عائم *x, size_t n)
{
    size_t size = حجم(عائم)*n;
    cudaError_t status = cudaMemcpy(x, x_gpu, size, cudaMemcpyDeviceToHost);
    check_error(status);
}

عائم cuda_mag_array(عائم *x_gpu, size_t n)
{
    عائم *temp = calloc(n, حجم(عائم));
    cuda_pull_array(x_gpu, temp, n);
    عائم m = mag_array(temp, n);
    حرر(temp);
    أرجع m;
}
#وإلا
فراغ cuda_set_device(صحيح n){}

#endif
