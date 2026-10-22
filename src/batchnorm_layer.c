#অন্তর্ভাব "convolutional_layer.h"
#অন্তর্ভাব "batchnorm_layer.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব <stdio.h>

layer make_batchnorm_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.out_h = h;
    l.w = l.out_w = w;
    l.c = l.out_c = c;
    l.output = calloc(h * w * c * batch, আকার(দশমিক));
    l.delta  = calloc(h * w * c * batch, আকার(দশমিক));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = calloc(c, আকার(দশমিক));
    l.scale_updates = calloc(c, আকার(দশমিক));
    l.biases = calloc(c, আকার(দশমিক));
    l.bias_updates = calloc(c, আকার(দশমিক));
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.mean = calloc(c, আকার(দশমিক));
    l.variance = calloc(c, আকার(দশমিক));

    l.rolling_mean = calloc(c, আকার(দশমিক));
    l.rolling_variance = calloc(c, আকার(দশমিক));

    l.forward = forward_batchnorm_layer;
    l.backward = backward_batchnorm_layer;
#ifdef GPU
    l.forward_gpu = forward_batchnorm_layer_gpu;
    l.backward_gpu = backward_batchnorm_layer_gpu;

    l.output_gpu =  cuda_make_array(l.output, h * w * c * batch);
    l.delta_gpu =   cuda_make_array(l.delta, h * w * c * batch);

    l.biases_gpu = cuda_make_array(l.biases, c);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, c);

    l.scales_gpu = cuda_make_array(l.scales, c);
    l.scale_updates_gpu = cuda_make_array(l.scale_updates, c);

    l.mean_gpu = cuda_make_array(l.mean, c);
    l.variance_gpu = cuda_make_array(l.variance, c);

    l.rolling_mean_gpu = cuda_make_array(l.mean, c);
    l.rolling_variance_gpu = cuda_make_array(l.variance, c);

    l.mean_delta_gpu = cuda_make_array(l.mean, c);
    l.variance_delta_gpu = cuda_make_array(l.variance, c);

    l.x_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    l.x_norm_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    #ifdef CUDNN
    cudnnCreateTensorDescriptor(&l.normTensorDesc);
    cudnnCreateTensorDescriptor(&l.dstTensorDesc);
    cudnnSetTensor4dDescriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_তথ্য_FLOAT, l.batch, l.out_c, l.out_h, l.out_w); 
    cudnnSetTensor4dDescriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_তথ্য_FLOAT, 1, l.out_c, 1, 1); 

    #endif
#endif
    ফেরত l;
}

শূন্য backward_scale_cpu(দশমিক *x_norm, দশমিক *delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, দশমিক *scale_updates)
{
    পূর্ণসংখ্যা i,b,f;
    জন্য(f = 0; f < n; ++f){
        দশমিক যোগ = 0;
        জন্য(b = 0; b < batch; ++b){
            জন্য(i = 0; i < আকার_2; ++i){
                পূর্ণসংখ্যা সূচক = i + আকার_2*(f + n*b);
                যোগ += delta[সূচক] * x_norm[সূচক];
            }
        }
        scale_updates[f] += যোগ;
    }
}

