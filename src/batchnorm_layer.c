#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "batchnorm_layer.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव <stdio.h>

layer make_batchnorm_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.बहिर्गमनम्_h = h;
    l.w = l.बहिर्गमनम्_w = w;
    l.c = l.बहिर्गमनम्_c = c;
    l.output = शून्यस्मृतिं_देहि(h * w * c * batch, परिमाण(भिन्न));
    l.delta  = शून्यस्मृतिं_देहि(h * w * c * batch, परिमाण(भिन्न));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = शून्यस्मृतिं_देहि(c, परिमाण(भिन्न));
    l.scale_updates = शून्यस्मृतिं_देहि(c, परिमाण(भिन्न));
    l.biases = शून्यस्मृतिं_देहि(c, परिमाण(भिन्न));
    l.bias_updates = शून्यस्मृतिं_देहि(c, परिमाण(भिन्न));
    पूर्णाङ्क i;
    प्रति(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.माध्यम् = शून्यस्मृतिं_देहि(c, परिमाण(भिन्न));
    l.variance = शून्यस्मृतिं_देहि(c, परिमाण(भिन्न));

    l.rolling_माध्यम् = शून्यस्मृतिं_देहि(c, परिमाण(भिन्न));
    l.rolling_variance = शून्यस्मृतिं_देहि(c, परिमाण(भिन्न));

    l.forward = forward_batchnorm_layer;
    l.backward = backward_batchnorm_layer;
#यदि_परिभाषितम् GPU
    l.forward_gpu = forward_batchnorm_layer_gpu;
    l.backward_gpu = backward_batchnorm_layer_gpu;

    l.output_gpu =  cuda_make_array(l.output, h * w * c * batch);
    l.delta_gpu =   cuda_make_array(l.delta, h * w * c * batch);

    l.biases_gpu = cuda_make_array(l.biases, c);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, c);

    l.scales_gpu = cuda_make_array(l.scales, c);
    l.scale_updates_gpu = cuda_make_array(l.scale_updates, c);

    l.माध्यम्_gpu = cuda_make_array(l.माध्यम्, c);
    l.variance_gpu = cuda_make_array(l.variance, c);

    l.rolling_माध्यम्_gpu = cuda_make_array(l.माध्यम्, c);
    l.rolling_variance_gpu = cuda_make_array(l.variance, c);

    l.माध्यम्_delta_gpu = cuda_make_array(l.माध्यम्, c);
    l.variance_delta_gpu = cuda_make_array(l.variance, c);

    l.x_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    l.x_norm_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    #यदि_परिभाषितम् CUDNN
    cudnnCreateTensorDescriptor(&l.normTensorDesc);
    cudnnCreateTensorDescriptor(&l.dstTensorDesc);
    cudnn_समुच्चयः_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h, l.बहिर्गमनम्_w); 
    cudnn_समुच्चयः_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, 1, l.बहिर्गमनम्_c, 1, 1); 

    #यदि_अन्तः
#यदि_अन्तः
    प्रत्यागम l;
}

शून्य backward_scale_cpu(भिन्न *x_norm, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क आकार, भिन्न *scale_updates)
{
    पूर्णाङ्क i,b,f;
    प्रति(f = 0; f < n; ++f){
        भिन्न योग = 0;
        प्रति(b = 0; b < batch; ++b){
            प्रति(i = 0; i < आकार; ++i){
                पूर्णाङ्क सूचकांक = i + आकार*(f + n*b);
                योग += delta[सूचकांक] * x_norm[सूचकांक];
            }
        }
        scale_updates[f] += योग;
    }
}

