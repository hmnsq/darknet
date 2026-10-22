صحيح gpu_اشاریہ = 0;

#جے_تعریف_شدہ GPU

#ضمن "cuda.h"
#ضمن "utils.h"
#ضمن "blas.h"
#ضمن <assert.h>
#ضمن <stdlib.h>
#ضمن <time.h>

فراغ cuda_سیٹ_device(صحيح n)
{
    gpu_اشاریہ = n;
    cudaError_t حالت = cuda_سیٹ_Device(n);
    check_error(حالت);
}

صحيح cuda_get_device()
{
    صحيح n = 0;
    cudaError_t حالت = cudaGetDevice(&n);
    check_error(حالت);
    أرجع n;
}

فراغ check_error(cudaError_t حالت)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cuda_Get_آخری_Error();
    إذا (حالت != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(حالت);
        حرف بفر[256];
        لکھو("CUDA Error: %s\n", s);
        أكد(0);
        snprintf(بفر, 256, "CUDA Error: %s", s);
        error(بفر);
    } 
    إذا (status2 != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(حالت);
        حرف بفر[256];
        لکھو("CUDA Error Prev: %s\n", s);
        أكد(0);
        snprintf(بفر, 256, "CUDA Error Prev: %s", s);
        error(بفر);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / بلاک + 1;
    size_t x = k;
    size_t y = 1;
    إذا(x > 65535){
        x = ceil(جذر(k));
        y = (n-1)/(x*بلاک) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    أرجع d;
}

#جے_تعریف_شدہ CUDNN
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
#جے_انت

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
    size_t ناپ = حجم(عائم)*n;
    cudaError_t حالت = cudaMalloc((فراغ **)&x_gpu, ناپ);
    check_error(حالت);
    إذا(x){
        حالت = cudaMemcpy(x_gpu, x, ناپ, cuda_Memcpy_Host_منزل_Device);
        check_error(حالت);
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
        curand_سیٹ_Pseudo_Random_Generator_بیج(gen[i], ویلا(0));
        init[i] = 1;
    }
    curand_Generate_اکسار(gen[i], x_gpu, n);
    check_error(cuda_Peek_At_آخری_Error());
}

عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s)
{
    عائم *tmp = صفر_حافظہ_دیو(n, حجم(عائم));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    عائم err = dot_cpu(n, tmp, 1, tmp, 1);
    لکھو("Error %s: %f\n", s, جذر(err/n));
    حرر(tmp);
    أرجع err;
}

صحيح *cuda_make_int_array(صحيح *x, size_t n)
{
    صحيح *x_gpu;
    size_t ناپ = حجم(صحيح)*n;
    cudaError_t حالت = cudaMalloc((فراغ **)&x_gpu, ناپ);
    check_error(حالت);
    إذا(x){
        حالت = cudaMemcpy(x_gpu, x, ناپ, cuda_Memcpy_Host_منزل_Device);
        check_error(حالت);
    }
    إذا(!x_gpu) error("Cuda malloc failed\n");
    أرجع x_gpu;
}

فراغ cuda_free(عائم *x_gpu)
{
    cudaError_t حالت = cudaFree(x_gpu);
    check_error(حالت);
}

فراغ cuda_ڈالو_array(عائم *x_gpu, عائم *x, size_t n)
{
    size_t ناپ = حجم(عائم)*n;
    cudaError_t حالت = cudaMemcpy(x_gpu, x, ناپ, cuda_Memcpy_Host_منزل_Device);
    check_error(حالت);
}

فراغ cuda_pull_array(عائم *x_gpu, عائم *x, size_t n)
{
    size_t ناپ = حجم(عائم)*n;
    cudaError_t حالت = cudaMemcpy(x, x_gpu, ناپ, cuda_Memcpy_Device_منزل_Host);
    check_error(حالت);
}

عائم cuda_mag_array(عائم *x_gpu, size_t n)
{
    عائم *عارضی = صفر_حافظہ_دیو(n, حجم(عائم));
    cuda_pull_array(x_gpu, عارضی, n);
    عائم m = mag_array(عارضی, n);
    حرر(عارضی);
    أرجع m;
}
#وإلا
فراغ cuda_سیٹ_device(صحيح n){}

#جے_انت
