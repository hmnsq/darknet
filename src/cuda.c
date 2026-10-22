ପୂର୍ଣ୍ଣସଂଖ୍ୟା gpu_index = 0;

#ifdef GPU

#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ <assert.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

ଶୂନ୍ୟ cuda_set_device(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    gpu_index = n;
    cudaError_t status = cudaSetDevice(n);
    check_error(status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cuda_get_device()
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
    cudaError_t status = cudaGetDevice(&n);
    check_error(status);
    ଫେରନ୍ତୁ n;
}

ଶୂନ୍ୟ check_error(cudaError_t status)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cudaGetLastError();
    ଯଦି (status != cudaSuccess)
    {   
        ଅଚଳ ଅକ୍ଷର *s = cudaGetErrorString(status);
        ଅକ୍ଷର buffer[256];
        printf("CUDA Error: %s\n", s);
        ପ୍ରତିଜ୍ଞା(0);
        snprintf(buffer, 256, "CUDA Error: %s", s);
        error(buffer);
    } 
    ଯଦି (status2 != cudaSuccess)
    {   
        ଅଚଳ ଅକ୍ଷର *s = cudaGetErrorString(status);
        ଅକ୍ଷର buffer[256];
        printf("CUDA Error Prev: %s\n", s);
        ପ୍ରତିଜ୍ଞା(0);
        snprintf(buffer, 256, "CUDA Error Prev: %s", s);
        error(buffer);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / BLOCK + 1;
    size_t x = k;
    size_t y = 1;
    ଯଦି(x > 65535){
        x = ceil(sqrt(k));
        y = (n-1)/(x*BLOCK) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    ଫେରନ୍ତୁ d;
}

#ifdef CUDNN
cudnnHandle_t cudnn_handle()
{
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା init[16] = {0};
    ସ୍ଥିର cudnnHandle_t handle[16];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = cuda_get_device();
    ଯଦି(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    ଫେରନ୍ତୁ handle[i];
}
#endif

cublasHandle_t blas_handle()
{
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା init[16] = {0};
    ସ୍ଥିର cublasHandle_t handle[16];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = cuda_get_device();
    ଯଦି(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    ଫେରନ୍ତୁ handle[i];
}

ଦଶମିକ *cuda_make_array(ଦଶମିକ *x, size_t n)
{
    ଦଶମିକ *x_gpu;
    size_t size = ଆକାର(ଦଶମିକ)*n;
    cudaError_t status = cudaMalloc((ଶୂନ୍ୟ **)&x_gpu, size);
    check_error(status);
    ଯଦି(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    } ଅନ୍ୟଥା {
        fill_gpu(n, 0, x_gpu, 1);
    }
    ଯଦି(!x_gpu) error("Cuda malloc failed\n");
    ଫେରନ୍ତୁ x_gpu;
}

ଶୂନ୍ୟ cuda_random(ଦଶମିକ *x_gpu, size_t n)
{
    ସ୍ଥିର curandGenerator_t gen[16];
    ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା init[16] = {0};
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = cuda_get_device();
    ଯଦି(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curandSetPseudoRandomGeneratorSeed(gen[i], time(0));
        init[i] = 1;
    }
    curandGenerateUniform(gen[i], x_gpu, n);
    check_error(cudaPeekAtLastError());
}

ଦଶମିକ cuda_compare(ଦଶମିକ *x_gpu, ଦଶମିକ *x, size_t n, ଅକ୍ଷର *s)
{
    ଦଶମିକ *tmp = calloc(n, ଆକାର(ଦଶମିକ));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    ଦଶମିକ err = dot_cpu(n, tmp, 1, tmp, 1);
    printf("Error %s: %f\n", s, sqrt(err/n));
    ମୁକ୍ତ(tmp);
    ଫେରନ୍ତୁ err;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା *cuda_make_int_array(ପୂର୍ଣ୍ଣସଂଖ୍ୟା *x, size_t n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *x_gpu;
    size_t size = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*n;
    cudaError_t status = cudaMalloc((ଶୂନ୍ୟ **)&x_gpu, size);
    check_error(status);
    ଯଦି(x){
        status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
        check_error(status);
    }
    ଯଦି(!x_gpu) error("Cuda malloc failed\n");
    ଫେରନ୍ତୁ x_gpu;
}

ଶୂନ୍ୟ cuda_free(ଦଶମିକ *x_gpu)
{
    cudaError_t status = cudaFree(x_gpu);
    check_error(status);
}

ଶୂନ୍ୟ cuda_ଠେଲ_array(ଦଶମିକ *x_gpu, ଦଶମିକ *x, size_t n)
{
    size_t size = ଆକାର(ଦଶମିକ)*n;
    cudaError_t status = cudaMemcpy(x_gpu, x, size, cudaMemcpyHostToDevice);
    check_error(status);
}

ଶୂନ୍ୟ cuda_pull_array(ଦଶମିକ *x_gpu, ଦଶମିକ *x, size_t n)
{
    size_t size = ଆକାର(ଦଶମିକ)*n;
    cudaError_t status = cudaMemcpy(x, x_gpu, size, cudaMemcpyDeviceToHost);
    check_error(status);
}

ଦଶମିକ cuda_mag_array(ଦଶମିକ *x_gpu, size_t n)
{
    ଦଶମିକ *temp = calloc(n, ଆକାର(ଦଶମିକ));
    cuda_pull_array(x_gpu, temp, n);
    ଦଶମିକ m = mag_array(temp, n);
    ମୁକ୍ତ(temp);
    ଫେରନ୍ତୁ m;
}
#ଅନ୍ୟଥା
ଶୂନ୍ୟ cuda_set_device(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n){}

#endif
