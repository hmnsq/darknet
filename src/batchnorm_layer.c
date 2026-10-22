#ਅਨ੍ਤਰ੍ਭਾਵ "convolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "batchnorm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

layer make_batchnorm_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.ਬਾਹਰਲਾ_h = h;
    l.w = l.ਬਾਹਰਲਾ_w = w;
    l.c = l.ਬਾਹਰਲਾ_c = c;
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(h * w * c * batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta  = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(h * w * c * batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.scale_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.biases = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.bias_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.ਔਸਤ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.variance = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.rolling_ਔਸਤ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.rolling_variance = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_batchnorm_layer;
    l.backward = backward_batchnorm_layer;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_batchnorm_layer_gpu;
    l.backward_gpu = backward_batchnorm_layer_gpu;

    l.output_gpu =  cuda_make_array(l.output, h * w * c * batch);
    l.delta_gpu =   cuda_make_array(l.delta, h * w * c * batch);

    l.biases_gpu = cuda_make_array(l.biases, c);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, c);

    l.scales_gpu = cuda_make_array(l.scales, c);
    l.scale_updates_gpu = cuda_make_array(l.scale_updates, c);

    l.ਔਸਤ_gpu = cuda_make_array(l.ਔਸਤ, c);
    l.variance_gpu = cuda_make_array(l.variance, c);

    l.rolling_ਔਸਤ_gpu = cuda_make_array(l.ਔਸਤ, c);
    l.rolling_variance_gpu = cuda_make_array(l.variance, c);

    l.ਔਸਤ_delta_gpu = cuda_make_array(l.ਔਸਤ, c);
    l.variance_delta_gpu = cuda_make_array(l.variance, c);

    l.x_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    l.x_norm_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
    cudnnCreateTensorDescriptor(&l.normTensorDesc);
    cudnnCreateTensorDescriptor(&l.dstTensorDesc);
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, 1, l.ਬਾਹਰਲਾ_c, 1, 1); 

    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ backward_scale_cpu(ਦਸ਼ਮਲਵ *x_norm, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ *scale_updates)
{
    ਪੂਰਨ i,b,f;
    ਲਈ(f = 0; f < n; ++f){
        ਦਸ਼ਮਲਵ ਜੋੜ = 0;
        ਲਈ(b = 0; b < batch; ++b){
            ਲਈ(i = 0; i < ਮਾਪ; ++i){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = i + ਮਾਪ*(f + n*b);
                ਜੋੜ += delta[ਸੂਚਕਾਂਕ] * x_norm[ਸੂਚਕਾਂਕ];
            }
        }
        scale_updates[f] += ਜੋੜ;
    }
}

