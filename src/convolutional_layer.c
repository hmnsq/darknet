#ضمن "convolutional_layer.h"
#ضمن "utils.h"
#ضمن "batchnorm_layer.h"
#ضمن "im2col.h"
#ضمن "col2im.h"
#ضمن "blas.h"
#ضمن "gemm.h"
#ضمن <stdio.h>
#ضمن <time.h>

#ifdef AI2
#ضمن "xnor_layer.h"
#endif

فراغ swap_binary(convolutional_layer *l)
{
    عائم *swap = l->weights;
    l->weights = l->binary_weights;
    l->binary_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->binary_weights_gpu;
    l->binary_weights_gpu = swap;
#endif
}

فراغ binarize_weights(عائم *weights, صحيح n, صحيح size, عائم *binary)
{
    صحيح i, f;
    لكل(f = 0; f < n; ++f){
        عائم mean = 0;
        لكل(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        لكل(i = 0; i < size; ++i){
            binary[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

فراغ binarize_cpu(عائم *input, صحيح n, عائم *binary)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        binary[i] = (input[i] > 0) ? 1 : -1;
    }
}

فراغ binarize_input(عائم *input, صحيح n, صحيح size, عائم *binary)
{
    صحيح i, s;
    لكل(s = 0; s < size; ++s){
        عائم mean = 0;
        لكل(i = 0; i < n; ++i){
            mean += fabs(input[i*size + s]);
        }
        mean = mean / n;
        لكل(i = 0; i < n; ++i){
            binary[i*size + s] = (input[i*size + s] > 0) ? mean : -mean;
        }
    }
}

صحيح convolutional_out_height(convolutional_layer l)
{
    ܦܢܐ (l.h + 2*l.pad - l.size) / l.stride + 1;
}

صحيح convolutional_out_width(convolutional_layer l)
{
    ܦܢܐ (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    ܦܢܐ float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    ܦܢܐ float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

ساكن size_t get_workspace_size(layer l){
#ifdef CUDNN
    ܐܢ(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        ܐܢ (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        ܐܢ (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        ܐܢ (s > most) most = s;
        ܦܢܐ most;
    }
#endif
    ܦܢܐ (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*حجم(عائم);
}

#ifdef GPU
#ifdef CUDNN
فراغ cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #ܐܢ CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #ܐܚܪܢܐ
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #ܐܢ CUDNN_MAJOR >= 7
    cudnnSetConvolutionGroupCount(l->convDesc, l->groups);
    #ܐܚܪܢܐ
    ܐܢ(l->groups > 1){
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

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح size, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح binary, صحيح xnor, صحيح adam)
{
    صحيح i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.binary = binary;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, حجم(عائم));
    l.weight_updates = calloc(c/groups*n*size*size, حجم(عائم));

    l.biases = calloc(n, حجم(عائم));
    l.bias_updates = calloc(n, حجم(عائم));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    عائم scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    لكل(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    صحيح out_w = convolutional_out_width(l);
    صحيح out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, حجم(عائم));
    l.delta  = calloc(l.batch*l.outputs, حجم(عائم));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    ܐܢ(binary){
        l.binary_weights = calloc(l.nweights, حجم(عائم));
        l.cweights = calloc(l.nweights, حجم(حرف));
        l.scales = calloc(n, حجم(عائم));
    }
    ܐܢ(xnor){
        l.binary_weights = calloc(l.nweights, حجم(عائم));
        l.binary_input = calloc(l.inputs*l.batch, حجم(عائم));
    }

    ܐܢ(batch_normalize){
        l.scales = calloc(n, حجم(عائم));
        l.scale_updates = calloc(n, حجم(عائم));
        لكل(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, حجم(عائم));
        l.variance = calloc(n, حجم(عائم));

        l.mean_delta = calloc(n, حجم(عائم));
        l.variance_delta = calloc(n, حجم(عائم));

        l.rolling_mean = calloc(n, حجم(عائم));
        l.rolling_variance = calloc(n, حجم(عائم));
        l.x = calloc(l.batch*l.outputs, حجم(عائم));
        l.x_norm = calloc(l.batch*l.outputs, حجم(عائم));
    }
    ܐܢ(adam){
        l.m = calloc(l.nweights, حجم(عائم));
        l.v = calloc(l.nweights, حجم(عائم));
        l.bias_m = calloc(n, حجم(عائم));
        l.scale_m = calloc(n, حجم(عائم));
        l.bias_v = calloc(n, حجم(عائم));
        l.scale_v = calloc(n, حجم(عائم));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    ܐܢ(gpu_index >= 0){
        ܐܢ (adam) {
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

        ܐܢ(binary){
            l.binary_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        ܐܢ(xnor){
            l.binary_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.binary_input_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        ܐܢ(batch_normalize){
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

    ܦܢܐ l;
}

فراغ denormalize_convolutional_layer(convolutional_layer l)
{
    صحيح i, j;
    لكل(i = 0; i < l.n; ++i){
        عائم scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        لكل(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

فراغ resize_convolutional_layer(convolutional_layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    صحيح out_w = convolutional_out_width(*l);
    صحيح out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*حجم(عائم));
    ܐܢ(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*حجم(عائم));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*حجم(عائم));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    ܐܢ(l->batch_normalize){
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

فراغ add_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح size)
{
    صحيح i,j,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            لكل(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

فراغ scale_bias(عائم *output, عائم *scales, صحيح batch, صحيح n, صحيح size)
{
    صحيح i,j,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            لكل(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح size)
{
    صحيح i,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

فراغ forward_convolutional_layer(convolutional_layer l, network net)
{
    صحيح i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ܐܢ(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.binary_weights);
        swap_binary(&l);
        binarize_cpu(net.input, l.c*l.h*l.w*l.batch, l.binary_input);
        net.input = l.binary_input;
    }

    صحيح m = l.n/l.groups;
    صحيح k = l.size*l.size*l.c/l.groups;
    صحيح n = l.out_w*l.out_h;
    لكل(i = 0; i < l.batch; ++i){
        لكل(j = 0; j < l.groups; ++j){
            عائم *a = l.weights + j*l.nweights/l.groups;
            عائم *b = net.workspace;
            عائم *c = l.output + (i*l.groups + j)*n*m;
            عائم *im =  net.input + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ܐܢ (l.size == 1) {
                b = im;
            } ܐܚܪܢܐ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    ܐܢ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ܐܚܪܢܐ {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    ܐܢ(l.binary || l.xnor) swap_binary(&l);
}

فراغ backward_convolutional_layer(convolutional_layer l, network net)
{
    صحيح i, j;
    صحيح m = l.n/l.groups;
    صحيح n = l.size*l.size*l.c/l.groups;
    صحيح k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ܐܢ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ܐܚܪܢܐ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    لكل(i = 0; i < l.batch; ++i){
        لكل(j = 0; j < l.groups; ++j){
            عائم *a = l.delta + (i*l.groups + j)*m*k;
            عائم *b = net.workspace;
            عائم *c = l.weight_updates + j*l.nweights/l.groups;

            عائم *im  = net.input + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            عائم *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ܐܢ(l.size == 1){
                b = im;
            } ܐܚܪܢܐ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            ܐܢ (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                ܐܢ (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                ܐܢ (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

فراغ update_convolutional_layer(convolutional_layer l, update_args a)
{
    عائم learning_rate = a.learning_rate*l.learning_rate_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ܐܢ(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, صحيح i)
{
    صحيح h = l.size;
    صحيح w = l.size;
    صحيح c = l.c/l.groups;
    ܦܢܐ float_to_image(w,h,c,l.weights+i*h*w*c);
}

فراغ rgbgr_weights(convolutional_layer l)
{
    صحيح i;
    لكل(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        ܐܢ (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

فراغ rescale_weights(convolutional_layer l, عائم scale, عائم trans)
{
    صحيح i;
    لكل(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        ܐܢ (im.c == 3) {
            scale_image(im, scale);
            عائم sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, حجم(image));
    صحيح i;
    لكل(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    ܦܢܐ weights;
}

image *visualize_convolutional_layer(convolutional_layer l, حرف *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    حرف buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    ܦܢܐ single_weights;
}

