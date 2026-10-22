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
    l.h = l.ٻاهريون_h = h;
    l.w = l.ٻاهريون_w = w;
    l.c = l.ٻاهريون_c = c;
    l.output = صفر_يادگيري_ڏيو(h * w * c * batch, حجم(عائم));
    l.delta  = صفر_يادگيري_ڏيو(h * w * c * batch, حجم(عائم));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = صفر_يادگيري_ڏيو(c, حجم(عائم));
    l.scale_updates = صفر_يادگيري_ڏيو(c, حجم(عائم));
    l.biases = صفر_يادگيري_ڏيو(c, حجم(عائم));
    l.bias_updates = صفر_يادگيري_ڏيو(c, حجم(عائم));
    صحيح i;
    لكل(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.سراسري = صفر_يادگيري_ڏيو(c, حجم(عائم));
    l.variance = صفر_يادگيري_ڏيو(c, حجم(عائم));

    l.rolling_سراسري = صفر_يادگيري_ڏيو(c, حجم(عائم));
    l.rolling_variance = صفر_يادگيري_ڏيو(c, حجم(عائم));

    l.forward = forward_batchnorm_layer;
    l.backward = backward_batchnorm_layer;
#جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_batchnorm_layer_gpu;
    l.backward_gpu = backward_batchnorm_layer_gpu;

    l.output_gpu =  cuda_make_array(l.output, h * w * c * batch);
    l.delta_gpu =   cuda_make_array(l.delta, h * w * c * batch);

    l.biases_gpu = cuda_make_array(l.biases, c);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, c);

    l.scales_gpu = cuda_make_array(l.scales, c);
    l.scale_updates_gpu = cuda_make_array(l.scale_updates, c);

    l.سراسري_gpu = cuda_make_array(l.سراسري, c);
    l.variance_gpu = cuda_make_array(l.variance, c);

    l.rolling_سراسري_gpu = cuda_make_array(l.سراسري, c);
    l.rolling_variance_gpu = cuda_make_array(l.variance, c);

    l.سراسري_delta_gpu = cuda_make_array(l.سراسري, c);
    l.variance_delta_gpu = cuda_make_array(l.variance, c);

    l.x_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    l.x_norm_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    #جيڪڏهن_وصف_ٿيل CUDNN
    cudnnCreateTensorDescriptor(&l.normTensorDesc);
    cudnnCreateTensorDescriptor(&l.dstTensorDesc);
    cudnn_سيٽ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l.batch, l.ٻاهريون_c, l.ٻاهريون_h, l.ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, 1, l.ٻاهريون_c, 1, 1); 

    #جيڪڏهن_پورو
#جيڪڏهن_پورو
    أرجع l;
}

فراغ backward_scale_cpu(عائم *x_norm, عائم *delta, صحيح batch, صحيح n, صحيح سائيز, عائم *scale_updates)
{
    صحيح i,b,f;
    لكل(f = 0; f < n; ++f){
        عائم جوڙ = 0;
        لكل(b = 0; b < batch; ++b){
            لكل(i = 0; i < سائيز; ++i){
                صحيح اشاريو = i + سائيز*(f + n*b);
                جوڙ += delta[اشاريو] * x_norm[اشاريو];
            }
        }
        scale_updates[f] += جوڙ;
    }
}

