#شامل "convolutional_layer.h"
#شامل "batchnorm_layer.h"
#شامل "blas.h"
#شامل <stdio.h>

layer make_batchnorm_layer(صحیح batch, صحیح w, صحیح h, صحیح c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.برآمد_h = h;
    l.w = l.برآمد_w = w;
    l.c = l.برآمد_c = c;
    l.output = صفر_حافظہ_دو(h * w * c * batch, حجم(اعشاری));
    l.delta  = صفر_حافظہ_دو(h * w * c * batch, حجم(اعشاری));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = صفر_حافظہ_دو(c, حجم(اعشاری));
    l.scale_updates = صفر_حافظہ_دو(c, حجم(اعشاری));
    l.biases = صفر_حافظہ_دو(c, حجم(اعشاری));
    l.bias_updates = صفر_حافظہ_دو(c, حجم(اعشاری));
    صحیح i;
    برائے(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.اوسط = صفر_حافظہ_دو(c, حجم(اعشاری));
    l.variance = صفر_حافظہ_دو(c, حجم(اعشاری));

    l.rolling_اوسط = صفر_حافظہ_دو(c, حجم(اعشاری));
    l.rolling_variance = صفر_حافظہ_دو(c, حجم(اعشاری));

    l.forward = forward_batchnorm_layer;
    l.backward = backward_batchnorm_layer;
#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_batchnorm_layer_gpu;
    l.backward_gpu = backward_batchnorm_layer_gpu;

    l.output_gpu =  cuda_make_array(l.output, h * w * c * batch);
    l.delta_gpu =   cuda_make_array(l.delta, h * w * c * batch);

    l.biases_gpu = cuda_make_array(l.biases, c);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, c);

    l.scales_gpu = cuda_make_array(l.scales, c);
    l.scale_updates_gpu = cuda_make_array(l.scale_updates, c);

    l.اوسط_gpu = cuda_make_array(l.اوسط, c);
    l.variance_gpu = cuda_make_array(l.variance, c);

    l.rolling_اوسط_gpu = cuda_make_array(l.اوسط, c);
    l.rolling_variance_gpu = cuda_make_array(l.variance, c);

    l.اوسط_delta_gpu = cuda_make_array(l.اوسط, c);
    l.variance_delta_gpu = cuda_make_array(l.variance, c);

    l.x_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    l.x_norm_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    #اگر_تعریف_شدہ CUDNN
    cudnnCreateTensorDescriptor(&l.normTensorDesc);
    cudnnCreateTensorDescriptor(&l.dstTensorDesc);
    cudnn_سیٹ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, l.batch, l.برآمد_c, l.برآمد_h, l.برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, 1, l.برآمد_c, 1, 1); 

    #اگر_ختم
#اگر_ختم
    واپس l;
}

خالی backward_scale_cpu(اعشاری *x_norm, اعشاری *delta, صحیح batch, صحیح n, صحیح سائز, اعشاری *scale_updates)
{
    صحیح i,b,f;
    برائے(f = 0; f < n; ++f){
        اعشاری مجموع = 0;
        برائے(b = 0; b < batch; ++b){
            برائے(i = 0; i < سائز; ++i){
                صحیح اشاریہ = i + سائز*(f + n*b);
                مجموع += delta[اشاریہ] * x_norm[اشاریہ];
            }
        }
        scale_updates[f] += مجموع;
    }
}