शून्य माध्यम्_delta_cpu(भिन्न *delta, भिन्न *variance, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *माध्यम्_delta)
{

    पूर्णाङ्क i,j,k;
    प्रति(i = 0; i < filters; ++i){
        माध्यम्_delta[i] = 0;
        प्रति (j = 0; j < batch; ++j) {
            प्रति (k = 0; k < spatial; ++k) {
                पूर्णाङ्क सूचकांक = j*filters*spatial + i*spatial + k;
                माध्यम्_delta[i] += delta[सूचकांक];
            }
        }
        माध्यम्_delta[i] *= (-1./वर्गमूलम्(variance[i] + .00001f));
    }
}
शून्य  variance_delta_cpu(भिन्न *x, भिन्न *delta, भिन्न *माध्यम्, भिन्न *variance, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *variance_delta)
{

    पूर्णाङ्क i,j,k;
    प्रति(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        प्रति(j = 0; j < batch; ++j){
            प्रति(k = 0; k < spatial; ++k){
                पूर्णाङ्क सूचकांक = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[सूचकांक]*(x[सूचकांक] - माध्यम्[i]);
            }
        }
        variance_delta[i] *= -.5 * घातः(variance[i] + .00001f, (भिन्न)(-3./2.));
    }
}
शून्य normalize_delta_cpu(भिन्न *x, भिन्न *माध्यम्, भिन्न *variance, भिन्न *माध्यम्_delta, भिन्न *variance_delta, पूर्णाङ्क batch, पूर्णाङ्क filters, पूर्णाङ्क spatial, भिन्न *delta)
{
    पूर्णाङ्क f, j, k;
    प्रति(j = 0; j < batch; ++j){
        प्रति(f = 0; f < filters; ++f){
            प्रति(k = 0; k < spatial; ++k){
                पूर्णाङ्क सूचकांक = j*filters*spatial + f*spatial + k;
                delta[सूचकांक] = delta[सूचकांक] * 1./(वर्गमूलम्(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[सूचकांक] - माध्यम्[f]) / (spatial * batch) + माध्यम्_delta[f]/(spatial*batch);
            }
        }
    }
}

शून्य resize_batchnorm_layer(layer *layer, पूर्णाङ्क w, पूर्णाङ्क h)
{
    fprintf(stderr, "Not implemented\n");
}

शून्य forward_batchnorm_layer(layer l, जालम् net)
{
    यदि(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.निवेशः, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    यदि(net.train){
        माध्यम्_cpu(l.output, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w, l.माध्यम्);
        variance_cpu(l.output, l.माध्यम्, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w, l.variance);

        scal_cpu(l.बहिर्गमनम्_c, .99, l.rolling_माध्यम्, 1);
        axpy_cpu(l.बहिर्गमनम्_c, .01, l.माध्यम्, 1, l.rolling_माध्यम्, 1);
        scal_cpu(l.बहिर्गमनम्_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.बहिर्गमनम्_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.माध्यम्, l.variance, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } अन्यथा {
        normalize_cpu(l.output, l.rolling_माध्यम्, l.rolling_variance, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);
    सङ्कलय_bias(l.output, l.biases, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);
}

शून्य backward_batchnorm_layer(layer l, जालम् net)
{
    यदि(!net.train){
        l.माध्यम् = l.rolling_माध्यम्;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);

    माध्यम्_delta_cpu(l.delta, l.variance, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h, l.माध्यम्_delta);
    variance_delta_cpu(l.x, l.delta, l.माध्यम्, l.variance, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.माध्यम्, l.variance, l.माध्यम्_delta, l.variance_delta, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h, l.delta);
    यदि(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#यदि_परिभाषितम् GPU

शून्य pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_माध्यम्_gpu, l.rolling_माध्यम्, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
शून्य प्रक्षिप_batchnorm_layer(layer l)
{
    cuda_प्रक्षिप_array(l.scales_gpu, l.scales, l.c);
    cuda_प्रक्षिप_array(l.rolling_माध्यम्_gpu, l.rolling_माध्यम्, l.c);
    cuda_प्रक्षिप_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

शून्य forward_batchnorm_layer_gpu(layer l, जालम् net)
{
    यदि(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.निवेशः_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    यदि (net.train) {
#यदि_परिभाषितम् CUDNN
        भिन्न one = 1;
        भिन्न zero = 0;
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
                l.rolling_माध्यम्_gpu,
                l.rolling_variance_gpu,
                .00001,
                l.माध्यम्_gpu,
                l.variance_gpu);
#अन्यथा
        fast_माध्यम्_gpu(l.output_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w, l.माध्यम्_gpu);
        fast_variance_gpu(l.output_gpu, l.माध्यम्_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w, l.variance_gpu);

        scal_gpu(l.बहिर्गमनम्_c, .99, l.rolling_माध्यम्_gpu, 1);
        axpy_gpu(l.बहिर्गमनम्_c, .01, l.माध्यम्_gpu, 1, l.rolling_माध्यम्_gpu, 1);
        scal_gpu(l.बहिर्गमनम्_c, .99, l.rolling_variance_gpu, 1);
        axpy_gpu(l.बहिर्गमनम्_c, .01, l.variance_gpu, 1, l.rolling_variance_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
        normalize_gpu(l.output_gpu, l.माध्यम्_gpu, l.variance_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_norm_gpu, 1);

        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);
        सङ्कलय_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h);
#यदि_अन्तः
    } अन्यथा {
        normalize_gpu(l.output_gpu, l.rolling_माध्यम्_gpu, l.rolling_variance_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);
        सङ्कलय_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h);
    }

}

शून्य backward_batchnorm_layer_gpu(layer l, जालम् net)
{
    यदि(!net.train){
        l.माध्यम्_gpu = l.rolling_माध्यम्_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#यदि_परिभाषितम् CUDNN
    भिन्न one = 1;
    भिन्न zero = 0;
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
            l.माध्यम्_gpu,
            l.variance_gpu);
    copy_gpu(l.outputs*l.batch, l.x_norm_gpu, 1, l.delta_gpu, 1);
#अन्यथा
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);

    fast_माध्यम्_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h, l.माध्यम्_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.माध्यम्_gpu, l.variance_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.माध्यम्_gpu, l.variance_gpu, l.माध्यम्_delta_gpu, l.variance_delta_gpu, l.batch, l.बहिर्गमनम्_c, l.बहिर्गमनम्_w*l.बहिर्गमनम्_h, l.delta_gpu);
#यदि_अन्तः
    यदि(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#यदि_अन्तः
