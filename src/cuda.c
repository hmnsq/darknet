صحیح gpu_اشاریہ = 0;

#اگر_تعریف_شدہ GPU

#شامل "cuda.h"
#شامل "utils.h"
#شامل "blas.h"
#شامل <assert.h>
#شامل <stdlib.h>
#شامل <time.h>

خالی cuda_سیٹ_device(صحیح n)
{
    gpu_اشاریہ = n;
    cudaError_t حالت_2 = cuda_سیٹ_Device(n);
    check_error(حالت_2);
}

صحیح cuda_get_device()
{
    صحیح n = 0;
    cudaError_t حالت_2 = cudaGetDevice(&n);
    check_error(حالت_2);
    واپس n;
}

خالی check_error(cudaError_t حالت_2)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cuda_Get_آخری_Error();
    اگر (حالت_2 != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(حالت_2);
        حرف بفر[256];
        لکھو("CUDA Error: %s\n", s);
        أكد(0);
        snprintf(بفر, 256, "CUDA Error: %s", s);
        error(بفر);
    } 
    اگر (status2 != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(حالت_2);
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
    اگر(x > 65535){
        x = ceil(جذر(k));
        y = (n-1)/(x*بلاک) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    واپس d;
}

#اگر_تعریف_شدہ CUDNN
cudnnHandle_t cudnn_handle()
{
    ساکن صحیح init[16] = {0};
    ساکن cudnnHandle_t handle[16];
    صحیح i = cuda_get_device();
    اگر(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    واپس handle[i];
}
#اگر_ختم

cublasHandle_t blas_handle()
{
    ساکن صحیح init[16] = {0};
    ساکن cublasHandle_t handle[16];
    صحیح i = cuda_get_device();
    اگر(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    واپس handle[i];
}

اعشاری *cuda_make_array(اعشاری *x, size_t n)
{
    اعشاری *x_gpu;
    size_t سائز = حجم(اعشاری)*n;
    cudaError_t حالت_2 = cudaMalloc((خالی **)&x_gpu, سائز);
    check_error(حالت_2);
    اگر(x){
        حالت_2 = cudaMemcpy(x_gpu, x, سائز, cuda_Memcpy_Host_مقصد_Device);
        check_error(حالت_2);
    } ورنہ {
        fill_gpu(n, 0, x_gpu, 1);
    }
    اگر(!x_gpu) error("Cuda malloc failed\n");
    واپس x_gpu;
}

خالی cuda_random(اعشاری *x_gpu, size_t n)
{
    ساکن curandGenerator_t gen[16];
    ساکن صحیح init[16] = {0};
    صحیح i = cuda_get_device();
    اگر(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curand_سیٹ_Pseudo_Random_Generator_بیج(gen[i], وقت(0));
        init[i] = 1;
    }
    curand_Generate_یکساں(gen[i], x_gpu, n);
    check_error(cuda_Peek_At_آخری_Error());
}

اعشاری cuda_compare(اعشاری *x_gpu, اعشاری *x, size_t n, حرف *s)
{
    اعشاری *tmp = صفر_حافظہ_دو(n, حجم(اعشاری));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    اعشاری err = dot_cpu(n, tmp, 1, tmp, 1);
    لکھو("Error %s: %f\n", s, جذر(err/n));
    آزاد(tmp);
    واپس err;
}

صحیح *cuda_make_int_array(صحیح *x, size_t n)
{
    صحیح *x_gpu;
    size_t سائز = حجم(صحیح)*n;
    cudaError_t حالت_2 = cudaMalloc((خالی **)&x_gpu, سائز);
    check_error(حالت_2);
    اگر(x){
        حالت_2 = cudaMemcpy(x_gpu, x, سائز, cuda_Memcpy_Host_مقصد_Device);
        check_error(حالت_2);
    }
    اگر(!x_gpu) error("Cuda malloc failed\n");
    واپس x_gpu;
}

خالی cuda_free(اعشاری *x_gpu)
{
    cudaError_t حالت_2 = cudaFree(x_gpu);
    check_error(حالت_2);
}

خالی cuda_ڈالو_array(اعشاری *x_gpu, اعشاری *x, size_t n)
{
    size_t سائز = حجم(اعشاری)*n;
    cudaError_t حالت_2 = cudaMemcpy(x_gpu, x, سائز, cuda_Memcpy_Host_مقصد_Device);
    check_error(حالت_2);
}

خالی cuda_pull_array(اعشاری *x_gpu, اعشاری *x, size_t n)
{
    size_t سائز = حجم(اعشاری)*n;
    cudaError_t حالت_2 = cudaMemcpy(x, x_gpu, سائز, cuda_Memcpy_Device_مقصد_Host);
    check_error(حالت_2);
}

اعشاری cuda_mag_array(اعشاری *x_gpu, size_t n)
{
    اعشاری *عارضی = صفر_حافظہ_دو(n, حجم(اعشاری));
    cuda_pull_array(x_gpu, عارضی, n);
    اعشاری m = mag_array(عارضی, n);
    آزاد(عارضی);
    واپس m;
}
#ورنہ
خالی cuda_سیٹ_device(صحیح n){}

#اگر_ختم
