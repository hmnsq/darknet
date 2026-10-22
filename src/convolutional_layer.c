#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "batchnorm_layer.h"
#अन्तर्भाव "im2col.h"
#अन्तर्भाव "col2im.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>

#यदि_परिभाषितम् AI2
#अन्तर्भाव "xnor_layer.h"
#यदि_अन्तः

शून्य swap_द्विमान(convolutional_layer *l)
{
    भिन्न *swap = l->weights;
    l->weights = l->द्विमान_weights;
    l->द्विमान_weights = swap;

#यदि_परिभाषितम् GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->द्विमान_weights_gpu;
    l->द्विमान_weights_gpu = swap;
#यदि_अन्तः
}

शून्य binarize_weights(भिन्न *weights, पूर्णाङ्क n, पूर्णाङ्क आकार, भिन्न *द्विमान)
{
    पूर्णाङ्क i, f;
    प्रति(f = 0; f < n; ++f){
        भिन्न माध्यम् = 0;
        प्रति(i = 0; i < आकार; ++i){
            माध्यम् += fabs(weights[f*आकार + i]);
        }
        माध्यम् = माध्यम् / आकार;
        प्रति(i = 0; i < आकार; ++i){
            द्विमान[f*आकार + i] = (weights[f*आकार + i] > 0) ? माध्यम् : -माध्यम्;
        }
    }
}

शून्य binarize_cpu(भिन्न *निवेशः, पूर्णाङ्क n, भिन्न *द्विमान)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        द्विमान[i] = (निवेशः[i] > 0) ? 1 : -1;
    }
}

शून्य binarize_निवेशः(भिन्न *निवेशः, पूर्णाङ्क n, पूर्णाङ्क आकार, भिन्न *द्विमान)
{
    पूर्णाङ्क i, s;
    प्रति(s = 0; s < आकार; ++s){
        भिन्न माध्यम् = 0;
        प्रति(i = 0; i < n; ++i){
            माध्यम् += fabs(निवेशः[i*आकार + s]);
        }
        माध्यम् = माध्यम् / n;
        प्रति(i = 0; i < n; ++i){
            द्विमान[i*आकार + s] = (निवेशः[i*आकार + s] > 0) ? माध्यम् : -माध्यम्;
        }
    }
}

पूर्णाङ्क convolutional_बहिर्गमनम्_height(convolutional_layer l)
{
    प्रत्यागम (l.h + 2*l.pad - l.आकार) / l.stride + 1;
}

पूर्णाङ्क convolutional_बहिर्गमनम्_विस्तारः(convolutional_layer l)
{
    प्रत्यागम (l.w + 2*l.pad - l.आकार) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    प्रत्यागम float_गन्तव्यम्_image(l.बहिर्गमनम्_w,l.बहिर्गमनम्_h,l.बहिर्गमनम्_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    प्रत्यागम float_गन्तव्यम्_image(l.बहिर्गमनम्_w,l.बहिर्गमनम्_h,l.बहिर्गमनम्_c,l.delta);
}

स्थिर size_t get_workspace_आकार(layer l){
#यदि_परिभाषितम् CUDNN
    यदि(gpu_सूचकांक >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnn_Get_Convolution_Forward_Workspace_आकार(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        यदि (s > most) most = s;
        cudnn_Get_Convolution_Backward_Filter_Workspace_आकार(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        यदि (s > most) most = s;
        cudnn_Get_Convolution_Backward_दत्त_Workspace_आकार(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        यदि (s > most) most = s;
        प्रत्यागम most;
    }
#यदि_अन्तः
    प्रत्यागम (size_t)l.बहिर्गमनम्_h*l.बहिर्गमनम्_w*l.आकार*l.आकार*l.c/l.groups*परिमाण(भिन्न);
}

#यदि_परिभाषितम् GPU
#यदि_परिभाषितम् CUDNN
शून्य cudnn_convolutional_सज्जता(layer *l)
{
    cudnn_समुच्चयः_Tensor4d_Descriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_समुच्चयः_Tensor4d_Descriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, l->batch, l->बहिर्गमनम्_c, l->बहिर्गमनम्_h, l->बहिर्गमनम्_w); 

    cudnn_समुच्चयः_Tensor4d_Descriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_समुच्चयः_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, l->batch, l->बहिर्गमनम्_c, l->बहिर्गमनम्_h, l->बहिर्गमनम्_w); 
    cudnn_समुच्चयः_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_दत्त_FLOAT, 1, l->बहिर्गमनम्_c, 1, 1); 

    cudnn_समुच्चयः_Filter4d_Descriptor(l->dweightDesc, CUDNN_दत्त_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->आकार, l->आकार); 
    cudnn_समुच्चयः_Filter4d_Descriptor(l->weightDesc, CUDNN_दत्त_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->आकार, l->आकार); 
    #यदि CUDNN_MAJOR >= 6
    cudnn_समुच्चयः_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_दत्त_FLOAT);
    #अन्यथा
    cudnn_समुच्चयः_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #यदि_अन्तः

    #यदि CUDNN_MAJOR >= 7
    cudnn_समुच्चयः_Convolution_गणः_गणनम्(l->convDesc, l->groups);
    #अन्यथा
    यदि(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #यदि_अन्तः

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->weightDesc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_सीमा,
            2000000000,
            &l->fw_algo);
    cudnn_Get_Convolution_Backward_दत्त_Algorithm(cudnn_handle(),
            l->weightDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_दत्त_SPECIFY_WORKSPACE_सीमा,
            2000000000,
            &l->bd_algo);
    cudnnGetConvolutionBackwardFilterAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dweightDesc,
            CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_सीमा,
            2000000000,
            &l->bf_algo);
}
#यदि_अन्तः
#यदि_अन्तः

