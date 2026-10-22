#ضمن "convolutional_layer.h"
#ضمن "batchnorm_layer.h"
#ضمن "blas.h"
#ضمن <stdio.h>

layer make_batchnorm_layer(صحيح batch, صحيح w, صحيح h, صحيح c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.بهر_h = h;
    l.w = l.بهر_w = w;
    l.c = l.بهر_c = c;
    l.output = حافظه_صفر_ورکړه(h * w * c * batch, حجم(عائم));
    l.delta  = حافظه_صفر_ورکړه(h * w * c * batch, حجم(عائم));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = حافظه_صفر_ورکړه(c, حجم(عائم));
    l.scale_updates = حافظه_صفر_ورکړه(c, حجم(عائم));
    l.biases = حافظه_صفر_ورکړه(c, حجم(عائم));
    l.bias_updates = حافظه_صفر_ورکړه(c, حجم(عائم));
    صحيح i;
    لكل(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.منځنۍ = حافظه_صفر_ورکړه(c, حجم(عائم));
    l.variance = حافظه_صفر_ورکړه(c, حجم(عائم));

    l.rolling_منځنۍ = حافظه_صفر_ورکړه(c, حجم(عائم));
    l.rolling_variance = حافظه_صفر_ورکړه(c, حجم(عائم));

    l.forward = forward_batchnorm_layer;
    l.backward = backward_batchnorm_layer;
#که_تعریف_شوی GPU
    l.forward_gpu = forward_batchnorm_layer_gpu;
    l.backward_gpu = backward_batchnorm_layer_gpu;

    l.output_gpu =  cuda_make_array(l.output, h * w * c * batch);
    l.delta_gpu =   cuda_make_array(l.delta, h * w * c * batch);

    l.biases_gpu = cuda_make_array(l.biases, c);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, c);

    l.scales_gpu = cuda_make_array(l.scales, c);
    l.scale_updates_gpu = cuda_make_array(l.scale_updates, c);

    l.منځنۍ_gpu = cuda_make_array(l.منځنۍ, c);
    l.variance_gpu = cuda_make_array(l.variance, c);

    l.rolling_منځنۍ_gpu = cuda_make_array(l.منځنۍ, c);
    l.rolling_variance_gpu = cuda_make_array(l.variance, c);

    l.منځنۍ_delta_gpu = cuda_make_array(l.منځنۍ, c);
    l.variance_delta_gpu = cuda_make_array(l.variance, c);

    l.x_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    l.x_norm_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    #که_تعریف_شوی CUDNN
    cudnnCreateTensorDescriptor(&l.normTensorDesc);
    cudnnCreateTensorDescriptor(&l.dstTensorDesc);
    cudnn_سېټ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l.batch, l.بهر_c, l.بهر_h, l.بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, 1, l.بهر_c, 1, 1); 

    #د_که_پای
#د_که_پای
    أرجع l;
}

فراغ backward_scale_cpu(عائم *x_norm, عائم *delta, صحيح batch, صحيح n, صحيح اندازه, عائم *scale_updates)
{
    صحيح i,b,f;
    لكل(f = 0; f < n; ++f){
        عائم مجموعه = 0;
        لكل(b = 0; b < batch; ++b){
            لكل(i = 0; i < اندازه; ++i){
                صحيح شاخص = i + اندازه*(f + n*b);
                مجموعه += delta[شاخص] * x_norm[شاخص];
            }
        }
        scale_updates[f] += مجموعه;
    }
}

