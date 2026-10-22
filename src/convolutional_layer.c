#ଅନ୍ତର୍ଭାଵ "convolutional_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "batchnorm_layer.h"
#ଅନ୍ତର୍ଭାଵ "im2col.h"
#ଅନ୍ତର୍ଭାଵ "col2im.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "gemm.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ifdef AI2
#ଅନ୍ତର୍ଭାଵ "xnor_layer.h"
#endif

ଶୂନ୍ୟ swap_ଦ୍ୱିମିକ(convolutional_layer *l)
{
    ଦଶମିକ *swap = l->weights;
    l->weights = l->ଦ୍ୱିମିକ_weights;
    l->ଦ୍ୱିମିକ_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->ଦ୍ୱିମିକ_weights_gpu;
    l->ଦ୍ୱିମିକ_weights_gpu = swap;
#endif
}

ଶୂନ୍ୟ binarize_weights(ଦଶମିକ *weights, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ *ଦ୍ୱିମିକ)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, f;
    ପାଇଁ(f = 0; f < n; ++f){
        ଦଶମିକ mean = 0;
        ପାଇଁ(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        ପାଇଁ(i = 0; i < size; ++i){
            ଦ୍ୱିମିକ[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

ଶୂନ୍ୟ binarize_cpu(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *ଦ୍ୱିମିକ)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଦ୍ୱିମିକ[i] = (ନିବେଶ[i] > 0) ? 1 : -1;
    }
}

ଶୂନ୍ୟ binarize_ନିବେଶ(ଦଶମିକ *ନିବେଶ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ *ଦ୍ୱିମିକ)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, s;
    ପାଇଁ(s = 0; s < size; ++s){
        ଦଶମିକ mean = 0;
        ପାଇଁ(i = 0; i < n; ++i){
            mean += fabs(ନିବେଶ[i*size + s]);
        }
        mean = mean / n;
        ପାଇଁ(i = 0; i < n; ++i){
            ଦ୍ୱିମିକ[i*size + s] = (ନିବେଶ[i*size + s] > 0) ? mean : -mean;
        }
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା convolutional_out_height(convolutional_layer l)
{
    ଫେରନ୍ତୁ (l.h + 2*l.pad - l.size) / l.stride + 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା convolutional_out_width(convolutional_layer l)
{
    ଫେରନ୍ତୁ (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    ଫେରନ୍ତୁ float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    ଫେରନ୍ତୁ float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

ସ୍ଥିର size_t get_workspace_size(layer l){
#ifdef CUDNN
    ଯଦି(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        ଯଦି (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        ଯଦି (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        ଯଦି (s > most) most = s;
        ଫେରନ୍ତୁ most;
    }
#endif
    ଫେରନ୍ତୁ (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*ଆକାର(ଦଶମିକ);
}

#ifdef GPU
#ifdef CUDNN
ଶୂନ୍ୟ cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #ଯଦି CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #ଅନ୍ୟଥା
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #ଯଦି CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_ଗଣନା(l->convDesc, l->groups);
    #ଅନ୍ୟଥା
    ଯଦି(l->groups > 1){
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

convolutional_layer make_convolutional_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride, ପୂର୍ଣ୍ଣସଂଖ୍ୟା padding, ACTIVATION activation, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଦ୍ୱିମିକ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା xnor, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adam)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.ଦ୍ୱିମିକ = ଦ୍ୱିମିକ;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, ଆକାର(ଦଶମିକ));
    l.weight_updates = calloc(c/groups*n*size*size, ଆକାର(ଦଶମିକ));

    l.biases = calloc(n, ଆକାର(ଦଶମିକ));
    l.bias_updates = calloc(n, ଆକାର(ଦଶମିକ));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    ଦଶମିକ scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    ପାଇଁ(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_w = convolutional_out_width(l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, ଆକାର(ଦଶମିକ));
    l.delta  = calloc(l.batch*l.outputs, ଆକାର(ଦଶମିକ));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    ଯଦି(ଦ୍ୱିମିକ){
        l.ଦ୍ୱିମିକ_weights = calloc(l.nweights, ଆକାର(ଦଶମିକ));
        l.cweights = calloc(l.nweights, ଆକାର(ଅକ୍ଷର));
        l.scales = calloc(n, ଆକାର(ଦଶମିକ));
    }
    ଯଦି(xnor){
        l.ଦ୍ୱିମିକ_weights = calloc(l.nweights, ଆକାର(ଦଶମିକ));
        l.ଦ୍ୱିମିକ_ନିବେଶ = calloc(l.inputs*l.batch, ଆକାର(ଦଶମିକ));
    }

    ଯଦି(batch_normalize){
        l.scales = calloc(n, ଆକାର(ଦଶମିକ));
        l.scale_updates = calloc(n, ଆକାର(ଦଶମିକ));
        ପାଇଁ(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, ଆକାର(ଦଶମିକ));
        l.variance = calloc(n, ଆକାର(ଦଶମିକ));

        l.mean_delta = calloc(n, ଆକାର(ଦଶମିକ));
        l.variance_delta = calloc(n, ଆକାର(ଦଶମିକ));

        l.rolling_mean = calloc(n, ଆକାର(ଦଶମିକ));
        l.rolling_variance = calloc(n, ଆକାର(ଦଶମିକ));
        l.x = calloc(l.batch*l.outputs, ଆକାର(ଦଶମିକ));
        l.x_norm = calloc(l.batch*l.outputs, ଆକାର(ଦଶମିକ));
    }
    ଯଦି(adam){
        l.m = calloc(l.nweights, ଆକାର(ଦଶମିକ));
        l.v = calloc(l.nweights, ଆକାର(ଦଶମିକ));
        l.bias_m = calloc(n, ଆକାର(ଦଶମିକ));
        l.scale_m = calloc(n, ଆକାର(ଦଶମିକ));
        l.bias_v = calloc(n, ଆକାର(ଦଶମିକ));
        l.scale_v = calloc(n, ଆକାର(ଦଶମିକ));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    ଯଦି(gpu_index >= 0){
        ଯଦି (adam) {
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

        ଯଦି(ଦ୍ୱିମିକ){
            l.ଦ୍ୱିମିକ_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        ଯଦି(xnor){
            l.ଦ୍ୱିମିକ_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.ଦ୍ୱିମିକ_ନିବେଶ_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        ଯଦି(batch_normalize){
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

    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ denormalize_convolutional_layer(convolutional_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପାଇଁ(i = 0; i < l.n; ++i){
        ଦଶମିକ scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        ପାଇଁ(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

ଶୂନ୍ୟ resize_convolutional_layer(convolutional_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    l->w = w;
    l->h = h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_w = convolutional_out_width(*l);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*ଆକାର(ଦଶମିକ));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*ଆକାର(ଦଶମିକ));
    ଯଦି(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*ଆକାର(ଦଶମିକ));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*ଆକାର(ଦଶମିକ));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    ଯଦି(l->batch_normalize){
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

ଶୂନ୍ୟ add_bias(ଦଶମିକ *output, ଦଶମିକ *biases, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,b;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(i = 0; i < n; ++i){
            ପାଇଁ(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

ଶୂନ୍ୟ scale_bias(ଦଶମିକ *output, ଦଶମିକ *scales, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,b;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(i = 0; i < n; ++i){
            ପାଇଁ(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

ଶୂନ୍ୟ backward_bias(ଦଶମିକ *bias_updates, ଦଶମିକ *delta, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,b;
    ପାଇଁ(b = 0; b < batch; ++b){
        ପାଇଁ(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

ଶୂନ୍ୟ forward_convolutional_layer(convolutional_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ଯଦି(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.ଦ୍ୱିମିକ_weights);
        swap_ଦ୍ୱିମିକ(&l);
        binarize_cpu(net.ନିବେଶ, l.c*l.h*l.w*l.batch, l.ଦ୍ୱିମିକ_ନିବେଶ);
        net.ନିବେଶ = l.ଦ୍ୱିମିକ_ନିବେଶ;
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.n/l.groups;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.size*l.size*l.c/l.groups;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.out_w*l.out_h;
    ପାଇଁ(i = 0; i < l.batch; ++i){
        ପାଇଁ(j = 0; j < l.groups; ++j){
            ଦଶମିକ *a = l.weights + j*l.nweights/l.groups;
            ଦଶମିକ *b = net.workspace;
            ଦଶମିକ *c = l.output + (i*l.groups + j)*n*m;
            ଦଶମିକ *im =  net.ନିବେଶ + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ଯଦି (l.size == 1) {
                b = im;
            } ଅନ୍ୟଥା {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    ଯଦି(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ଅନ୍ୟଥା {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    ଯଦି(l.ଦ୍ୱିମିକ || l.xnor) swap_ଦ୍ୱିମିକ(&l);
}

ଶୂନ୍ୟ backward_convolutional_layer(convolutional_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = l.n/l.groups;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = l.size*l.size*l.c/l.groups;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ଯଦି(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ଅନ୍ୟଥା {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    ପାଇଁ(i = 0; i < l.batch; ++i){
        ପାଇଁ(j = 0; j < l.groups; ++j){
            ଦଶମିକ *a = l.delta + (i*l.groups + j)*m*k;
            ଦଶମିକ *b = net.workspace;
            ଦଶମିକ *c = l.weight_updates + j*l.nweights/l.groups;

            ଦଶମିକ *im  = net.ନିବେଶ + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            ଦଶମିକ *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ଯଦି(l.size == 1){
                b = im;
            } ଅନ୍ୟଥା {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            ଯଦି (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                ଯଦି (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                ଯଦି (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

ଶୂନ୍ୟ update_convolutional_layer(convolutional_layer l, update_args a)
{
    ଦଶମିକ learning_rate = a.learning_rate*l.learning_rate_scale;
    ଦଶମିକ momentum = a.momentum;
    ଦଶମିକ decay = a.decay;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ଯଦି(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = l.size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = l.size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = l.c/l.groups;
    ଫେରନ୍ତୁ float_to_image(w,h,c,l.weights+i*h*w*c);
}

ଶୂନ୍ୟ rgbgr_weights(convolutional_layer l)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        ଯଦି (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

ଶୂନ୍ୟ rescale_weights(convolutional_layer l, ଦଶମିକ scale, ଦଶମିକ trans)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        ଯଦି (im.c == 3) {
            scale_image(im, scale);
            ଦଶମିକ sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, ଆକାର(image));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    ଫେରନ୍ତୁ weights;
}

image *visualize_convolutional_layer(convolutional_layer l, ଅକ୍ଷର *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    ଅକ୍ଷର buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    ଫେରନ୍ତୁ single_weights;
}

