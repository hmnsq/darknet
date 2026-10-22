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

शून्य swap_द्विआधारी(convolutional_layer *l)
{
    भिन्न *swap = l->weights;
    l->weights = l->द्विआधारी_weights;
    l->द्विआधारी_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->द्विआधारी_weights_gpu;
    l->द्विआधारी_weights_gpu = swap;
#endif
}

शून्य binarize_weights(भिन्न *weights, पूर्णाङ्क n, पूर्णाङ्क size, भिन्न *द्विआधारी)
{
    पूर्णाङ्क i, f;
    प्रति(f = 0; f < n; ++f){
        भिन्न mean = 0;
        प्रति(i = 0; i < size; ++i){
            mean += भ_असल(weights[f*size + i]);
        }
        mean = mean / size;
        प्रति(i = 0; i < size; ++i){
            द्विआधारी[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

शून्य binarize_cpu(भिन्न *निवेश, पूर्णाङ्क n, भिन्न *द्विआधारी)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        द्विआधारी[i] = (निवेश[i] > 0) ? 1 : -1;
    }
}

शून्य binarize_निवेश(भिन्न *निवेश, पूर्णाङ्क n, पूर्णाङ्क size, भिन्न *द्विआधारी)
{
    पूर्णाङ्क i, s;
    प्रति(s = 0; s < size; ++s){
        भिन्न mean = 0;
        प्रति(i = 0; i < n; ++i){
            mean += भ_असल(निवेश[i*size + s]);
        }
        mean = mean / n;
        प्रति(i = 0; i < n; ++i){
            द्विआधारी[i*size + s] = (निवेश[i*size + s] > 0) ? mean : -mean;
        }
    }
}

पूर्णाङ्क convolutional_out_height(convolutional_layer l)
{
    प्रत्यागम (l.h + 2*l.pad - l.size) / l.stride + 1;
}

पूर्णाङ्क convolutional_out_width(convolutional_layer l)
{
    प्रत्यागम (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    प्रत्यागम float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    प्रत्यागम float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

स्थिर माप_प्रकार get_workspace_size(layer l){
#ifdef CUDNN
    यदि(gpu_index >= 0){
        माप_प्रकार most = 0;
        माप_प्रकार s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        यदि (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        यदि (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        यदि (s > most) most = s;
        प्रत्यागम most;
    }
#endif
    प्रत्यागम (माप_प्रकार)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*परिमाण(भिन्न);
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
    #यदि CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #अन्यथा
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #यदि CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_गिनती(l->convDesc, l->groups);
    #अन्यथा
    यदि(l->groups > 1){
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

convolutional_layer make_convolutional_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क groups, पूर्णाङ्क size, पूर्णाङ्क stride, पूर्णाङ्क padding, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क द्विआधारी, पूर्णाङ्क xnor, पूर्णाङ्क adam)
{
    पूर्णाङ्क i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.द्विआधारी = द्विआधारी;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = सुस्मृति(c/groups*n*size*size, परिमाण(भिन्न));
    l.weight_updates = सुस्मृति(c/groups*n*size*size, परिमाण(भिन्न));

    l.biases = सुस्मृति(n, परिमाण(भिन्न));
    l.bias_updates = सुस्मृति(n, परिमाण(भिन्न));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    भिन्न scale = वर्ग_मूल(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    प्रति(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    पूर्णाङ्क out_w = convolutional_out_width(l);
    पूर्णाङ्क out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = सुस्मृति(l.batch*l.outputs, परिमाण(भिन्न));
    l.delta  = सुस्मृति(l.batch*l.outputs, परिमाण(भिन्न));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    यदि(द्विआधारी){
        l.द्विआधारी_weights = सुस्मृति(l.nweights, परिमाण(भिन्न));
        l.cweights = सुस्मृति(l.nweights, परिमाण(वर्ण));
        l.scales = सुस्मृति(n, परिमाण(भिन्न));
    }
    यदि(xnor){
        l.द्विआधारी_weights = सुस्मृति(l.nweights, परिमाण(भिन्न));
        l.द्विआधारी_निवेश = सुस्मृति(l.inputs*l.batch, परिमाण(भिन्न));
    }

    यदि(batch_normalize){
        l.scales = सुस्मृति(n, परिमाण(भिन्न));
        l.scale_updates = सुस्मृति(n, परिमाण(भिन्न));
        प्रति(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = सुस्मृति(n, परिमाण(भिन्न));
        l.variance = सुस्मृति(n, परिमाण(भिन्न));

        l.mean_delta = सुस्मृति(n, परिमाण(भिन्न));
        l.variance_delta = सुस्मृति(n, परिमाण(भिन्न));

        l.rolling_mean = सुस्मृति(n, परिमाण(भिन्न));
        l.rolling_variance = सुस्मृति(n, परिमाण(भिन्न));
        l.x = सुस्मृति(l.batch*l.outputs, परिमाण(भिन्न));
        l.x_norm = सुस्मृति(l.batch*l.outputs, परिमाण(भिन्न));
    }
    यदि(adam){
        l.m = सुस्मृति(l.nweights, परिमाण(भिन्न));
        l.v = सुस्मृति(l.nweights, परिमाण(भिन्न));
        l.bias_m = सुस्मृति(n, परिमाण(भिन्न));
        l.scale_m = सुस्मृति(n, परिमाण(भिन्न));
        l.bias_v = सुस्मृति(n, परिमाण(भिन्न));
        l.scale_v = सुस्मृति(n, परिमाण(भिन्न));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    यदि(gpu_index >= 0){
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

        l.delta_gpu = cuda_make_array(l.delta, l.batch*out_h*out_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*out_h*out_w*n);

        यदि(द्विआधारी){
            l.द्विआधारी_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        यदि(xnor){
            l.द्विआधारी_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.द्विआधारी_निवेश_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        यदि(batch_normalize){
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

    ख_लिखो(मानक_त्रुटि, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, size, size, stride, w, h, c, l.out_w, l.out_h, l.out_c, (2.0 * l.n * l.size*l.size*l.c/l.groups * l.out_h*l.out_w)/1000000000.);

    प्रत्यागम l;
}

शून्य denormalize_convolutional_layer(convolutional_layer l)
{
    पूर्णाङ्क i, j;
    प्रति(i = 0; i < l.n; ++i){
        भिन्न scale = l.scales[i]/वर्ग_मूल(l.rolling_variance[i] + .00001);
        प्रति(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta  = पुनः_स्मृति(l->delta,  l->batch*l->outputs*परिमाण(भिन्न));
    यदि(l->batch_normalize){
        l->x = पुनः_स्मृति(l->x, l->batch*l->outputs*परिमाण(भिन्न));
        l->x_norm  = पुनः_स्मृति(l->x_norm, l->batch*l->outputs*परिमाण(भिन्न));
    }

#ifdef GPU
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
#ifdef CUDNN
    cudnn_convolutional_setup(l);
#endif
#endif
    l->workspace_size = get_workspace_size(*l);
}

शून्य add_bias(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size)
{
    पूर्णाङ्क i,j,b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(i = 0; i < n; ++i){
            प्रति(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

शून्य scale_bias(भिन्न *output, भिन्न *scales, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size)
{
    पूर्णाङ्क i,j,b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(i = 0; i < n; ++i){
            प्रति(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

शून्य backward_bias(भिन्न *bias_updates, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size)
{
    पूर्णाङ्क i,b;
    प्रति(b = 0; b < batch; ++b){
        प्रति(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

शून्य forward_convolutional_layer(convolutional_layer l, network net)
{
    पूर्णाङ्क i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    यदि(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.द्विआधारी_weights);
        swap_द्विआधारी(&l);
        binarize_cpu(net.निवेश, l.c*l.h*l.w*l.batch, l.द्विआधारी_निवेश);
        net.निवेश = l.द्विआधारी_निवेश;
    }

    पूर्णाङ्क m = l.n/l.groups;
    पूर्णाङ्क k = l.size*l.size*l.c/l.groups;
    पूर्णाङ्क n = l.out_w*l.out_h;
    प्रति(i = 0; i < l.batch; ++i){
        प्रति(j = 0; j < l.groups; ++j){
            भिन्न *a = l.weights + j*l.nweights/l.groups;
            भिन्न *b = net.workspace;
            भिन्न *c = l.output + (i*l.groups + j)*n*m;
            भिन्न *im =  net.निवेश + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            यदि (l.size == 1) {
                b = im;
            } अन्यथा {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    यदि(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } अन्यथा {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    यदि(l.द्विआधारी || l.xnor) swap_द्विआधारी(&l);
}

शून्य backward_convolutional_layer(convolutional_layer l, network net)
{
    पूर्णाङ्क i, j;
    पूर्णाङ्क m = l.n/l.groups;
    पूर्णाङ्क n = l.size*l.size*l.c/l.groups;
    पूर्णाङ्क k = l.out_w*l.out_h;

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

            भिन्न *im  = net.निवेश + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            भिन्न *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            यदि(l.size == 1){
                b = im;
            } अन्यथा {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            यदि (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                यदि (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                यदि (l.size != 1) {
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

    यदि(l.scales){
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
    प्रत्यागम float_to_image(w,h,c,l.weights+i*h*w*c);
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
            भिन्न sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = सुस्मृति(l.n, परिमाण(image));
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

image *visualize_convolutional_layer(convolutional_layer l, वर्ण *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    वर्ण buff[256];
    प्र_लिखो(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    प्रत्यागम single_weights;
}