convolutional_layer make_convolutional_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क groups, पूर्णाङ्क आकार, पूर्णाङ्क stride, पूर्णाङ्क padding, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क द्विमान, पूर्णाङ्क xnor, पूर्णाङ्क adam)
{
    पूर्णाङ्क i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.द्विमान = द्विमान;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.आकार = आकार;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = शून्यस्मृतिं_देहि(c/groups*n*आकार*आकार, परिमाण(भिन्न));
    l.weight_updates = शून्यस्मृतिं_देहि(c/groups*n*आकार*आकार, परिमाण(भिन्न));

    l.biases = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
    l.bias_updates = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));

    l.nweights = c/groups*n*आकार*आकार;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    भिन्न scale = वर्गमूलम्(2./(आकार*आकार*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    प्रति(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_प्रसामान्यम्();
    पूर्णाङ्क बहिर्गमनम्_w = convolutional_बहिर्गमनम्_विस्तारः(l);
    पूर्णाङ्क बहिर्गमनम्_h = convolutional_बहिर्गमनम्_height(l);
    l.बहिर्गमनम्_h = बहिर्गमनम्_h;
    l.बहिर्गमनम्_w = बहिर्गमनम्_w;
    l.बहिर्गमनम्_c = n;
    l.outputs = l.बहिर्गमनम्_h * l.बहिर्गमनम्_w * l.बहिर्गमनम्_c;
    l.inputs = l.w * l.h * l.c;

    l.output = शून्यस्मृतिं_देहि(l.batch*l.outputs, परिमाण(भिन्न));
    l.delta  = शून्यस्मृतिं_देहि(l.batch*l.outputs, परिमाण(भिन्न));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    यदि(द्विमान){
        l.द्विमान_weights = शून्यस्मृतिं_देहि(l.nweights, परिमाण(भिन्न));
        l.cweights = शून्यस्मृतिं_देहि(l.nweights, परिमाण(वर्ण));
        l.scales = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
    }
    यदि(xnor){
        l.द्विमान_weights = शून्यस्मृतिं_देहि(l.nweights, परिमाण(भिन्न));
        l.द्विमान_निवेशः = शून्यस्मृतिं_देहि(l.inputs*l.batch, परिमाण(भिन्न));
    }

    यदि(batch_normalize){
        l.scales = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        l.scale_updates = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        प्रति(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.माध्यम् = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        l.variance = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));

        l.माध्यम्_delta = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        l.variance_delta = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));

        l.rolling_माध्यम् = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        l.rolling_variance = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        l.x = शून्यस्मृतिं_देहि(l.batch*l.outputs, परिमाण(भिन्न));
        l.x_norm = शून्यस्मृतिं_देहि(l.batch*l.outputs, परिमाण(भिन्न));
    }
    यदि(adam){
        l.m = शून्यस्मृतिं_देहि(l.nweights, परिमाण(भिन्न));
        l.v = शून्यस्मृतिं_देहि(l.nweights, परिमाण(भिन्न));
        l.bias_m = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        l.scale_m = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        l.bias_v = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
        l.scale_v = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न));
    }

