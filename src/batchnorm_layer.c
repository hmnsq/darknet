#అన్తర్భావ "convolutional_layer.h"
#అన్తర్భావ "batchnorm_layer.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ <stdio.h>

layer make_batchnorm_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.బహిర్గతం_h = h;
    l.w = l.బహిర్గతం_w = w;
    l.c = l.బహిర్గతం_c = c;
    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(h * w * c * batch, పరిమాణం(దశాంశం));
    l.delta  = సున్నా_జ్ఞాపకం_ఇవ్వు(h * w * c * batch, పరిమాణం(దశాంశం));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = సున్నా_జ్ఞాపకం_ఇవ్వు(c, పరిమాణం(దశాంశం));
    l.scale_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(c, పరిమాణం(దశాంశం));
    l.biases = సున్నా_జ్ఞాపకం_ఇవ్వు(c, పరిమాణం(దశాంశం));
    l.bias_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(c, పరిమాణం(దశాంశం));
    పూర్ణాంకం i;
    కొరకు(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.సగటు = సున్నా_జ్ఞాపకం_ఇవ్వు(c, పరిమాణం(దశాంశం));
    l.variance = సున్నా_జ్ఞాపకం_ఇవ్వు(c, పరిమాణం(దశాంశం));

    l.rolling_సగటు = సున్నా_జ్ఞాపకం_ఇవ్వు(c, పరిమాణం(దశాంశం));
    l.rolling_variance = సున్నా_జ్ఞాపకం_ఇవ్వు(c, పరిమాణం(దశాంశం));

    l.forward = forward_batchnorm_layer;
    l.backward = backward_batchnorm_layer;
#నిర్వచితమైతే GPU
    l.forward_gpu = forward_batchnorm_layer_gpu;
    l.backward_gpu = backward_batchnorm_layer_gpu;

    l.output_gpu =  cuda_make_array(l.output, h * w * c * batch);
    l.delta_gpu =   cuda_make_array(l.delta, h * w * c * batch);

    l.biases_gpu = cuda_make_array(l.biases, c);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, c);

    l.scales_gpu = cuda_make_array(l.scales, c);
    l.scale_updates_gpu = cuda_make_array(l.scale_updates, c);

    l.సగటు_gpu = cuda_make_array(l.సగటు, c);
    l.variance_gpu = cuda_make_array(l.variance, c);

    l.rolling_సగటు_gpu = cuda_make_array(l.సగటు, c);
    l.rolling_variance_gpu = cuda_make_array(l.variance, c);

    l.సగటు_delta_gpu = cuda_make_array(l.సగటు, c);
    l.variance_delta_gpu = cuda_make_array(l.variance, c);

    l.x_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    l.x_norm_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    #నిర్వచితమైతే CUDNN
    cudnnCreateTensorDescriptor(&l.normTensorDesc);
    cudnnCreateTensorDescriptor(&l.dstTensorDesc);
    cudnn_సమితి_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h, l.బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, 1, l.బహిర్గతం_c, 1, 1); 

    #అయితే_ముగింపు
#అయితే_ముగింపు
    తిరిగి l;
}

శూన్య backward_scale_cpu(దశాంశం *x_norm, దశాంశం *delta, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత, దశాంశం *scale_updates)
{
    పూర్ణాంకం i,b,f;
    కొరకు(f = 0; f < n; ++f){
        దశాంశం కూడిక = 0;
        కొరకు(b = 0; b < batch; ++b){
            కొరకు(i = 0; i < కొలత; ++i){
                పూర్ణాంకం సూచీ = i + కొలత*(f + n*b);
                కూడిక += delta[సూచీ] * x_norm[సూచీ];
            }
        }
        scale_updates[f] += కూడిక;
    }
}

