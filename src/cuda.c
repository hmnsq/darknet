صحيح gpu_شاخص = 0;

#اگر_تعریف_شده GPU

#ضمن "cuda.h"
#ضمن "utils.h"
#ضمن "blas.h"
#ضمن <assert.h>
#ضمن <stdlib.h>
#ضمن <time.h>

فراغ cuda_مجموعه_device(صحيح n)
{
    gpu_شاخص = n;
    cudaError_t وضعیت = cuda_مجموعه_Device(n);
    check_error(وضعیت);
}

صحيح cuda_get_device()
{
    صحيح n = 0;
    cudaError_t وضعیت = cudaGetDevice(&n);
    check_error(وضعیت);
    أرجع n;
}

فراغ check_error(cudaError_t وضعیت)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cuda_Get_آخرین_Error();
    إذا (وضعیت != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(وضعیت);
        حرف میانگیر[256];
        چاپ_کن("CUDA Error: %s\n", s);
        أكد(0);
        snprintf(میانگیر, 256, "CUDA Error: %s", s);
        error(میانگیر);
    } 
    إذا (status2 != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(وضعیت);
        حرف میانگیر[256];
        چاپ_کن("CUDA Error Prev: %s\n", s);
        أكد(0);
        snprintf(میانگیر, 256, "CUDA Error Prev: %s", s);
        error(میانگیر);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / بلوک + 1;
    size_t x = k;
    size_t y = 1;
    إذا(x > 65535){
        x = ceil(جذر(k));
        y = (n-1)/(x*بلوک) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    أرجع d;
}

#اگر_تعریف_شده CUDNN
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
#پایان_اگر

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
    size_t اندازه = حجم(عائم)*n;
    cudaError_t وضعیت = cudaMalloc((فراغ **)&x_gpu, اندازه);
    check_error(وضعیت);
    إذا(x){
        وضعیت = cudaMemcpy(x_gpu, x, اندازه, cuda_Memcpy_Host_مقصد_Device);
        check_error(وضعیت);
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
        curand_مجموعه_Pseudo_Random_Generator_دانه(gen[i], زمان(0));
        init[i] = 1;
    }
    curand_Generate_یکنواخت(gen[i], x_gpu, n);
    check_error(cuda_Peek_At_آخرین_Error());
}

عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s)
{
    عائم *tmp = حافظه_صفر_بده(n, حجم(عائم));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    عائم err = dot_cpu(n, tmp, 1, tmp, 1);
    چاپ_کن("Error %s: %f\n", s, جذر(err/n));
    حرر(tmp);
    أرجع err;
}

صحيح *cuda_make_int_array(صحيح *x, size_t n)
{
    صحيح *x_gpu;
    size_t اندازه = حجم(صحيح)*n;
    cudaError_t وضعیت = cudaMalloc((فراغ **)&x_gpu, اندازه);
    check_error(وضعیت);
    إذا(x){
        وضعیت = cudaMemcpy(x_gpu, x, اندازه, cuda_Memcpy_Host_مقصد_Device);
        check_error(وضعیت);
    }
    إذا(!x_gpu) error("Cuda malloc failed\n");
    أرجع x_gpu;
}

فراغ cuda_free(عائم *x_gpu)
{
    cudaError_t وضعیت = cudaFree(x_gpu);
    check_error(وضعیت);
}

فراغ cuda_افزودن_array(عائم *x_gpu, عائم *x, size_t n)
{
    size_t اندازه = حجم(عائم)*n;
    cudaError_t وضعیت = cudaMemcpy(x_gpu, x, اندازه, cuda_Memcpy_Host_مقصد_Device);
    check_error(وضعیت);
}

فراغ cuda_pull_array(عائم *x_gpu, عائم *x, size_t n)
{
    size_t اندازه = حجم(عائم)*n;
    cudaError_t وضعیت = cudaMemcpy(x, x_gpu, اندازه, cuda_Memcpy_Device_مقصد_Host);
    check_error(وضعیت);
}

عائم cuda_mag_array(عائم *x_gpu, size_t n)
{
    عائم *موقت = حافظه_صفر_بده(n, حجم(عائم));
    cuda_pull_array(x_gpu, موقت, n);
    عائم m = mag_array(موقت, n);
    حرر(موقت);
    أرجع m;
}
#وإلا
فراغ cuda_مجموعه_device(صحيح n){}

#پایان_اگر