#यदि_परिभाषितम् GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    यदि(gpu_सूचकांक >= 0){
        यदि (adam) {
            l.m_gpu = cuda_make_array(l.m, l.nweights);
            l.v_gpu = cuda_make_array(l.v, l.nweights);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }

        l.weights_gpu = cuda_make_array(l.weights, l.nweights);
        l.weight_updates_gpu = cuda_make_array(l.weight_updates, l.nweights);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*बहिर्गमनम्_h*बहिर्गमनम्_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*बहिर्गमनम्_h*बहिर्गमनम्_w*n);

        यदि(द्विमान){
            l.द्विमान_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        यदि(xnor){
            l.द्विमान_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.द्विमान_निवेशः_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        यदि(batch_normalize){
            l.माध्यम्_gpu = cuda_make_array(l.माध्यम्, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_माध्यम्_gpu = cuda_make_array(l.माध्यम्, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.माध्यम्_delta_gpu = cuda_make_array(l.माध्यम्, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*बहिर्गमनम्_h*बहिर्गमनम्_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*बहिर्गमनम्_h*बहिर्गमनम्_w*n);
        }
#यदि_परिभाषितम् CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.weightDesc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_सज्जता(&l);
#यदि_अन्तः
    }
#यदि_अन्तः
    l.workspace_आकार = get_workspace_आकार(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, आकार, आकार, stride, w, h, c, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c, (2.0 * l.n * l.आकार*l.आकार*l.c/l.groups * l.बहिर्गमनम्_h*l.बहिर्गमनम्_w)/1000000000.);

    प्रत्यागम l;
}

शून्य denormalize_convolutional_layer(convolutional_layer l)
{
    पूर्णाङ्क i, j;
    प्रति(i = 0; i < l.n; ++i){
        भिन्न scale = l.scales[i]/वर्गमूलम्(l.rolling_variance[i] + .00001);
        प्रति(j = 0; j < l.c/l.groups*l.आकार*l.आकार; ++j){
            l.weights[i*l.c/l.groups*l.आकार*l.आकार + j] *= scale;
        }
        l.biases[i] -= l.rolling_माध्यम्[i] * scale;
        l.scales[i] = 1;
        l.rolling_माध्यम्[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

/*
void test_convolutional_layer()
{
    convolutional_layer l = make_convolutional_layer(1, 5, 5, 3, 2, 5, 2, 1, LEAKY, 1, 0, 0, 0);
    l.batch_normalize = 1;
    float data[] = {1,1,1,1,1,
        1,1,1,1,1,
        1,1,1,1,1,
        1,1,1,1,1,
        1,1,1,1,1,
        2,2,2,2,2,
        2,2,2,2,2,
        2,2,2,2,2,
        2,2,2,2,2,
        2,2,2,2,2,
        3,3,3,3,3,
        3,3,3,3,3,
        3,3,3,3,3,
        3,3,3,3,3,
        3,3,3,3,3};
    //net.input = data;
    //forward_convolutional_layer(l);
}
*/

शून्य resize_convolutional_layer(convolutional_layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    l->w = w;
    l->h = h;
    पूर्णाङ्क बहिर्गमनम्_w = convolutional_बहिर्गमनम्_विस्तारः(*l);
    पूर्णाङ्क बहिर्गमनम्_h = convolutional_बहिर्गमनम्_height(*l);

    l->बहिर्गमनम्_w = बहिर्गमनम्_w;
    l->बहिर्गमनम्_h = बहिर्गमनम्_h;

    l->outputs = l->बहिर्गमनम्_h * l->बहिर्गमनम्_w * l->बहिर्गमनम्_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*परिमाण(भिन्न));
    यदि(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*परिमाण(भिन्न));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*परिमाण(भिन्न));
    }

#यदि_परिभाषितम् GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    यदि(l->batch_normalize){
        cuda_free(l->x_gpu);
        cuda_free(l->x_norm_gpu);

        l->x_gpu = cuda_make_array(l->output, l->batch*l->outputs);
        l->x_norm_gpu = cuda_make_array(l->output, l->batch*l->outputs);
    }
#यदि_परिभाषितम् CUDNN
    cudnn_convolutional_सज्जता(l);
#यदि_अन्तः
#यदि_अन्तः
    l->workspace_आकार = get_workspace_आकार(*l);
}

शून्य सङ्कलय_bias(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क आकार)
{
    पूर्णाङ्क i,j,b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(i = 0; i < n; ++i){
            प्रति(j = 0; j < आकार; ++j){
                output[(b*n + i)*आकार + j] += biases[i];
            }
        }
    }
}

