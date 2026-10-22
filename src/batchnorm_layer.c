#અન્તર્ભાવ "convolutional_layer.h"
#અન્તર્ભાવ "batchnorm_layer.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ <stdio.h>

layer make_batchnorm_layer(પૂર્ણાંક batch, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.out_h = h;
    l.w = l.out_w = w;
    l.c = l.out_c = c;
    l.output = calloc(h * w * c * batch, કદ(દશાંશ));
    l.delta  = calloc(h * w * c * batch, કદ(દશાંશ));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = calloc(c, કદ(દશાંશ));
    l.scale_updates = calloc(c, કદ(દશાંશ));
    l.biases = calloc(c, કદ(દશાંશ));
    l.bias_updates = calloc(c, કદ(દશાંશ));
    પૂર્ણાંક i;
    માટે(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.mean = calloc(c, કદ(દશાંશ));
    l.variance = calloc(c, કદ(દશાંશ));

    l.rolling_mean = calloc(c, કદ(દશાંશ));
    l.rolling_variance = calloc(c, કદ(દશાંશ));

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
    પાછા l;
}

શૂન્ય backward_scale_cpu(દશાંશ *x_norm, દશાંશ *delta, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size, દશાંશ *scale_updates)
{
    પૂર્ણાંક i,b,f;
    માટે(f = 0; f < n; ++f){
        દશાંશ sum = 0;
        માટે(b = 0; b < batch; ++b){
            માટે(i = 0; i < size; ++i){
                પૂર્ણાંક index = i + size*(f + n*b);
                sum += delta[index] * x_norm[index];
            }
        }
        scale_updates[f] += sum;
    }
}

શૂન્ય mean_delta_cpu(દશાંશ *delta, દશાંશ *variance, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *mean_delta)
{

    પૂર્ણાંક i,j,k;
    માટે(i = 0; i < filters; ++i){
        mean_delta[i] = 0;
        માટે (j = 0; j < batch; ++j) {
            માટે (k = 0; k < spatial; ++k) {
                પૂર્ણાંક index = j*filters*spatial + i*spatial + k;
                mean_delta[i] += delta[index];
            }
        }
        mean_delta[i] *= (-1./sqrt(variance[i] + .00001f));
    }
}
શૂન્ય  variance_delta_cpu(દશાંશ *x, દશાંશ *delta, દશાંશ *mean, દશાંશ *variance, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *variance_delta)
{

    પૂર્ણાંક i,j,k;
    માટે(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        માટે(j = 0; j < batch; ++j){
            માટે(k = 0; k < spatial; ++k){
                પૂર્ણાંક index = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[index]*(x[index] - mean[i]);
            }
        }
        variance_delta[i] *= -.5 * pow(variance[i] + .00001f, (દશાંશ)(-3./2.));
    }
}
શૂન્ય normalize_delta_cpu(દશાંશ *x, દશાંશ *mean, દશાંશ *variance, દશાંશ *mean_delta, દશાંશ *variance_delta, પૂર્ણાંક batch, પૂર્ણાંક filters, પૂર્ણાંક spatial, દશાંશ *delta)
{
    પૂર્ણાંક f, j, k;
    માટે(j = 0; j < batch; ++j){
        માટે(f = 0; f < filters; ++f){
            માટે(k = 0; k < spatial; ++k){
                પૂર્ણાંક index = j*filters*spatial + f*spatial + k;
                delta[index] = delta[index] * 1./(sqrt(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[index] - mean[f]) / (spatial * batch) + mean_delta[f]/(spatial*batch);
            }
        }
    }
}

શૂન્ય resize_batchnorm_layer(layer *layer, પૂર્ણાંક w, પૂર્ણાંક h)
{
    fprintf(stderr, "Not implemented\n");
}

શૂન્ય forward_batchnorm_layer(layer l, network net)
{
    જો(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.આગત, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    જો(net.train){
        mean_cpu(l.output, l.batch, l.out_c, l.out_h*l.out_w, l.mean);
        variance_cpu(l.output, l.mean, l.batch, l.out_c, l.out_h*l.out_w, l.variance);

        scal_cpu(l.out_c, .99, l.rolling_mean, 1);
        axpy_cpu(l.out_c, .01, l.mean, 1, l.rolling_mean, 1);
        scal_cpu(l.out_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.out_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.mean, l.variance, l.batch, l.out_c, l.out_h*l.out_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } નહીંતર {
        normalize_cpu(l.output, l.rolling_mean, l.rolling_variance, l.batch, l.out_c, l.out_h*l.out_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.out_c, l.out_h*l.out_w);
    add_bias(l.output, l.biases, l.batch, l.out_c, l.out_h*l.out_w);
}

શૂન્ય backward_batchnorm_layer(layer l, network net)
{
    જો(!net.train){
        l.mean = l.rolling_mean;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.out_c, l.out_w*l.out_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.out_c, l.out_w*l.out_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.out_c, l.out_h*l.out_w);

    mean_delta_cpu(l.delta, l.variance, l.batch, l.out_c, l.out_w*l.out_h, l.mean_delta);
    variance_delta_cpu(l.x, l.delta, l.mean, l.variance, l.batch, l.out_c, l.out_w*l.out_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.mean, l.variance, l.mean_delta, l.variance_delta, l.batch, l.out_c, l.out_w*l.out_h, l.delta);
    જો(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ifdef GPU

શૂન્ય pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_mean_gpu, l.rolling_mean, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
શૂન્ય ધકેલો_batchnorm_layer(layer l)
{
    cuda_ધકેલો_array(l.scales_gpu, l.scales, l.c);
    cuda_ધકેલો_array(l.rolling_mean_gpu, l.rolling_mean, l.c);
    cuda_ધકેલો_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

શૂન્ય forward_batchnorm_layer_gpu(layer l, network net)
{
    જો(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.આગત_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    જો (net.train) {
#ifdef CUDNN
        દશાંશ one = 1;
        દશાંશ zero = 0;
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
#નહીંતર
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
    } નહીંતર {
        normalize_gpu(l.output_gpu, l.rolling_mean_gpu, l.rolling_variance_gpu, l.batch, l.out_c, l.out_h*l.out_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.out_c, l.out_h*l.out_w);
        add_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.out_c, l.out_w*l.out_h);
    }

}

શૂન્ય backward_batchnorm_layer_gpu(layer l, network net)
{
    જો(!net.train){
        l.mean_gpu = l.rolling_mean_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#ifdef CUDNN
    દશાંશ one = 1;
    દશાંશ zero = 0;
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
#નહીંતર
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.out_c, l.out_w*l.out_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.out_c, l.out_h*l.out_w);

    fast_mean_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.mean_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.mean_gpu, l.variance_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.mean_gpu, l.variance_gpu, l.mean_delta_gpu, l.variance_delta_gpu, l.batch, l.out_c, l.out_w*l.out_h, l.delta_gpu);
#endif
    જો(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