శూన్య సగటు_delta_cpu(దశాంశం *delta, దశాంశం *variance, పూర్ణాంకం batch, పూర్ణాంకం filters, పూర్ణాంకం spatial, దశాంశం *సగటు_delta)
{

    పూర్ణాంకం i,j,k;
    కొరకు(i = 0; i < filters; ++i){
        సగటు_delta[i] = 0;
        కొరకు (j = 0; j < batch; ++j) {
            కొరకు (k = 0; k < spatial; ++k) {
                పూర్ణాంకం సూచీ = j*filters*spatial + i*spatial + k;
                సగటు_delta[i] += delta[సూచీ];
            }
        }
        సగటు_delta[i] *= (-1./వర్గమూలం(variance[i] + .00001f));
    }
}
శూన్య  variance_delta_cpu(దశాంశం *x, దశాంశం *delta, దశాంశం *సగటు, దశాంశం *variance, పూర్ణాంకం batch, పూర్ణాంకం filters, పూర్ణాంకం spatial, దశాంశం *variance_delta)
{

    పూర్ణాంకం i,j,k;
    కొరకు(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        కొరకు(j = 0; j < batch; ++j){
            కొరకు(k = 0; k < spatial; ++k){
                పూర్ణాంకం సూచీ = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[సూచీ]*(x[సూచీ] - సగటు[i]);
            }
        }
        variance_delta[i] *= -.5 * ఘాతం(variance[i] + .00001f, (దశాంశం)(-3./2.));
    }
}
శూన్య normalize_delta_cpu(దశాంశం *x, దశాంశం *సగటు, దశాంశం *variance, దశాంశం *సగటు_delta, దశాంశం *variance_delta, పూర్ణాంకం batch, పూర్ణాంకం filters, పూర్ణాంకం spatial, దశాంశం *delta)
{
    పూర్ణాంకం f, j, k;
    కొరకు(j = 0; j < batch; ++j){
        కొరకు(f = 0; f < filters; ++f){
            కొరకు(k = 0; k < spatial; ++k){
                పూర్ణాంకం సూచీ = j*filters*spatial + f*spatial + k;
                delta[సూచీ] = delta[సూచీ] * 1./(వర్గమూలం(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[సూచీ] - సగటు[f]) / (spatial * batch) + సగటు_delta[f]/(spatial*batch);
            }
        }
    }
}

శూన్య resize_batchnorm_layer(layer *layer, పూర్ణాంకం w, పూర్ణాంకం h)
{
    fprintf(stderr, "Not implemented\n");
}

శూన్య forward_batchnorm_layer(layer l, జాలం net)
{
    ఒకవేళ(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.ఆదానం, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    ఒకవేళ(net.train){
        సగటు_cpu(l.output, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w, l.సగటు);
        variance_cpu(l.output, l.సగటు, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w, l.variance);

        scal_cpu(l.బహిర్గతం_c, .99, l.rolling_సగటు, 1);
        axpy_cpu(l.బహిర్గతం_c, .01, l.సగటు, 1, l.rolling_సగటు, 1);
        scal_cpu(l.బహిర్గతం_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.బహిర్గతం_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.సగటు, l.variance, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } లేకపోతే {
        normalize_cpu(l.output, l.rolling_సగటు, l.rolling_variance, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);
    కూడు_bias(l.output, l.biases, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);
}

శూన్య backward_batchnorm_layer(layer l, జాలం net)
{
    ఒకవేళ(!net.train){
        l.సగటు = l.rolling_సగటు;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);

    సగటు_delta_cpu(l.delta, l.variance, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h, l.సగటు_delta);
    variance_delta_cpu(l.x, l.delta, l.సగటు, l.variance, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.సగటు, l.variance, l.సగటు_delta, l.variance_delta, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h, l.delta);
    ఒకవేళ(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#నిర్వచితమైతే GPU

శూన్య pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_సగటు_gpu, l.rolling_సగటు, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
శూన్య నెట్టు_batchnorm_layer(layer l)
{
    cuda_నెట్టు_array(l.scales_gpu, l.scales, l.c);
    cuda_నెట్టు_array(l.rolling_సగటు_gpu, l.rolling_సగటు, l.c);
    cuda_నెట్టు_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

శూన్య forward_batchnorm_layer_gpu(layer l, జాలం net)
{
    ఒకవేళ(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.ఆదానం_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    ఒకవేళ (net.train) {
#నిర్వచితమైతే CUDNN
        దశాంశం one = 1;
        దశాంశం zero = 0;
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
                l.rolling_సగటు_gpu,
                l.rolling_variance_gpu,
                .00001,
                l.సగటు_gpu,
                l.variance_gpu);
#లేకపోతే
        fast_సగటు_gpu(l.output_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w, l.సగటు_gpu);
        fast_variance_gpu(l.output_gpu, l.సగటు_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w, l.variance_gpu);

        scal_gpu(l.బహిర్గతం_c, .99, l.rolling_సగటు_gpu, 1);
        axpy_gpu(l.బహిర్గతం_c, .01, l.సగటు_gpu, 1, l.rolling_సగటు_gpu, 1);
        scal_gpu(l.బహిర్గతం_c, .99, l.rolling_variance_gpu, 1);
        axpy_gpu(l.బహిర్గతం_c, .01, l.variance_gpu, 1, l.rolling_variance_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
        normalize_gpu(l.output_gpu, l.సగటు_gpu, l.variance_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_norm_gpu, 1);

        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);
        కూడు_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h);
#అయితే_ముగింపు
    } లేకపోతే {
        normalize_gpu(l.output_gpu, l.rolling_సగటు_gpu, l.rolling_variance_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);
        కూడు_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h);
    }

}

శూన్య backward_batchnorm_layer_gpu(layer l, జాలం net)
{
    ఒకవేళ(!net.train){
        l.సగటు_gpu = l.rolling_సగటు_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#నిర్వచితమైతే CUDNN
    దశాంశం one = 1;
    దశాంశం zero = 0;
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
            l.సగటు_gpu,
            l.variance_gpu);
    copy_gpu(l.outputs*l.batch, l.x_norm_gpu, 1, l.delta_gpu, 1);
#లేకపోతే
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h*l.బహిర్గతం_w);

    fast_సగటు_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h, l.సగటు_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.సగటు_gpu, l.variance_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.సగటు_gpu, l.variance_gpu, l.సగటు_delta_gpu, l.variance_delta_gpu, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_w*l.బహిర్గతం_h, l.delta_gpu);
#అయితే_ముగింపు
    ఒకవేళ(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#అయితే_ముగింపు