فراغ منځنۍ_delta_cpu(عائم *delta, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *منځنۍ_delta)
{

    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        منځنۍ_delta[i] = 0;
        لكل (j = 0; j < batch; ++j) {
            لكل (k = 0; k < spatial; ++k) {
                صحيح شاخص = j*filters*spatial + i*spatial + k;
                منځنۍ_delta[i] += delta[شاخص];
            }
        }
        منځنۍ_delta[i] *= (-1./جذر(variance[i] + .00001f));
    }
}
فراغ  variance_delta_cpu(عائم *x, عائم *delta, عائم *منځنۍ, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *variance_delta)
{

    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح شاخص = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[شاخص]*(x[شاخص] - منځنۍ[i]);
            }
        }
        variance_delta[i] *= -.5 * توان(variance[i] + .00001f, (عائم)(-3./2.));
    }
}
فراغ normalize_delta_cpu(عائم *x, عائم *منځنۍ, عائم *variance, عائم *منځنۍ_delta, عائم *variance_delta, صحيح batch, صحيح filters, صحيح spatial, عائم *delta)
{
    صحيح f, j, k;
    لكل(j = 0; j < batch; ++j){
        لكل(f = 0; f < filters; ++f){
            لكل(k = 0; k < spatial; ++k){
                صحيح شاخص = j*filters*spatial + f*spatial + k;
                delta[شاخص] = delta[شاخص] * 1./(جذر(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[شاخص] - منځنۍ[f]) / (spatial * batch) + منځنۍ_delta[f]/(spatial*batch);
            }
        }
    }
}

فراغ resize_batchnorm_layer(layer *layer, صحيح w, صحيح h)
{
    fprintf(stderr, "Not implemented\n");
}

فراغ forward_batchnorm_layer(layer l, شبکه net)
{
    إذا(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.ننوت, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    إذا(net.train){
        منځنۍ_cpu(l.output, l.batch, l.بهر_c, l.بهر_h*l.بهر_w, l.منځنۍ);
        variance_cpu(l.output, l.منځنۍ, l.batch, l.بهر_c, l.بهر_h*l.بهر_w, l.variance);

        scal_cpu(l.بهر_c, .99, l.rolling_منځنۍ, 1);
        axpy_cpu(l.بهر_c, .01, l.منځنۍ, 1, l.rolling_منځنۍ, 1);
        scal_cpu(l.بهر_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.بهر_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.منځنۍ, l.variance, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } وإلا {
        normalize_cpu(l.output, l.rolling_منځنۍ, l.rolling_variance, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);
    جمع_کړه_bias(l.output, l.biases, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);
}

فراغ backward_batchnorm_layer(layer l, شبکه net)
{
    إذا(!net.train){
        l.منځنۍ = l.rolling_منځنۍ;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.بهر_c, l.بهر_w*l.بهر_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.بهر_c, l.بهر_w*l.بهر_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);

    منځنۍ_delta_cpu(l.delta, l.variance, l.batch, l.بهر_c, l.بهر_w*l.بهر_h, l.منځنۍ_delta);
    variance_delta_cpu(l.x, l.delta, l.منځنۍ, l.variance, l.batch, l.بهر_c, l.بهر_w*l.بهر_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.منځنۍ, l.variance, l.منځنۍ_delta, l.variance_delta, l.batch, l.بهر_c, l.بهر_w*l.بهر_h, l.delta);
    إذا(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#که_تعریف_شوی GPU

فراغ pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_منځنۍ_gpu, l.rolling_منځنۍ, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
فراغ ټېل_batchnorm_layer(layer l)
{
    cuda_ټېل_array(l.scales_gpu, l.scales, l.c);
    cuda_ټېل_array(l.rolling_منځنۍ_gpu, l.rolling_منځنۍ, l.c);
    cuda_ټېل_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

فراغ forward_batchnorm_layer_gpu(layer l, شبکه net)
{
    إذا(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.ننوت_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    إذا (net.train) {
#که_تعریف_شوی CUDNN
        عائم one = 1;
        عائم zero = 0;
        cudnnBatchNormalizationForwardTraining(cudnn_handle(),
                CUDNN_BATCHNORM_SPATIAL,
                &one,
                &zero,
                l.dstTensorDesc,
                l.x_gpu,
                l.dstTensorDesc,
                l.output_gpu,
                l.normTensorDesc,
                l.scales_gpu,
                l.biases_gpu,
                .01,
                l.rolling_منځنۍ_gpu,
                l.rolling_variance_gpu,
                .00001,
                l.منځنۍ_gpu,
                l.variance_gpu);
#وإلا
        fast_منځنۍ_gpu(l.output_gpu, l.batch, l.بهر_c, l.بهر_h*l.بهر_w, l.منځنۍ_gpu);
        fast_variance_gpu(l.output_gpu, l.منځنۍ_gpu, l.batch, l.بهر_c, l.بهر_h*l.بهر_w, l.variance_gpu);

        scal_gpu(l.بهر_c, .99, l.rolling_منځنۍ_gpu, 1);
        axpy_gpu(l.بهر_c, .01, l.منځنۍ_gpu, 1, l.rolling_منځنۍ_gpu, 1);
        scal_gpu(l.بهر_c, .99, l.rolling_variance_gpu, 1);
        axpy_gpu(l.بهر_c, .01, l.variance_gpu, 1, l.rolling_variance_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
        normalize_gpu(l.output_gpu, l.منځنۍ_gpu, l.variance_gpu, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_norm_gpu, 1);

        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);
        جمع_کړه_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.بهر_c, l.بهر_w*l.بهر_h);
#د_که_پای
    } وإلا {
        normalize_gpu(l.output_gpu, l.rolling_منځنۍ_gpu, l.rolling_variance_gpu, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);
        جمع_کړه_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.بهر_c, l.بهر_w*l.بهر_h);
    }

}

فراغ backward_batchnorm_layer_gpu(layer l, شبکه net)
{
    إذا(!net.train){
        l.منځنۍ_gpu = l.rolling_منځنۍ_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#که_تعریف_شوی CUDNN
    عائم one = 1;
    عائم zero = 0;
    cudnnBatchNormalizationBackward(cudnn_handle(),
            CUDNN_BATCHNORM_SPATIAL,
            &one,
            &zero,
            &one,
            &one,
            l.dstTensorDesc,
            l.x_gpu,
            l.dstTensorDesc,
            l.delta_gpu,
            l.dstTensorDesc,
            l.x_norm_gpu,
            l.normTensorDesc,
            l.scales_gpu,
            l.scale_updates_gpu,
            l.bias_updates_gpu,
            .00001,
            l.منځنۍ_gpu,
            l.variance_gpu);
    copy_gpu(l.outputs*l.batch, l.x_norm_gpu, 1, l.delta_gpu, 1);
#وإلا
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.بهر_c, l.بهر_w*l.بهر_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.بهر_c, l.بهر_w*l.بهر_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.بهر_c, l.بهر_h*l.بهر_w);

    fast_منځنۍ_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.بهر_c, l.بهر_w*l.بهر_h, l.منځنۍ_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.منځنۍ_gpu, l.variance_gpu, l.batch, l.بهر_c, l.بهر_w*l.بهر_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.منځنۍ_gpu, l.variance_gpu, l.منځنۍ_delta_gpu, l.variance_delta_gpu, l.batch, l.بهر_c, l.بهر_w*l.بهر_h, l.delta_gpu);
#د_که_پای
    إذا(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#د_که_پای
