పూర్ణాంకం gpu_సూచీ = 0;

#నిర్వచితమైతే GPU

#అన్తర్భావ "cuda.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ <assert.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <time.h>

శూన్య cuda_సమితి_device(పూర్ణాంకం n)
{
    gpu_సూచీ = n;
    cudaError_t స్థితి_2 = cuda_సమితి_Device(n);
    check_error(స్థితి_2);
}

పూర్ణాంకం cuda_get_device()
{
    పూర్ణాంకం n = 0;
    cudaError_t స్థితి_2 = cudaGetDevice(&n);
    check_error(స్థితి_2);
    తిరిగి n;
}

శూన్య check_error(cudaError_t స్థితి_2)
{
    //cudaDeviceSynchronize();
    cudaError_t status2 = cuda_Get_చివరి_Error();
    ఒకవేళ (స్థితి_2 != cudaSuccess)
    {   
        స్థిరరాశి అక్షరం *s = cudaGetErrorString(స్థితి_2);
        అక్షరం బఫరు[256];
        రాయి("CUDA Error: %s\n", s);
        ప్రతిజ్ఞా(0);
        snprintf(బఫరు, 256, "CUDA Error: %s", s);
        error(బఫరు);
    } 
    ఒకవేళ (status2 != cudaSuccess)
    {   
        స్థిరరాశి అక్షరం *s = cudaGetErrorString(స్థితి_2);
        అక్షరం బఫరు[256];
        రాయి("CUDA Error Prev: %s\n", s);
        ప్రతిజ్ఞా(0);
        snprintf(బఫరు, 256, "CUDA Error Prev: %s", s);
        error(బఫరు);
    } 
}

dim3 cuda_gridsize(size_t n){
    size_t k = (n-1) / ఖండం + 1;
    size_t x = k;
    size_t y = 1;
    ఒకవేళ(x > 65535){
        x = ceil(వర్గమూలం(k));
        y = (n-1)/(x*ఖండం) + 1;
    }
    dim3 d = {x, y, 1};
    //printf("%ld %ld %ld %ld\n", n, x, y, x*y*BLOCK);
    తిరిగి d;
}

#నిర్వచితమైతే CUDNN
cudnnHandle_t cudnn_handle()
{
    స్థిర పూర్ణాంకం init[16] = {0};
    స్థిర cudnnHandle_t handle[16];
    పూర్ణాంకం i = cuda_get_device();
    ఒకవేళ(!init[i]) {
        cudnnCreate(&handle[i]);
        init[i] = 1;
    }
    తిరిగి handle[i];
}
#అయితే_ముగింపు

cublasHandle_t blas_handle()
{
    స్థిర పూర్ణాంకం init[16] = {0};
    స్థిర cublasHandle_t handle[16];
    పూర్ణాంకం i = cuda_get_device();
    ఒకవేళ(!init[i]) {
        cublasCreate(&handle[i]);
        init[i] = 1;
    }
    తిరిగి handle[i];
}

దశాంశం *cuda_make_array(దశాంశం *x, size_t n)
{
    దశాంశం *x_gpu;
    size_t కొలత = పరిమాణం(దశాంశం)*n;
    cudaError_t స్థితి_2 = cudaMalloc((శూన్య **)&x_gpu, కొలత);
    check_error(స్థితి_2);
    ఒకవేళ(x){
        స్థితి_2 = cudaMemcpy(x_gpu, x, కొలత, cuda_Memcpy_Host_గమ్యం_Device);
        check_error(స్థితి_2);
    } లేకపోతే {
        fill_gpu(n, 0, x_gpu, 1);
    }
    ఒకవేళ(!x_gpu) error("Cuda malloc failed\n");
    తిరిగి x_gpu;
}

శూన్య cuda_random(దశాంశం *x_gpu, size_t n)
{
    స్థిర curandGenerator_t gen[16];
    స్థిర పూర్ణాంకం init[16] = {0};
    పూర్ణాంకం i = cuda_get_device();
    ఒకవేళ(!init[i]){
        curandCreateGenerator(&gen[i], CURAND_RNG_PSEUDO_DEFAULT);
        curand_సమితి_Pseudo_Random_Generator_విత్తనం(gen[i], సమయం(0));
        init[i] = 1;
    }
    curand_Generate_ఏకరీతి(gen[i], x_gpu, n);
    check_error(cuda_Peek_At_చివరి_Error());
}

దశాంశం cuda_compare(దశాంశం *x_gpu, దశాంశం *x, size_t n, అక్షరం *s)
{
    దశాంశం *tmp = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
    cuda_pull_array(x_gpu, tmp, n);
    //int i;
    //for(i = 0; i < n; ++i) printf("%f %f\n", tmp[i], x[i]);
    axpy_cpu(n, -1, x, 1, tmp, 1);
    దశాంశం err = dot_cpu(n, tmp, 1, tmp, 1);
    రాయి("Error %s: %f\n", s, వర్గమూలం(err/n));
    విడుదల(tmp);
    తిరిగి err;
}

పూర్ణాంకం *cuda_make_int_array(పూర్ణాంకం *x, size_t n)
{
    పూర్ణాంకం *x_gpu;
    size_t కొలత = పరిమాణం(పూర్ణాంకం)*n;
    cudaError_t స్థితి_2 = cudaMalloc((శూన్య **)&x_gpu, కొలత);
    check_error(స్థితి_2);
    ఒకవేళ(x){
        స్థితి_2 = cudaMemcpy(x_gpu, x, కొలత, cuda_Memcpy_Host_గమ్యం_Device);
        check_error(స్థితి_2);
    }
    ఒకవేళ(!x_gpu) error("Cuda malloc failed\n");
    తిరిగి x_gpu;
}

శూన్య cuda_free(దశాంశం *x_gpu)
{
    cudaError_t స్థితి_2 = cudaFree(x_gpu);
    check_error(స్థితి_2);
}

శూన్య cuda_నెట్టు_array(దశాంశం *x_gpu, దశాంశం *x, size_t n)
{
    size_t కొలత = పరిమాణం(దశాంశం)*n;
    cudaError_t స్థితి_2 = cudaMemcpy(x_gpu, x, కొలత, cuda_Memcpy_Host_గమ్యం_Device);
    check_error(స్థితి_2);
}

శూన్య cuda_pull_array(దశాంశం *x_gpu, దశాంశం *x, size_t n)
{
    size_t కొలత = పరిమాణం(దశాంశం)*n;
    cudaError_t స్థితి_2 = cudaMemcpy(x, x_gpu, కొలత, cuda_Memcpy_Device_గమ్యం_Host);
    check_error(స్థితి_2);
}

దశాంశం cuda_mag_array(దశాంశం *x_gpu, size_t n)
{
    దశాంశం *తాత్కాలిక = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
    cuda_pull_array(x_gpu, తాత్కాలిక, n);
    దశాంశం m = mag_array(తాత్కాలిక, n);
    విడుదల(తాత్కాలిక);
    తిరిగి m;
}
#లేకపోతే
శూన్య cuda_సమితి_device(పూర్ణాంకం n){}

#అయితే_ముగింపు
