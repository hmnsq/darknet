ਪੂਰਨ gpu_ਸੂਚਕਾਂਕ = 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

ਖਾਲੀ cuda_ਸੈੱਟ_device(ਪੂਰਨ n)
{
    gpu_ਸੂਚਕਾਂਕ = n;
    cudaError_t ਸਥਿਤੀ = cuda_ਸੈੱਟ_Device(n);
    check_error(ਸਥਿਤੀ);
}

ਪੂਰਨ cuda_get_device()
{
    ਪੂਰਨ n = 0;
    cudaError_t ਸਥਿਤੀ = cudaGetDevice(&n);
    check_error(ਸਥਿਤੀ);
    ਵਾਪਸ n;
}

ਖਾਲੀ check_error(cudaError_t ਸਥਿਤੀ)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cuda_Get_ਆਖਰੀ_Error();
    ਜੇ (ਸਥਿਤੀ != cudaSuccess)
    {   
        ਅਚਲ ਅੱਖਰ *s = cudaGetErrorString(ਸਥਿਤੀ);
        ਅੱਖਰ ਬਫਰ[256];
        ਲਿਖੋ("CUDA Error: %s\n", s);
        ਪ੍ਰਤਿਜ੍ਞਾ(0);
        snprintf(ਬਫਰ, 256, "CUDA Error: %s", s);
        error(ਬਫਰ);
    } 
    ਜੇ (status2 != cudaSuccess)
    {   
        ਅਚਲ ਅੱਖਰ *s = cudaGetErrorString(ਸਥਿਤੀ);
        ਅੱਖਰ ਬਫਰ[256];
        ਲਿਖੋ("CUDA Error Prev: %s\n", s);
        ਪ੍ਰਤਿਜ੍ਞਾ(0);
        snprintf(ਬਫਰ, 256, "CUDA Error Prev: %s", s);
        error(ਬਫਰ);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / ਖੰਡ + 1;
    size_t x = k;
    size_t y = 1;
    ਜੇ(x > 65535){
        x = ceil(ਵਰਗਮੂਲ(k));
        y = (n-1)/(x*ਖੰਡ) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    ਵਾਪਸ d;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
cudnnHandle_t cudnn_handle()
{
    ਸਥਿਰ ਪੂਰਨ init[16] = {0};
    ਸਥਿਰ cudnnHandle_t handle[16];
    ਪੂਰਨ i = cuda_get_device();
    ਜੇ(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    ਵਾਪਸ handle[i];
}
#ਜੇ_ਅੰਤ

cublasHandle_t blas_handle()
{
    ਸਥਿਰ ਪੂਰਨ init[16] = {0};
    ਸਥਿਰ cublasHandle_t handle[16];
    ਪੂਰਨ i = cuda_get_device();
    ਜੇ(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    ਵਾਪਸ handle[i];
}

ਦਸ਼ਮਲਵ *cuda_make_array(ਦਸ਼ਮਲਵ *x, size_t n)
{
    ਦਸ਼ਮਲਵ *x_gpu;
    size_t ਮਾਪ = ਆਕਾਰ(ਦਸ਼ਮਲਵ)*n;
    cudaError_t ਸਥਿਤੀ = cudaMalloc((ਖਾਲੀ **)&x_gpu, ਮਾਪ);
    check_error(ਸਥਿਤੀ);
    ਜੇ(x){
        ਸਥਿਤੀ = cudaMemcpy(x_gpu, x, ਮਾਪ, cuda_Memcpy_Host_ਮੰਜ਼ਿਲ_Device);
        check_error(ਸਥਿਤੀ);
    } ਨਹੀਂਤਾਂ {
        fill_gpu(n, 0, x_gpu, 1);
    }
    ਜੇ(!x_gpu) error("Cuda malloc failed\n");
    ਵਾਪਸ x_gpu;
}

ਖਾਲੀ cuda_random(ਦਸ਼ਮਲਵ *x_gpu, size_t n)
{
    ਸਥਿਰ curandGenerator_t gen[16];
    ਸਥਿਰ ਪੂਰਨ init[16] = {0};
    ਪੂਰਨ i = cuda_get_device();
    ਜੇ(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curand_ਸੈੱਟ_Pseudo_Random_Generator_ਬੀਜ(gen[i], ਸਮਾਂ(0));
        init[i] = 1;
    }
    curand_Generate_ਇਕਸਾਰ(gen[i], x_gpu, n);
    check_error(cuda_Peek_At_ਆਖਰੀ_Error());
}

ਦਸ਼ਮਲਵ cuda_compare(ਦਸ਼ਮਲਵ *x_gpu, ਦਸ਼ਮਲਵ *x, size_t n, ਅੱਖਰ *s)
{
    ਦਸ਼ਮਲਵ *tmp = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    ਦਸ਼ਮਲਵ err = dot_cpu(n, tmp, 1, tmp, 1);
    ਲਿਖੋ("Error %s: %f\n", s, ਵਰਗਮੂਲ(err/n));
    ਮੁਕਤ(tmp);
    ਵਾਪਸ err;
}

ਪੂਰਨ *cuda_make_int_array(ਪੂਰਨ *x, size_t n)
{
    ਪੂਰਨ *x_gpu;
    size_t ਮਾਪ = ਆਕਾਰ(ਪੂਰਨ)*n;
    cudaError_t ਸਥਿਤੀ = cudaMalloc((ਖਾਲੀ **)&x_gpu, ਮਾਪ);
    check_error(ਸਥਿਤੀ);
    ਜੇ(x){
        ਸਥਿਤੀ = cudaMemcpy(x_gpu, x, ਮਾਪ, cuda_Memcpy_Host_ਮੰਜ਼ਿਲ_Device);
        check_error(ਸਥਿਤੀ);
    }
    ਜੇ(!x_gpu) error("Cuda malloc failed\n");
    ਵਾਪਸ x_gpu;
}

ਖਾਲੀ cuda_free(ਦਸ਼ਮਲਵ *x_gpu)
{
    cudaError_t ਸਥਿਤੀ = cudaFree(x_gpu);
    check_error(ਸਥਿਤੀ);
}

ਖਾਲੀ cuda_ਧੱਕੋ_array(ਦਸ਼ਮਲਵ *x_gpu, ਦਸ਼ਮਲਵ *x, size_t n)
{
    size_t ਮਾਪ = ਆਕਾਰ(ਦਸ਼ਮਲਵ)*n;
    cudaError_t ਸਥਿਤੀ = cudaMemcpy(x_gpu, x, ਮਾਪ, cuda_Memcpy_Host_ਮੰਜ਼ਿਲ_Device);
    check_error(ਸਥਿਤੀ);
}

ਖਾਲੀ cuda_pull_array(ਦਸ਼ਮਲਵ *x_gpu, ਦਸ਼ਮਲਵ *x, size_t n)
{
    size_t ਮਾਪ = ਆਕਾਰ(ਦਸ਼ਮਲਵ)*n;
    cudaError_t ਸਥਿਤੀ = cudaMemcpy(x, x_gpu, ਮਾਪ, cuda_Memcpy_Device_ਮੰਜ਼ਿਲ_Host);
    check_error(ਸਥਿਤੀ);
}

ਦਸ਼ਮਲਵ cuda_mag_array(ਦਸ਼ਮਲਵ *x_gpu, size_t n)
{
    ਦਸ਼ਮਲਵ *ਆਰਜ਼ੀ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    cuda_pull_array(x_gpu, ਆਰਜ਼ੀ, n);
    ਦਸ਼ਮਲਵ m = mag_array(ਆਰਜ਼ੀ, n);
    ਮੁਕਤ(ਆਰਜ਼ੀ);
    ਵਾਪਸ m;
}
#ਨਹੀਂਤਾਂ
ਖਾਲੀ cuda_ਸੈੱਟ_device(ਪੂਰਨ n){}

#ਜੇ_ਅੰਤ