خالی اوسط_delta_cpu(اعشاری *delta, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *اوسط_delta)
{

    صحیح i,j,k;
    برائے(i = 0; i < filters; ++i){
        اوسط_delta[i] = 0;
        برائے (j = 0; j < batch; ++j) {
            برائے (k = 0; k < spatial; ++k) {
                صحیح اشاریہ = j*filters*spatial + i*spatial + k;
                اوسط_delta[i] += delta[اشاریہ];
            }
        }
        اوسط_delta[i] *= (-1./جذر(variance[i] + .00001f));
    }
}
خالی  variance_delta_cpu(اعشاری *x, اعشاری *delta, اعشاری *اوسط, اعشاری *variance, صحیح batch, صحیح filters, صحیح spatial, اعشاری *variance_delta)
{

    صحیح i,j,k;
    برائے(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        برائے(j = 0; j < batch; ++j){
            برائے(k = 0; k < spatial; ++k){
                صحیح اشاریہ = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[اشاریہ]*(x[اشاریہ] - اوسط[i]);
            }
        }
        variance_delta[i] *= -.5 * قوت(variance[i] + .00001f, (اعشاری)(-3./2.));
    }
}
خالی normalize_delta_cpu(اعشاری *x, اعشاری *اوسط, اعشاری *variance, اعشاری *اوسط_delta, اعشاری *variance_delta, صحیح batch, صحیح filters, صحیح spatial, اعشاری *delta)
{
    صحیح f, j, k;
    برائے(j = 0; j < batch; ++j){
        برائے(f = 0; f < filters; ++f){
            برائے(k = 0; k < spatial; ++k){
                صحیح اشاریہ = j*filters*spatial + f*spatial + k;
                delta[اشاریہ] = delta[اشاریہ] * 1./(جذر(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[اشاریہ] - اوسط[f]) / (spatial * batch) + اوسط_delta[f]/(spatial*batch);
            }
        }
    }
}

خالی resize_batchnorm_layer(layer *layer, صحیح w, صحیح h)
{
    fprintf(stderr, "Not implemented\n");
}

خالی forward_batchnorm_layer(layer l, جال net)
{
    اگر(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.داخلہ, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    اگر(net.train){
        اوسط_cpu(l.output, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w, l.اوسط);
        variance_cpu(l.output, l.اوسط, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w, l.variance);

        scal_cpu(l.برآمد_c, .99, l.rolling_اوسط, 1);
        axpy_cpu(l.برآمد_c, .01, l.اوسط, 1, l.rolling_اوسط, 1);
        scal_cpu(l.برآمد_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.برآمد_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.اوسط, l.variance, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } ورنہ {
        normalize_cpu(l.output, l.rolling_اوسط, l.rolling_variance, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);
    جوڑو_bias(l.output, l.biases, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);
}

خالی backward_batchnorm_layer(layer l, جال net)
{
    اگر(!net.train){
        l.اوسط = l.rolling_اوسط;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);

    اوسط_delta_cpu(l.delta, l.variance, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h, l.اوسط_delta);
    variance_delta_cpu(l.x, l.delta, l.اوسط, l.variance, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.اوسط, l.variance, l.اوسط_delta, l.variance_delta, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h, l.delta);
    اگر(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#اگر_تعریف_شدہ GPU

خالی pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_اوسط_gpu, l.rolling_اوسط, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
خالی ڈالو_batchnorm_layer(layer l)
{
    cuda_ڈالو_array(l.scales_gpu, l.scales, l.c);
    cuda_ڈالو_array(l.rolling_اوسط_gpu, l.rolling_اوسط, l.c);
    cuda_ڈالو_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

خالی forward_batchnorm_layer_gpu(layer l, جال net)
{
    اگر(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.داخلہ_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    اگر (net.train) {
#اگر_تعریف_شدہ CUDNN
        اعشاری one = 1;
        اعشاری zero = 0;
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
                l.rolling_اوسط_gpu,
                l.rolling_variance_gpu,
                .00001,
                l.اوسط_gpu,
                l.variance_gpu);
#ورنہ
        fast_اوسط_gpu(l.output_gpu, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w, l.اوسط_gpu);
        fast_variance_gpu(l.output_gpu, l.اوسط_gpu, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w, l.variance_gpu);

        scal_gpu(l.برآمد_c, .99, l.rolling_اوسط_gpu, 1);
        axpy_gpu(l.برآمد_c, .01, l.اوسط_gpu, 1, l.rolling_اوسط_gpu, 1);
        scal_gpu(l.برآمد_c, .99, l.rolling_variance_gpu, 1);
        axpy_gpu(l.برآمد_c, .01, l.variance_gpu, 1, l.rolling_variance_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
        normalize_gpu(l.output_gpu, l.اوسط_gpu, l.variance_gpu, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_norm_gpu, 1);

        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);
        جوڑو_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h);
#اگر_ختم
    } ورنہ {
        normalize_gpu(l.output_gpu, l.rolling_اوسط_gpu, l.rolling_variance_gpu, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);
        جوڑو_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h);
    }

}

خالی backward_batchnorm_layer_gpu(layer l, جال net)
{
    اگر(!net.train){
        l.اوسط_gpu = l.rolling_اوسط_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#اگر_تعریف_شدہ CUDNN
    اعشاری one = 1;
    اعشاری zero = 0;
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
            l.اوسط_gpu,
            l.variance_gpu);
    copy_gpu(l.outputs*l.batch, l.x_norm_gpu, 1, l.delta_gpu, 1);
#ورنہ
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.برآمد_c, l.برآمد_h*l.برآمد_w);

    fast_اوسط_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h, l.اوسط_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.اوسط_gpu, l.variance_gpu, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.اوسط_gpu, l.variance_gpu, l.اوسط_delta_gpu, l.variance_delta_gpu, l.batch, l.برآمد_c, l.برآمد_w*l.برآمد_h, l.delta_gpu);
#اگر_ختم
    اگر(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#اگر_ختم
