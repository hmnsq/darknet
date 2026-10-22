#ଅନ୍ତର୍ଭାଵ "convolutional_layer.h"
#ଅନ୍ତର୍ଭାଵ "batchnorm_layer.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>

layer make_batchnorm_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.out_h = h;
    l.w = l.out_w = w;
    l.c = l.out_c = c;
    l.output = calloc(h * w * c * batch, ଆକାର(ଦଶମିକ));
    l.delta  = calloc(h * w * c * batch, ଆକାର(ଦଶମିକ));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = calloc(c, ଆକାର(ଦଶମିକ));
    l.scale_updates = calloc(c, ଆକାର(ଦଶମିକ));
    l.biases = calloc(c, ଆକାର(ଦଶମିକ));
    l.bias_updates = calloc(c, ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.mean = calloc(c, ଆକାର(ଦଶମିକ));
    l.variance = calloc(c, ଆକାର(ଦଶମିକ));

    l.rolling_mean = calloc(c, ଆକାର(ଦଶମିକ));
    l.rolling_variance = calloc(c, ଆକାର(ଦଶମିକ));

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
    cudnnSetTensor4dDescriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l.batch, l.out_c, l.out_h, l.out_w); 
    cudnnSetTensor4dDescriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l.out_c, 1, 1); 

    #endif
#endif
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ backward_scale_cpu(ଦଶମିକ *x_norm, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ *scale_updates)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,b,f;
    ପାଇଁ(f = 0; f < n; ++f){
        ଦଶମିକ sum = 0;
        ପାଇଁ(b = 0; b < batch; ++b){
            ପାଇଁ(i = 0; i < size; ++i){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = i + size*(f + n*b);
                sum += delta[index] * x_norm[index];
            }
        }
        scale_updates[f] += sum;
    }
}

ଶୂନ୍ୟ mean_delta_cpu(ଦଶମିକ *delta, ଦଶମିକ *variance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *mean_delta)
{

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    ପାଇଁ(i = 0; i < filters; ++i){
        mean_delta[i] = 0;
        ପାଇଁ (j = 0; j < batch; ++j) {
            ପାଇଁ (k = 0; k < spatial; ++k) {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = j*filters*spatial + i*spatial + k;
                mean_delta[i] += delta[index];
            }
        }
        mean_delta[i] *= (-1./sqrt(variance[i] + .00001f));
    }
}
ଶୂନ୍ୟ  variance_delta_cpu(ଦଶମିକ *x, ଦଶମିକ *delta, ଦଶମିକ *mean, ଦଶମିକ *variance, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *variance_delta)
{

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    ପାଇଁ(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        ପାଇଁ(j = 0; j < batch; ++j){
            ପାଇଁ(k = 0; k < spatial; ++k){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[index]*(x[index] - mean[i]);
            }
        }
        variance_delta[i] *= -.5 * pow(variance[i] + .00001f, (ଦଶମିକ)(-3./2.));
    }
}
ଶୂନ୍ୟ normalize_delta_cpu(ଦଶମିକ *x, ଦଶମିକ *mean, ଦଶମିକ *variance, ଦଶମିକ *mean_delta, ଦଶମିକ *variance_delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା filters, ପୂର୍ଣ୍ଣସଂଖ୍ୟା spatial, ଦଶମିକ *delta)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା f, j, k;
    ପାଇଁ(j = 0; j < batch; ++j){
        ପାଇଁ(f = 0; f < filters; ++f){
            ପାଇଁ(k = 0; k < spatial; ++k){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = j*filters*spatial + f*spatial + k;
                delta[index] = delta[index] * 1./(sqrt(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[index] - mean[f]) / (spatial * batch) + mean_delta[f]/(spatial*batch);
            }
        }
    }
}

ଶୂନ୍ୟ resize_batchnorm_layer(layer *layer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    fprintf(stderr, "Not implemented\n");
}

ଶୂନ୍ୟ forward_batchnorm_layer(layer l, network net)
{
    ଯଦି(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.ନିବେଶ, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    ଯଦି(net.train){
        mean_cpu(l.output, l.batch, l.out_c, l.out_h*l.out_w, l.mean);
        variance_cpu(l.output, l.mean, l.batch, l.out_c, l.out_h*l.out_w, l.variance);

        scal_cpu(l.out_c, .99, l.rolling_mean, 1);
        axpy_cpu(l.out_c, .01, l.mean, 1, l.rolling_mean, 1);
        scal_cpu(l.out_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.out_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.mean, l.variance, l.batch, l.out_c, l.out_h*l.out_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } ଅନ୍ୟଥା {
        normalize_cpu(l.output, l.rolling_mean, l.rolling_variance, l.batch, l.out_c, l.out_h*l.out_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.out_c, l.out_h*l.out_w);
    add_bias(l.output, l.biases, l.batch, l.out_c, l.out_h*l.out_w);
}

ଶୂନ୍ୟ backward_batchnorm_layer(layer l, network net)
{
    ଯଦି(!net.train){
        l.mean = l.rolling_mean;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.out_c, l.out_w*l.out_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.out_c, l.out_w*l.out_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.out_c, l.out_h*l.out_w);

    mean_delta_cpu(l.delta, l.variance, l.batch, l.out_c, l.out_w*l.out_h, l.mean_delta);
    variance_delta_cpu(l.x, l.delta, l.mean, l.variance, l.batch, l.out_c, l.out_w*l.out_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.mean, l.variance, l.mean_delta, l.variance_delta, l.batch, l.out_c, l.out_w*l.out_h, l.delta);
    ଯଦି(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ଶୂନ୍ୟ pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
ଶୂନ୍ୟ ଠେଲ_batchnorm_layer(layer l)
{
    cuda_ଠେଲ_array(l.scales_gpu, l.scales, l.c);
    cuda_ଠେଲ_array(l.rolling_mean_gpu, l.rolling_mean, l.c);
    cuda_ଠେଲ_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

ଶୂନ୍ୟ forward_batchnorm_layer_gpu(layer l, network net)
{
    ଯଦି(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.ନିବେଶ_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    ଯଦି (net.train) {
#ifdef CUDNN
        ଦଶମିକ one = 1;
        ଦଶମିକ zero = 0;
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
#ଅନ୍ୟଥା
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
    } ଅନ୍ୟଥା {
        normalize_gpu(l.output_gpu, l.rolling_mean_gpu, l.rolling_variance_gpu, l.batch, l.out_c, l.out_h*l.out_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_h*l.out_w);
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.out_c, l.out_w*l.out_h);
    }

}

ଶୂନ୍ୟ backward_batchnorm_layer_gpu(layer l, network net)
{
    ଯଦି(!net.train){
        l.mean_gpu = l.rolling_mean_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#ifdef CUDNN
    ଦଶମିକ one = 1;
    ଦଶମିକ zero = 0;
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
#ଅନ୍ୟଥା
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.out_c, l.out_w*l.out_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.out_c, l.out_h*l.out_w);

    fast_mean_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.mean_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.mean_gpu, l.variance_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.mean_gpu, l.variance_gpu, l.mean_delta_gpu, l.variance_delta_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.delta_gpu);
#endif
    ଯଦି(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