शून्य scale_bias(भिन्न *output, भिन्न *scales, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क आकार)
{
    पूर्णाङ्क i,j,b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(i = 0; i < n; ++i){
            प्रति(j = 0; j < आकार; ++j){
                output[(b*n + i)*आकार + j] *= scales[i];
            }
        }
    }
}

शून्य backward_bias(भिन्न *bias_updates, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क आकार)
{
    पूर्णाङ्क i,b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(i = 0; i < n; ++i){
            bias_updates[i] += योग_array(delta+आकार*(i+b*n), आकार);
        }
    }
}

शून्य forward_convolutional_layer(convolutional_layer l, जालम् net)
{
    पूर्णाङ्क i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    यदि(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.आकार*l.आकार, l.द्विमान_weights);
        swap_द्विमान(&l);
        binarize_cpu(net.निवेशः, l.c*l.h*l.w*l.batch, l.द्विमान_निवेशः);
        net.निवेशः = l.द्विमान_निवेशः;
    }

    पूर्णाङ्क m = l.n/l.groups;
    पूर्णाङ्क k = l.आकार*l.आकार*l.c/l.groups;
    पूर्णाङ्क n = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;
    प्रति(i = 0; i < l.batch; ++i){
        प्रति(j = 0; j < l.groups; ++j){
            भिन्न *a = l.weights + j*l.nweights/l.groups;
            भिन्न *b = net.workspace;
            भिन्न *c = l.output + (i*l.groups + j)*n*m;
            भिन्न *im =  net.निवेशः + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            यदि (l.आकार == 1) {
                b = im;
            } अन्यथा {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.आकार, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    यदि(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } अन्यथा {
        सङ्कलय_bias(l.output, l.biases, l.batch, l.n, l.बहिर्गमनम्_h*l.बहिर्गमनम्_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    यदि(l.द्विमान || l.xnor) swap_द्विमान(&l);
}

शून्य backward_convolutional_layer(convolutional_layer l, जालम् net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क m = l.n/l.groups;
    पूर्णाङ्क n = l.आकार*l.आकार*l.c/l.groups;
    पूर्णाङ्क k = l.बहिर्गमनम्_w*l.बहिर्गमनम्_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    यदि(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } अन्यथा {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    प्रति(i = 0; i < l.batch; ++i){
        प्रति(j = 0; j < l.groups; ++j){
            भिन्न *a = l.delta + (i*l.groups + j)*m*k;
            भिन्न *b = net.workspace;
            भिन्न *c = l.weight_updates + j*l.nweights/l.groups;

            भिन्न *im  = net.निवेशः + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            भिन्न *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            यदि(l.आकार == 1){
                b = im;
            } अन्यथा {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.आकार, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            यदि (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                यदि (l.आकार == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                यदि (l.आकार != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.आकार, l.stride, l.pad, imd);
                }
            }
        }
    }
}

शून्य update_convolutional_layer(convolutional_layer l, update_तर्काः a)
{
    भिन्न learning_दरः = a.learning_दरः*l.learning_दरः_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;

    axpy_cpu(l.n, learning_दरः/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    यदि(l.scales){
        axpy_cpu(l.n, learning_दरः/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_दरः/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, पूर्णाङ्क i)
{
    पूर्णाङ्क h = l.आकार;
    पूर्णाङ्क w = l.आकार;
    पूर्णाङ्क c = l.c/l.groups;
    प्रत्यागम float_गन्तव्यम्_image(w,h,c,l.weights+i*h*w*c);
}

शून्य rgbgr_weights(convolutional_layer l)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        यदि (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

शून्य rescale_weights(convolutional_layer l, भिन्न scale, भिन्न trans)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        यदि (im.c == 3) {
            scale_image(im, scale);
            भिन्न योग = योग_array(im.दत्त, im.w*im.h*im.c);
            l.biases[i] += योग*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = शून्यस्मृतिं_देहि(l.n, परिमाण(image));
    पूर्णाङ्क i;
    प्रति(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    प्रत्यागम weights;
}

image *visualize_convolutional_layer(convolutional_layer l, वर्ण *गवाक्षः, image *पूर्व_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, गवाक्षः);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    वर्ण buff[256];
    sprintf(buff, "%s: Output", गवाक्षः);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    प्रत्यागम single_weights;
}

