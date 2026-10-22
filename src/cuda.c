પૂર્ણાંક gpu_index = 0;

#ifdef GPU

#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ <assert.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <time.h>

શૂન્ય cuda_set_device(પૂર્ણાંક n)
{
    gpu_index = n;
    cudaError_t status = cudaSetDevice(n);
    check_error(status);
}

પૂર્ણાંક cuda_get_device()
{
    પૂર્ણાંક n = 0;
    cudaError_t status = cudaGetDevice(&n);
    check_error(status);
    પાછા n;
}

શૂન્ય check_error(cudaError_t status)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cudaGetLastError();
    જો (status != cudaSuccess)
    {   
        અચલ અક્ષર *s = cudaGetErrorString(status);
        અક્ષર buffer[256];
        printf("CUDA Error: %s\n", s);
        પ્રતિજ્ઞા(0);
        snprintf(buffer, 256, "CUDA Error: %s", s);
        error(buffer);
    } 
    જો (status2 != cudaSuccess)
    {   
        અચલ અક્ષર *s = cudaGetErrorString(status);
        અક્ષર buffer[256];
        printf("CUDA Error Prev: %s\n", s);
        પ્રતિજ્ઞા(0);
        snprintf(buffer, 256, "CUDA Error Prev: %s", s);
        error(buffer);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / BLOCK + 1;
    size_t x = k;
    size_t y = 1;
    જો(x > 65535){
        x = ceil(sqrt(k));
        y = (n-1)/(x*BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    પાછા d;
}

#ifdef CUDNN
cudnnHandle_t cudnn_handle()
{
    સ્થિર પૂર્ણાંક init[16] = {0};
    સ્થિર cudnnHandle_t handle[16];
    પૂર્ણાંક i = cuda_get_device();
    જો(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    પાછા handle[i];
}
#endif

cublasHandle_t blas_handle()
{
    સ્થિર પૂર્ણાંક init[16] = {0};
    સ્થિર cublasHandle_t handle[16];
    પૂર્ણાંક i = cuda_get_device();
    જો(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    પાછા handle[i];
}

દશાંશ *cuda_make_array(દશાંશ *x, size_t n)
{
    દશાંશ *x_gpu;
    size_t size = કદ(દશાંશ)*n;
    cudaError_t status = cudaMalloc((શૂન્ય **)&x_gpu, size);
    check_error(status);
    જો(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    } નહીંતર {
        fill_gpu(n, 0, x_gpu, 1);
    }
    જો(!x_gpu) error("Cuda malloc failed\n");
    પાછા x_gpu;
}

શૂન્ય cuda_random(દશાંશ *x_gpu, size_t n)
{
    સ્થિર curandGenerator_t gen[16];
    સ્થિર પૂર્ણાંક init[16] = {0};
    પૂર્ણાંક i = cuda_get_device();
    જો(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen[i], time(0));
        init[i] = 1;
    }
    curandGenerateUniform(gen[i], x_gpu, n);
    check_error(cudaPeekAtLastError());
}

દશાંશ cuda_compare(દશાંશ *x_gpu, દશાંશ *x, size_t n, અક્ષર *s)
{
    દશાંશ *tmp = calloc(n, કદ(દશાંશ));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    દશાંશ err = dot_cpu(n, tmp, 1, tmp, 1);
    printf("Error %s: %f\n", s, sqrt(err/n));
    મુક્ત(tmp);
    પાછા err;
}

પૂર્ણાંક *cuda_make_int_array(પૂર્ણાંક *x, size_t n)
{
    પૂર્ણાંક *x_gpu;
    size_t size = કદ(પૂર્ણાંક)*n;
    cudaError_t status = cudaMalloc((શૂન્ય **)&x_gpu, size);
    check_error(status);
    જો(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    }
    જો(!x_gpu) error("Cuda malloc failed\n");
    પાછા x_gpu;
}

શૂન્ય cuda_free(દશાંશ *x_gpu)
{
    cudaError_t status = cudaFree(x_gpu);
    check_error(status);
}

શૂન્ય cuda_ધકેલો_array(દશાંશ *x_gpu, દશાંશ *x, size_t n)
{
    size_t size = કદ(દશાંશ)*n;
    cudaError_t status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
    check_error(status);
}

શૂન્ય cuda_pull_array(દશાંશ *x_gpu, દશાંશ *x, size_t n)
{
    size_t size = કદ(દશાંશ)*n;
    cudaError_t status = cudaMemcpy(x, x_gpu, size, cudaMemcpyDeviceToHost);
    check_error(status);
}

દશાંશ cuda_mag_array(દશાંશ *x_gpu, size_t n)
{
    દશાંશ *temp = calloc(n, કદ(દશાંશ));
    cuda_pull_array(x_gpu, temp, n);
    દશાંશ m = mag_array(temp, n);
    મુક્ત(temp);
    પાછા m;
}
#નહીંતર
શૂન્ય cuda_set_device(પૂર્ણાંક n){}

#endif
