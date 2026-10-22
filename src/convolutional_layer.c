#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "batchnorm_layer.h"
#अन्तर्भाव "im2col.h"
#अन्तर्भाव "col2im.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>

#ifdef AI2
#अन्तर्भाव "xnor_layer.h"
#endif

शून्य swap_द्विमान(convolutional_layer *l)
{
    भिन्न *swap = l->weights;
    l->weights = l->द्विमान_weights;
    l->द्विमान_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->द्विमान_weights_gpu;
    l->द्विमान_weights_gpu = swap;
#endif
}

शून्य binarize_weights(भिन्न *weights, पूर्णाङ्क n, पूर्णाङ्क size, भिन्न *द्विमान)
{
    पूर्णाङ्क i, f;
    अत्थाय(f = 0; f < n; ++f){
        भिन्न mean = 0;
        अत्थाय(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        अत्थाय(i = 0; i < size; ++i){
            द्विमान[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

शून्य binarize_cpu(भिन्न *निवेशः, पूर्णाङ्क n, भिन्न *द्विमान)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < n; ++i){
        द्विमान[i] = (निवेशः[i] > 0) ? 1 : -1;
    }
}

शून्य binarize_निवेशः(भिन्न *निवेशः, पूर्णाङ्क n, पूर्णाङ्क size, भिन्न *द्विमान)
{
    पूर्णाङ्क i, s;
    अत्थाय(s = 0; s < size; ++s){
        भिन्न mean = 0;
        अत्थाय(i = 0; i < n; ++i){
            mean += fabs(निवेशः[i*size + s]);
        }
        mean = mean / n;
        अत्थाय(i = 0; i < n; ++i){
            द्विमान[i*size + s] = (निवेशः[i*size + s] > 0) ? mean : -mean;
        }
    }
}

पूर्णाङ्क convolutional_out_height(convolutional_layer l)
{
    पटिनिवत्त (l.h + 2*l.pad - l.size) / l.stride + 1;
}

पूर्णाङ्क convolutional_out_width(convolutional_layer l)
{
    पटिनिवत्त (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    पटिनिवत्त float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    पटिनिवत्त float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

स्थिर size_t get_workspace_size(layer l){
#ifdef CUDNN
    सचे(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        सचे (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        सचे (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        सचे (s > most) most = s;
        पटिनिवत्त most;
    }
#endif
    पटिनिवत्त (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*परिमाण(भिन्न);
}

#ifdef GPU
#ifdef CUDNN
शून्य cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #सचे CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #अञ्ञथा
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #सचे CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_गणनम्(l->convDesc, l->groups);
    #अञ्ञथा
    सचे(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #endif

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->weightDesc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_LIMIT,
            2000000000,
            &l->fw_algo);
    cudnnGetConvolutionBackwardDataAlgorithm(cudnn_handle(),
            l->weightDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_DATA_SPECIFY_WORKSPACE_LIMIT,
            2000000000,
            &l->bd_algo);
    cudnnGetConvolutionBackwardFilterAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dweightDesc,
            CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_LIMIT,
            2000000000,
            &l->bf_algo);
}
#endif
#endif

convolutional_layer make_convolutional_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क groups, पूर्णाङ्क size, पूर्णाङ्क stride, पूर्णाङ्क padding, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क द्विमान, पूर्णाङ्क xnor, पूर्णाङ्क adam)
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
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, परिमाण(भिन्न));
    l.weight_updates = calloc(c/groups*n*size*size, परिमाण(भिन्न));

    l.biases = calloc(n, परिमाण(भिन्न));
    l.bias_updates = calloc(n, परिमाण(भिन्न));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    भिन्न scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    अत्थाय(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    पूर्णाङ्क out_w = convolutional_out_width(l);
    पूर्णाङ्क out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, परिमाण(भिन्न));
    l.delta  = calloc(l.batch*l.outputs, परिमाण(भिन्न));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    सचे(द्विमान){
        l.द्विमान_weights = calloc(l.nweights, परिमाण(भिन्न));
        l.cweights = calloc(l.nweights, परिमाण(वर्ण));
        l.scales = calloc(n, परिमाण(भिन्न));
    }
    सचे(xnor){
        l.द्विमान_weights = calloc(l.nweights, परिमाण(भिन्न));
        l.द्विमान_निवेशः = calloc(l.inputs*l.batch, परिमाण(भिन्न));
    }

    सचे(batch_normalize){
        l.scales = calloc(n, परिमाण(भिन्न));
        l.scale_updates = calloc(n, परिमाण(भिन्न));
        अत्थाय(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, परिमाण(भिन्न));
        l.variance = calloc(n, परिमाण(भिन्न));

        l.mean_delta = calloc(n, परिमाण(भिन्न));
        l.variance_delta = calloc(n, परिमाण(भिन्न));

        l.rolling_mean = calloc(n, परिमाण(भिन्न));
        l.rolling_variance = calloc(n, परिमाण(भिन्न));
        l.x = calloc(l.batch*l.outputs, परिमाण(भिन्न));
        l.x_norm = calloc(l.batch*l.outputs, परिमाण(भिन्न));
    }
    सचे(adam){
        l.m = calloc(l.nweights, परिमाण(भिन्न));
        l.v = calloc(l.nweights, परिमाण(भिन्न));
        l.bias_m = calloc(n, परिमाण(भिन्न));
        l.scale_m = calloc(n, परिमाण(भिन्न));
        l.bias_v = calloc(n, परिमाण(भिन्न));
        l.scale_v = calloc(n, परिमाण(भिन्न));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    सचे(gpu_index >= 0){
        सचे (adam) {
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

        l.delta_gpu = cuda_make_array(l.delta, l.batch*out_h*out_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*out_h*out_w*n);

        सचे(द्विमान){
            l.द्विमान_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        सचे(xnor){
            l.द्विमान_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.द्विमान_निवेशः_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        सचे(batch_normalize){
            l.mean_gpu = cuda_make_array(l.mean, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_mean_gpu = cuda_make_array(l.mean, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.mean_delta_gpu = cuda_make_array(l.mean, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*out_h*out_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*out_h*out_w*n);
        }
#ifdef CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.weightDesc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_setup(&l);
#endif
    }
#endif
    l.workspace_size = get_workspace_size(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, size, size, stride, w, h, c, l.out_w, l.out_h, l.out_c, (2.0 * l.n * l.size*l.size*l.c/l.groups * l.out_h*l.out_w)/1000000000.);

    पटिनिवत्त l;
}

शून्य denormalize_convolutional_layer(convolutional_layer l)
{
    पूर्णाङ्क i, j;
    अत्थाय(i = 0; i < l.n; ++i){
        भिन्न scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        अत्थाय(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
            l.weights[i*l.c/l.groups*l.size*l.size + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
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
    पूर्णाङ्क out_w = convolutional_out_width(*l);
    पूर्णाङ्क out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*परिमाण(भिन्न));
    सचे(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*परिमाण(भिन्न));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*परिमाण(भिन्न));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    सचे(l->batch_normalize){
        cuda_free(l->x_gpu);
        cuda_free(l->x_norm_gpu);

        l->x_gpu = cuda_make_array(l->output, l->batch*l->outputs);
        l->x_norm_gpu = cuda_make_array(l->output, l->batch*l->outputs);
    }
#ifdef CUDNN
    cudnn_convolutional_setup(l);
#endif
#endif
    l->workspace_size = get_workspace_size(*l);
}

शून्य add_bias(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size)
{
    पूर्णाङ्क i,j,b;
    अत्थाय(b = 0; b < batch; ++b){
        अत्थाय(i = 0; i < n; ++i){
            अत्थाय(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

शून्य scale_bias(भिन्न *output, भिन्न *scales, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size)
{
    पूर्णाङ्क i,j,b;
    अत्थाय(b = 0; b < batch; ++b){
        अत्थाय(i = 0; i < n; ++i){
            अत्थाय(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

शून्य backward_bias(भिन्न *bias_updates, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size)
{
    पूर्णाङ्क i,b;
    अत्थाय(b = 0; b < batch; ++b){
        अत्थाय(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

शून्य forward_convolutional_layer(convolutional_layer l, network net)
{
    पूर्णाङ्क i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    सचे(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.द्विमान_weights);
        swap_द्विमान(&l);
        binarize_cpu(net.निवेशः, l.c*l.h*l.w*l.batch, l.द्विमान_निवेशः);
        net.निवेशः = l.द्विमान_निवेशः;
    }

    पूर्णाङ्क m = l.n/l.groups;
    पूर्णाङ्क k = l.size*l.size*l.c/l.groups;
    पूर्णाङ्क n = l.out_w*l.out_h;
    अत्थाय(i = 0; i < l.batch; ++i){
        अत्थाय(j = 0; j < l.groups; ++j){
            भिन्न *a = l.weights + j*l.nweights/l.groups;
            भिन्न *b = net.workspace;
            भिन्न *c = l.output + (i*l.groups + j)*n*m;
            भिन्न *im =  net.निवेशः + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            सचे (l.size == 1) {
                b = im;
            } अञ्ञथा {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    सचे(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } अञ्ञथा {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    सचे(l.द्विमान || l.xnor) swap_द्विमान(&l);
}

शून्य backward_convolutional_layer(convolutional_layer l, network net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क m = l.n/l.groups;
    पूर्णाङ्क n = l.size*l.size*l.c/l.groups;
    पूर्णाङ्क k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    सचे(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } अञ्ञथा {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    अत्थाय(i = 0; i < l.batch; ++i){
        अत्थाय(j = 0; j < l.groups; ++j){
            भिन्न *a = l.delta + (i*l.groups + j)*m*k;
            भिन्न *b = net.workspace;
            भिन्न *c = l.weight_updates + j*l.nweights/l.groups;

            भिन्न *im  = net.निवेशः + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            भिन्न *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            सचे(l.size == 1){
                b = im;
            } अञ्ञथा {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            सचे (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                सचे (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                सचे (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

शून्य update_convolutional_layer(convolutional_layer l, update_args a)
{
    भिन्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    सचे(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, पूर्णाङ्क i)
{
    पूर्णाङ्क h = l.size;
    पूर्णाङ्क w = l.size;
    पूर्णाङ्क c = l.c/l.groups;
    पटिनिवत्त float_to_image(w,h,c,l.weights+i*h*w*c);
}

शून्य rgbgr_weights(convolutional_layer l)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        सचे (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

शून्य rescale_weights(convolutional_layer l, भिन्न scale, भिन्न trans)
{
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        सचे (im.c == 3) {
            scale_image(im, scale);
            भिन्न sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, परिमाण(image));
    पूर्णाङ्क i;
    अत्थाय(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    पटिनिवत्त weights;
}

image *visualize_convolutional_layer(convolutional_layer l, वर्ण *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    वर्ण buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    पटिनिवत्त single_weights;
}

