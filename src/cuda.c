صحيح gpu_اشاريو = 0;

#جيڪڏهن_وصف_ٿيل GPU

#ضمن "cuda.h"
#ضمن "utils.h"
#ضمن "blas.h"
#ضمن <assert.h>
#ضمن <stdlib.h>
#ضمن <time.h>

فراغ cuda_سيٽ_device(صحيح n)
{
    gpu_اشاريو = n;
    cudaError_t حالت = cuda_سيٽ_Device(n);
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
    cudaError_t status2 = cuda_Get_آخري_Error();
    إذا (حالت != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(حالت);
        حرف بفر[256];
        لکو("CUDA Error: %s\n", s);
        أكد(0);
        snprintf(بفر, 256, "CUDA Error: %s", s);
        error(بفر);
    } 
    إذا (status2 != cudaSuccess)
    {   
        ثابت حرف *s = cudaGetErrorString(حالت);
        حرف بفر[256];
        لکو("CUDA Error Prev: %s\n", s);
        أكد(0);
        snprintf(بفر, 256, "CUDA Error Prev: %s", s);
        error(بفر);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / بلاڪ + 1;
    size_t x = k;
    size_t y = 1;
    إذا(x > 65535){
        x = ceil(جذر(k));
        y = (n-1)/(x*بلاڪ) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    أرجع d;
}

#جيڪڏهن_وصف_ٿيل CUDNN
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
#جيڪڏهن_پورو

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
    size_t سائيز = حجم(عائم)*n;
    cudaError_t حالت = cudaMalloc((فراغ **)&x_gpu, سائيز);
    check_error(حالت);
    إذا(x){
        حالت = cudaMemcpy(x_gpu, x, سائيز, cuda_Memcpy_Host_منزل_Device);
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
        curand_سيٽ_Pseudo_Random_Generator_ٻج(gen[i], وقت(0));
        init[i] = 1;
    }
    curand_Generate_هڪجهڙو(gen[i], x_gpu, n);
    check_error(cuda_Peek_At_آخري_Error());
}

عائم cuda_compare(عائم *x_gpu, عائم *x, size_t n, حرف *s)
{
    عائم *tmp = صفر_يادگيري_ڏيو(n, حجم(عائم));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    عائم err = dot_cpu(n, tmp, 1, tmp, 1);
    لکو("Error %s: %f\n", s, جذر(err/n));
    حرر(tmp);
    أرجع err;
}

صحيح *cuda_make_int_array(صحيح *x, size_t n)
{
    صحيح *x_gpu;
    size_t سائيز = حجم(صحيح)*n;
    cudaError_t حالت = cudaMalloc((فراغ **)&x_gpu, سائيز);
    check_error(حالت);
    إذا(x){
        حالت = cudaMemcpy(x_gpu, x, سائيز, cuda_Memcpy_Host_منزل_Device);
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

فراغ cuda_ڌڪو_array(عائم *x_gpu, عائم *x, size_t n)
{
    size_t سائيز = حجم(عائم)*n;
    cudaError_t حالت = cudaMemcpy(x_gpu, x, سائيز, cuda_Memcpy_Host_منزل_Device);
    check_error(حالت);
}

فراغ cuda_pull_array(عائم *x_gpu, عائم *x, size_t n)
{
    size_t سائيز = حجم(عائم)*n;
    cudaError_t حالت = cudaMemcpy(x, x_gpu, سائيز, cuda_Memcpy_Device_منزل_Host);
    check_error(حالت);
}

عائم cuda_mag_array(عائم *x_gpu, size_t n)
{
    عائم *عارضي = صفر_يادگيري_ڏيو(n, حجم(عائم));
    cuda_pull_array(x_gpu, عارضي, n);
    عائم m = mag_array(عارضي, n);
    حرر(عارضي);
    أرجع m;
}
#وإلا
فراغ cuda_سيٽ_device(صحيح n){}

#جيڪڏهن_پورو
