puur_naa_nka gpu_index = 0;

#ifdef GPU

#include "cuda.h"
#include "utils.h"
#include "blas.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

void cuda_set_device(puur_naa_nka n)
{
    gpu_index = n;
    cudaError_t status = cudaSetDevice(n);
    check_error(status);
}

puur_naa_nka cuda_get_device()
{
    puur_naa_nka n = 0;
    cudaError_t status = cudaGetDevice(&n);
    check_error(status);
    ᰀᰧᰌ n;
}

void check_error(cudaError_t status)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cudaGetLastError();
    ᰀᰧ (status != cudaSuccess)
    {   
        const var_na *s = cudaGetErrorString(status);
        var_na buffer[256];
        printf("CUDA Error: %s\n", s);
        assert(0);
        snprintf(buffer, 256, "CUDA Error: %s", s);
        error(buffer);
    } 
    ᰀᰧ (status2 != cudaSuccess)
    {   
        const var_na *s = cudaGetErrorString(status);
        var_na buffer[256];
        printf("CUDA Error Prev: %s\n", s);
        assert(0);
        snprintf(buffer, 256, "CUDA Error Prev: %s", s);
        error(buffer);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / BLOCK + 1;
    size_t x = k;
    size_t y = 1;
    ᰀᰧ(x > 65535){
        x = ceil(sqrt(k));
        y = (n-1)/(x*BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    ᰀᰧᰌ d;
}

#ifdef CUDNN
cudnnHandle_t cudnn_handle()
{
    static puur_naa_nka init[16] = {0};
    static cudnnHandle_t handle[16];
    puur_naa_nka i = cuda_get_device();
    ᰀᰧ(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    ᰀᰧᰌ handle[i];
}
#endif

cublasHandle_t blas_handle()
{
    static puur_naa_nka init[16] = {0};
    static cublasHandle_t handle[16];
    puur_naa_nka i = cuda_get_device();
    ᰀᰧ(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    ᰀᰧᰌ handle[i];
}

float *cuda_make_array(float *x, size_t n)
{
    float *x_gpu;
    size_t size = parimaa_na(float)*n;
    cudaError_t status = cudaMalloc((void **)&x_gpu, size);
    check_error(status);
    ᰀᰧ(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    } ᰀᰧᰀ {
        fill_gpu(n, 0, x_gpu, 1);
    }
    ᰀᰧ(!x_gpu) error("Cuda malloc failed\n");
    ᰀᰧᰌ x_gpu;
}

void cuda_random(float *x_gpu, size_t n)
{
    static curandGenerator_t gen[16];
    static puur_naa_nka init[16] = {0};
    puur_naa_nka i = cuda_get_device();
    ᰀᰧ(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen[i], time(0));
        init[i] = 1;
    }
    curandGenerateUniform(gen[i], x_gpu, n);
    check_error(cudaPeekAtLastError());
}

float cuda_compare(float *x_gpu, float *x, size_t n, var_na *s)
{
    float *tmp = calloc(n, parimaa_na(float));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    float err = dot_cpu(n, tmp, 1, tmp, 1);
    printf("Error %s: %f\n", s, sqrt(err/n));
    free(tmp);
    ᰀᰧᰌ err;
}

puur_naa_nka *cuda_make_int_array(puur_naa_nka *x, size_t n)
{
    puur_naa_nka *x_gpu;
    size_t size = parimaa_na(puur_naa_nka)*n;
    cudaError_t status = cudaMalloc((void **)&x_gpu, size);
    check_error(status);
    ᰀᰧ(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    }
    ᰀᰧ(!x_gpu) error("Cuda malloc failed\n");
    ᰀᰧᰌ x_gpu;
}

void cuda_free(float *x_gpu)
{
    cudaError_t status = cudaFree(x_gpu);
    check_error(status);
}

void cuda_push_array(float *x_gpu, float *x, size_t n)
{
    size_t size = parimaa_na(float)*n;
    cudaError_t status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
    check_error(status);
}

void cuda_pull_array(float *x_gpu, float *x, size_t n)
{
    size_t size = parimaa_na(float)*n;
    cudaError_t status = cudaMemcpy(x, x_gpu, size, cudaMemcpyDeviceToHost);
    check_error(status);
}

float cuda_mag_array(float *x_gpu, size_t n)
{
    float *temp = calloc(n, parimaa_na(float));
    cuda_pull_array(x_gpu, temp, n);
    float m = mag_array(temp, n);
    free(temp);
    ᰀᰧᰌ m;
}
#ᰀᰧᰀ
void cuda_set_device(puur_naa_nka n){}

#endif