শূন্য mean_delta_cpu(দশমিক *delta, দশমিক *variance, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *mean_delta)
{

    পূর্ণসংখ্যা i,j,k;
    জন্য(i = 0; i < filters; ++i){
        mean_delta[i] = 0;
        জন্য (j = 0; j < batch; ++j) {
            জন্য (k = 0; k < spatial; ++k) {
                পূর্ণসংখ্যা সূচক = j*filters*spatial + i*spatial + k;
                mean_delta[i] += delta[সূচক];
            }
        }
        mean_delta[i] *= (-1./sqrt(variance[i] + .00001f));
    }
}
শূন্য  variance_delta_cpu(দশমিক *x, দশমিক *delta, দশমিক *mean, দশমিক *variance, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *variance_delta)
{

    পূর্ণসংখ্যা i,j,k;
    জন্য(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        জন্য(j = 0; j < batch; ++j){
            জন্য(k = 0; k < spatial; ++k){
                পূর্ণসংখ্যা সূচক = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[সূচক]*(x[সূচক] - mean[i]);
            }
        }
        variance_delta[i] *= -.5 * pow(variance[i] + .00001f, (দশমিক)(-3./2.));
    }
}
শূন্য normalize_delta_cpu(দশমিক *x, দশমিক *mean, দশমিক *variance, দশমিক *mean_delta, দশমিক *variance_delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা filters, পূর্ণসংখ্যা spatial, দশমিক *delta)
{
    পূর্ণসংখ্যা f, j, k;
    জন্য(j = 0; j < batch; ++j){
        জন্য(f = 0; f < filters; ++f){
            জন্য(k = 0; k < spatial; ++k){
                পূর্ণসংখ্যা সূচক = j*filters*spatial + f*spatial + k;
                delta[সূচক] = delta[সূচক] * 1./(sqrt(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[সূচক] - mean[f]) / (spatial * batch) + mean_delta[f]/(spatial*batch);
            }
        }
    }
}

শূন্য resize_batchnorm_layer(layer *layer, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    fprintf(stderr, "Not implemented\n");
}

শূন্য forward_batchnorm_layer(layer l, network net)
{
    যদি(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.প্রবেশ, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    যদি(net.train){
        mean_cpu(l.output, l.batch, l.out_c, l.out_h*l.out_w, l.mean);
        variance_cpu(l.output, l.mean, l.batch, l.out_c, l.out_h*l.out_w, l.variance);

        scal_cpu(l.out_c, .99, l.rolling_mean, 1);
        axpy_cpu(l.out_c, .01, l.mean, 1, l.rolling_mean, 1);
        scal_cpu(l.out_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.out_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.mean, l.variance, l.batch, l.out_c, l.out_h*l.out_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } অন্যথা {
        normalize_cpu(l.output, l.rolling_mean, l.rolling_variance, l.batch, l.out_c, l.out_h*l.out_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.out_c, l.out_h*l.out_w);
    add_bias(l.output, l.biases, l.batch, l.out_c, l.out_h*l.out_w);
}

শূন্য backward_batchnorm_layer(layer l, network net)
{
    যদি(!net.train){
        l.mean = l.rolling_mean;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.out_c, l.out_w*l.out_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.out_c, l.out_w*l.out_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.out_c, l.out_h*l.out_w);

    mean_delta_cpu(l.delta, l.variance, l.batch, l.out_c, l.out_w*l.out_h, l.mean_delta);
    variance_delta_cpu(l.x, l.delta, l.mean, l.variance, l.batch, l.out_c, l.out_w*l.out_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.mean, l.variance, l.mean_delta, l.variance_delta, l.batch, l.out_c, l.out_w*l.out_h, l.delta);
    যদি(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ifdef GPU

শূন্য pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
শূন্য ঠেলো_batchnorm_layer(layer l)
{
    cuda_ঠেলো_array(l.scales_gpu, l.scales, l.c);
    cuda_ঠেলো_array(l.rolling_mean_gpu, l.rolling_mean, l.c);
    cuda_ঠেলো_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

শূন্য forward_batchnorm_layer_gpu(layer l, network net)
{
    যদি(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    যদি (net.train) {
#ifdef CUDNN
        দশমিক one = 1;
        দশমিক zero = 0;
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
                l.rolling_mean_gpu,
                l.rolling_variance_gpu,
                .00001,
                l.mean_gpu,
                l.variance_gpu);
#অন্যথা
        fast_mean_gpu(l.output_gpu, l.batch, l.out_c, l.out_h*l.out_w, l.mean_gpu);
        fast_variance_gpu(l.output_gpu, l.mean_gpu, l.batch, l.out_c, l.out_h*l.out_w, l.variance_gpu);

        scal_gpu(l.out_c, .99, l.rolling_mean_gpu, 1);
        axpy_gpu(l.out_c, .01, l.mean_gpu, 1, l.rolling_mean_gpu, 1);
        scal_gpu(l.out_c, .99, l.rolling_variance_gpu, 1);
        axpy_gpu(l.out_c, .01, l.variance_gpu, 1, l.rolling_variance_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
        normalize_gpu(l.output_gpu, l.mean_gpu, l.variance_gpu, l.batch, l.out_c, l.out_h*l.out_w);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_norm_gpu, 1);

        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_h*l.out_w);
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.out_c, l.out_w*l.out_h);
#endif
    } অন্যথা {
        normalize_gpu(l.output_gpu, l.rolling_mean_gpu, l.rolling_variance_gpu, l.batch, l.out_c, l.out_h*l.out_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_h*l.out_w);
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.out_c, l.out_w*l.out_h);
    }

}

শূন্য backward_batchnorm_layer_gpu(layer l, network net)
{
    যদি(!net.train){
        l.mean_gpu = l.rolling_mean_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#ifdef CUDNN
    দশমিক one = 1;
    দশমিক zero = 0;
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
            l.mean_gpu,
            l.variance_gpu);
    copy_gpu(l.outputs*l.batch, l.x_norm_gpu, 1, l.delta_gpu, 1);
#অন্যথা
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.out_c, l.out_w*l.out_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.out_c, l.out_h*l.out_w);

    fast_mean_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.mean_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.mean_gpu, l.variance_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.mean_gpu, l.variance_gpu, l.mean_delta_gpu, l.variance_delta_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.delta_gpu);
#endif
    যদি(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