ਖਾਲੀ ਔਸਤ_delta_cpu(ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *variance, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *ਔਸਤ_delta)
{

    ਪੂਰਨ i,j,k;
    ਲਈ(i = 0; i < filters; ++i){
        ਔਸਤ_delta[i] = 0;
        ਲਈ (j = 0; j < batch; ++j) {
            ਲਈ (k = 0; k < spatial; ++k) {
                ਪੂਰਨ ਸੂਚਕਾਂਕ = j*filters*spatial + i*spatial + k;
                ਔਸਤ_delta[i] += delta[ਸੂਚਕਾਂਕ];
            }
        }
        ਔਸਤ_delta[i] *= (-1./ਵਰਗਮੂਲ(variance[i] + .00001f));
    }
}
ਖਾਲੀ  variance_delta_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *delta, ਦਸ਼ਮਲਵ *ਔਸਤ, ਦਸ਼ਮਲਵ *variance, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *variance_delta)
{

    ਪੂਰਨ i,j,k;
    ਲਈ(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        ਲਈ(j = 0; j < batch; ++j){
            ਲਈ(k = 0; k < spatial; ++k){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[ਸੂਚਕਾਂਕ]*(x[ਸੂਚਕਾਂਕ] - ਔਸਤ[i]);
            }
        }
        variance_delta[i] *= -.5 * ਘਾਤ(variance[i] + .00001f, (ਦਸ਼ਮਲਵ)(-3./2.));
    }
}
ਖਾਲੀ normalize_delta_cpu(ਦਸ਼ਮਲਵ *x, ਦਸ਼ਮਲਵ *ਔਸਤ, ਦਸ਼ਮਲਵ *variance, ਦਸ਼ਮਲਵ *ਔਸਤ_delta, ਦਸ਼ਮਲਵ *variance_delta, ਪੂਰਨ batch, ਪੂਰਨ filters, ਪੂਰਨ spatial, ਦਸ਼ਮਲਵ *delta)
{
    ਪੂਰਨ f, j, k;
    ਲਈ(j = 0; j < batch; ++j){
        ਲਈ(f = 0; f < filters; ++f){
            ਲਈ(k = 0; k < spatial; ++k){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = j*filters*spatial + f*spatial + k;
                delta[ਸੂਚਕਾਂਕ] = delta[ਸੂਚਕਾਂਕ] * 1./(ਵਰਗਮੂਲ(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[ਸੂਚਕਾਂਕ] - ਔਸਤ[f]) / (spatial * batch) + ਔਸਤ_delta[f]/(spatial*batch);
            }
        }
    }
}

ਖਾਲੀ resize_batchnorm_layer(layer *layer, ਪੂਰਨ w, ਪੂਰਨ h)
{
    fprintf(stderr, "Not implemented\n");
}

ਖਾਲੀ forward_batchnorm_layer(layer l, ਜਾਲ net)
{
    ਜੇ(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    ਜੇ(net.train){
        ਔਸਤ_cpu(l.output, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w, l.ਔਸਤ);
        variance_cpu(l.output, l.ਔਸਤ, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w, l.variance);

        scal_cpu(l.ਬਾਹਰਲਾ_c, .99, l.rolling_ਔਸਤ, 1);
        axpy_cpu(l.ਬਾਹਰਲਾ_c, .01, l.ਔਸਤ, 1, l.rolling_ਔਸਤ, 1);
        scal_cpu(l.ਬਾਹਰਲਾ_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.ਬਾਹਰਲਾ_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.ਔਸਤ, l.variance, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } ਨਹੀਂਤਾਂ {
        normalize_cpu(l.output, l.rolling_ਔਸਤ, l.rolling_variance, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);
    ਜੋੜੋ_bias(l.output, l.biases, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);
}

ਖਾਲੀ backward_batchnorm_layer(layer l, ਜਾਲ net)
{
    ਜੇ(!net.train){
        l.ਔਸਤ = l.rolling_ਔਸਤ;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);

    ਔਸਤ_delta_cpu(l.delta, l.variance, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.ਔਸਤ_delta);
    variance_delta_cpu(l.x, l.delta, l.ਔਸਤ, l.variance, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.ਔਸਤ, l.variance, l.ਔਸਤ_delta, l.variance_delta, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.delta);
    ਜੇ(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU

ਖਾਲੀ pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_ਔਸਤ_gpu, l.rolling_ਔਸਤ, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
ਖਾਲੀ ਧੱਕੋ_batchnorm_layer(layer l)
{
    cuda_ਧੱਕੋ_array(l.scales_gpu, l.scales, l.c);
    cuda_ਧੱਕੋ_array(l.rolling_ਔਸਤ_gpu, l.rolling_ਔਸਤ, l.c);
    cuda_ਧੱਕੋ_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

ਖਾਲੀ forward_batchnorm_layer_gpu(layer l, ਜਾਲ net)
{
    ਜੇ(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    ਜੇ (net.train) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
        ਦਸ਼ਮਲਵ one = 1;
        ਦਸ਼ਮਲਵ zero = 0;
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
                l.rolling_ਔਸਤ_gpu,
                l.rolling_variance_gpu,
                .00001,
                l.ਔਸਤ_gpu,
                l.variance_gpu);
#ਨਹੀਂਤਾਂ
        fast_ਔਸਤ_gpu(l.output_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w, l.ਔਸਤ_gpu);
        fast_variance_gpu(l.output_gpu, l.ਔਸਤ_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w, l.variance_gpu);

        scal_gpu(l.ਬਾਹਰਲਾ_c, .99, l.rolling_ਔਸਤ_gpu, 1);
        axpy_gpu(l.ਬਾਹਰਲਾ_c, .01, l.ਔਸਤ_gpu, 1, l.rolling_ਔਸਤ_gpu, 1);
        scal_gpu(l.ਬਾਹਰਲਾ_c, .99, l.rolling_variance_gpu, 1);
        axpy_gpu(l.ਬਾਹਰਲਾ_c, .01, l.variance_gpu, 1, l.rolling_variance_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
        normalize_gpu(l.output_gpu, l.ਔਸਤ_gpu, l.variance_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_norm_gpu, 1);

        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);
        ਜੋੜੋ_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h);
#ਜੇ_ਅੰਤ
    } ਨਹੀਂਤਾਂ {
        normalize_gpu(l.output_gpu, l.rolling_ਔਸਤ_gpu, l.rolling_variance_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);
        ਜੋੜੋ_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h);
    }

}

ਖਾਲੀ backward_batchnorm_layer_gpu(layer l, ਜਾਲ net)
{
    ਜੇ(!net.train){
        l.ਔਸਤ_gpu = l.rolling_ਔਸਤ_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
    ਦਸ਼ਮਲਵ one = 1;
    ਦਸ਼ਮਲਵ zero = 0;
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
            l.ਔਸਤ_gpu,
            l.variance_gpu);
    copy_gpu(l.outputs*l.batch, l.x_norm_gpu, 1, l.delta_gpu, 1);
#ਨਹੀਂਤਾਂ
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);

    fast_ਔਸਤ_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.ਔਸਤ_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.ਔਸਤ_gpu, l.variance_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.ਔਸਤ_gpu, l.variance_gpu, l.ਔਸਤ_delta_gpu, l.variance_delta_gpu, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.delta_gpu);
#ਜੇ_ਅੰਤ
    ਜੇ(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ਜੇ_ਅੰਤ
