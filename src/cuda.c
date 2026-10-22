পূর্ণসংখ্যা gpu_সূচক = 0;

#ifdef GPU

#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব <assert.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <time.h>

শূন্য cuda_set_device(পূর্ণসংখ্যা n)
{
    gpu_সূচক = n;
    cudaError_t status = cudaSetDevice(n);
    check_error(status);
}

পূর্ণসংখ্যা cuda_get_device()
{
    পূর্ণসংখ্যা n = 0;
    cudaError_t status = cudaGetDevice(&n);
    check_error(status);
    ফেরত n;
}

শূন্য check_error(cudaError_t status)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cudaGetLastError();
    যদি (status != cudaSuccess)
    {   
        ধ্রুবক অক্ষর *s = cudaGetErrorString(status);
        অক্ষর বাফার[256];
        printf("CUDA Error: %s\n", s);
        প্রতিজ্ঞা(0);
        snprintf(বাফার, 256, "CUDA Error: %s", s);
        error(বাফার);
    } 
    যদি (status2 != cudaSuccess)
    {   
        ধ্রুবক অক্ষর *s = cudaGetErrorString(status);
        অক্ষর বাফার[256];
        printf("CUDA Error Prev: %s\n", s);
        প্রতিজ্ঞা(0);
        snprintf(বাফার, 256, "CUDA Error Prev: %s", s);
        error(বাফার);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / BLOCK + 1;
    size_t x = k;
    size_t y = 1;
    যদি(x > 65535){
        x = ceil(sqrt(k));
        y = (n-1)/(x*BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    ফেরত d;
}

#ifdef CUDNN
cudnnHandle_t cudnn_handle()
{
    স্থির পূর্ণসংখ্যা init[16] = {0};
    স্থির cudnnHandle_t handle[16];
    পূর্ণসংখ্যা i = cuda_get_device();
    যদি(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    ফেরত handle[i];
}
#endif

cublasHandle_t blas_handle()
{
    স্থির পূর্ণসংখ্যা init[16] = {0};
    স্থির cublasHandle_t handle[16];
    পূর্ণসংখ্যা i = cuda_get_device();
    যদি(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    ফেরত handle[i];
}

দশমিক *cuda_make_array(দশমিক *x, size_t n)
{
    দশমিক *x_gpu;
    size_t আকার_2 = আকার(দশমিক)*n;
    cudaError_t status = cudaMalloc((শূন্য **)&x_gpu, আকার_2);
    check_error(status);
    যদি(x){
        status = cudaMemcpy(x_gpu, x, আকার_2, cudaMemcpyHostToDevice);
        check_error(status);
    } অন্যথা {
        fill_gpu(n, 0, x_gpu, 1);
    }
    যদি(!x_gpu) error("Cuda malloc failed\n");
    ফেরত x_gpu;
}

শূন্য cuda_random(দশমিক *x_gpu, size_t n)
{
    স্থির curandGenerator_t gen[16];
    স্থির পূর্ণসংখ্যা init[16] = {0};
    পূর্ণসংখ্যা i = cuda_get_device();
    যদি(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen[i], time(0));
        init[i] = 1;
    }
    curandGenerateUniform(gen[i], x_gpu, n);
    check_error(cudaPeekAtLastError());
}

দশমিক cuda_compare(দশমিক *x_gpu, দশমিক *x, size_t n, অক্ষর *s)
{
    দশমিক *tmp = calloc(n, আকার(দশমিক));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    দশমিক err = dot_cpu(n, tmp, 1, tmp, 1);
    printf("Error %s: %f\n", s, sqrt(err/n));
    মুক্ত(tmp);
    ফেরত err;
}

পূর্ণসংখ্যা *cuda_make_int_array(পূর্ণসংখ্যা *x, size_t n)
{
    পূর্ণসংখ্যা *x_gpu;
    size_t আকার_2 = আকার(পূর্ণসংখ্যা)*n;
    cudaError_t status = cudaMalloc((শূন্য **)&x_gpu, আকার_2);
    check_error(status);
    যদি(x){
        status = cudaMemcpy(x_gpu, x, আকার_2, cudaMemcpyHostToDevice);
        check_error(status);
    }
    যদি(!x_gpu) error("Cuda malloc failed\n");
    ফেরত x_gpu;
}

শূন্য cuda_free(দশমিক *x_gpu)
{
    cudaError_t status = cudaFree(x_gpu);
    check_error(status);
}

শূন্য cuda_ঠেলো_array(দশমিক *x_gpu, দশমিক *x, size_t n)
{
    size_t আকার_2 = আকার(দশমিক)*n;
    cudaError_t status = cudaMemcpy(x_gpu, x, আকার_2, cudaMemcpyHostToDevice);
    check_error(status);
}

শূন্য cuda_pull_array(দশমিক *x_gpu, দশমিক *x, size_t n)
{
    size_t আকার_2 = আকার(দশমিক)*n;
    cudaError_t status = cudaMemcpy(x, x_gpu, আকার_2, cudaMemcpyDeviceToHost);
    check_error(status);
}

দশমিক cuda_mag_array(দশমিক *x_gpu, size_t n)
{
    দশমিক *অস্থায়ী = calloc(n, আকার(দশমিক));
    cuda_pull_array(x_gpu, অস্থায়ী, n);
    দশমিক m = mag_array(অস্থায়ী, n);
    মুক্ত(অস্থায়ী);
    ফেরত m;
}
#অন্যথা
শূন্য cuda_set_device(পূর্ণসংখ্যা n){}

#endif