فراغ سراسري_delta_cpu(عائم *delta, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *سراسري_delta)
{

    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        سراسري_delta[i] = 0;
        لكل (j = 0; j < batch; ++j) {
            لكل (k = 0; k < spatial; ++k) {
                صحيح اشاريو = j*filters*spatial + i*spatial + k;
                سراسري_delta[i] += delta[اشاريو];
            }
        }
        سراسري_delta[i] *= (-1./جذر(variance[i] + .00001f));
    }
}
فراغ  variance_delta_cpu(عائم *x, عائم *delta, عائم *سراسري, عائم *variance, صحيح batch, صحيح filters, صحيح spatial, عائم *variance_delta)
{

    صحيح i,j,k;
    لكل(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        لكل(j = 0; j < batch; ++j){
            لكل(k = 0; k < spatial; ++k){
                صحيح اشاريو = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[اشاريو]*(x[اشاريو] - سراسري[i]);
            }
        }
        variance_delta[i] *= -.5 * قوت(variance[i] + .00001f, (عائم)(-3./2.));
    }
}
فراغ normalize_delta_cpu(عائم *x, عائم *سراسري, عائم *variance, عائم *سراسري_delta, عائم *variance_delta, صحيح batch, صحيح filters, صحيح spatial, عائم *delta)
{
    صحيح f, j, k;
    لكل(j = 0; j < batch; ++j){
        لكل(f = 0; f < filters; ++f){
            لكل(k = 0; k < spatial; ++k){
                صحيح اشاريو = j*filters*spatial + f*spatial + k;
                delta[اشاريو] = delta[اشاريو] * 1./(جذر(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[اشاريو] - سراسري[f]) / (spatial * batch) + سراسري_delta[f]/(spatial*batch);
            }
        }
    }
}

فراغ resize_batchnorm_layer(layer *layer, صحيح w, صحيح h)
{
    fprintf(stderr, "Not implemented\n");
}

فراغ forward_batchnorm_layer(layer l, ڄار net)
{
    إذا(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.داخلا, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    إذا(net.train){
        سراسري_cpu(l.output, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w, l.سراسري);
        variance_cpu(l.output, l.سراسري, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w, l.variance);

        scal_cpu(l.ٻاهريون_c, .99, l.rolling_سراسري, 1);
        axpy_cpu(l.ٻاهريون_c, .01, l.سراسري, 1, l.rolling_سراسري, 1);
        scal_cpu(l.ٻاهريون_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.ٻاهريون_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.سراسري, l.variance, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } وإلا {
        normalize_cpu(l.output, l.rolling_سراسري, l.rolling_variance, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);
    جوڙيو_bias(l.output, l.biases, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);
}

فراغ backward_batchnorm_layer(layer l, ڄار net)
{
    إذا(!net.train){
        l.سراسري = l.rolling_سراسري;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);

    سراسري_delta_cpu(l.delta, l.variance, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h, l.سراسري_delta);
    variance_delta_cpu(l.x, l.delta, l.سراسري, l.variance, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.سراسري, l.variance, l.سراسري_delta, l.variance_delta, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h, l.delta);
    إذا(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#جيڪڏهن_وصف_ٿيل GPU

فراغ pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_سراسري_gpu, l.rolling_سراسري, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
فراغ ڌڪو_batchnorm_layer(layer l)
{
    cuda_ڌڪو_array(l.scales_gpu, l.scales, l.c);
    cuda_ڌڪو_array(l.rolling_سراسري_gpu, l.rolling_سراسري, l.c);
    cuda_ڌڪو_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

فراغ forward_batchnorm_layer_gpu(layer l, ڄار net)
{
    إذا(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.داخلا_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    إذا (net.train) {
#جيڪڏهن_وصف_ٿيل CUDNN
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
                l.rolling_سراسري_gpu,
                l.rolling_variance_gpu,
                .00001,
                l.سراسري_gpu,
                l.variance_gpu);
#وإلا
        fast_سراسري_gpu(l.output_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w, l.سراسري_gpu);
        fast_variance_gpu(l.output_gpu, l.سراسري_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w, l.variance_gpu);

        scal_gpu(l.ٻاهريون_c, .99, l.rolling_سراسري_gpu, 1);
        axpy_gpu(l.ٻاهريون_c, .01, l.سراسري_gpu, 1, l.rolling_سراسري_gpu, 1);
        scal_gpu(l.ٻاهريون_c, .99, l.rolling_variance_gpu, 1);
        axpy_gpu(l.ٻاهريون_c, .01, l.variance_gpu, 1, l.rolling_variance_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
        normalize_gpu(l.output_gpu, l.سراسري_gpu, l.variance_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_norm_gpu, 1);

        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);
        جوڙيو_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h);
#جيڪڏهن_پورو
    } وإلا {
        normalize_gpu(l.output_gpu, l.rolling_سراسري_gpu, l.rolling_variance_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);
        جوڙيو_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h);
    }

}

فراغ backward_batchnorm_layer_gpu(layer l, ڄار net)
{
    إذا(!net.train){
        l.سراسري_gpu = l.rolling_سراسري_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#جيڪڏهن_وصف_ٿيل CUDNN
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
            l.سراسري_gpu,
            l.variance_gpu);
    copy_gpu(l.outputs*l.batch, l.x_norm_gpu, 1, l.delta_gpu, 1);
#وإلا
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_h*l.ٻاهريون_w);

    fast_سراسري_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h, l.سراسري_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.سراسري_gpu, l.variance_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.سراسري_gpu, l.variance_gpu, l.سراسري_delta_gpu, l.variance_delta_gpu, l.batch, l.ٻاهريون_c, l.ٻاهريون_w*l.ٻاهريون_h, l.delta_gpu);
#جيڪڏهن_پورو
    إذا(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#جيڪڏهن_پورو
