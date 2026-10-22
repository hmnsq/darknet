#அந்தர்பாவ "convolutional_layer.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "batchnorm_layer.h"
#அந்தர்பாவ "im2col.h"
#அந்தர்பாவ "col2im.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "gemm.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <time.h>

#ifdef AI2
#அந்தர்பாவ "xnor_layer.h"
#endif

வெறுமை swap_இருமம்(convolutional_layer *l)
{
    தசம *swap = l->weights;
    l->weights = l->இருமம்_weights;
    l->இருமம்_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->இருமம்_weights_gpu;
    l->இருமம்_weights_gpu = swap;
#endif
}

வெறுமை binarize_weights(தசம *weights, முழு n, முழு size, தசம *இருமம்)
{
    முழு i, f;
    க்கு(f = 0; f < n; ++f){
        தசம mean = 0;
        க்கு(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        க்கு(i = 0; i < size; ++i){
            இருமம்[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

வெறுமை binarize_cpu(தசம *உள்ளீடு, முழு n, தசம *இருமம்)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        இருமம்[i] = (உள்ளீடு[i] > 0) ? 1 : -1;
    }
}

வெறுமை binarize_உள்ளீடு(தசம *உள்ளீடு, முழு n, முழு size, தசம *இருமம்)
{
    முழு i, s;
    க்கு(s = 0; s < size; ++s){
        தசம mean = 0;
        க்கு(i = 0; i < n; ++i){
            mean += fabs(உள்ளீடு[i*size + s]);
        }
        mean = mean / n;
        க்கு(i = 0; i < n; ++i){
            இருமம்[i*size + s] = (உள்ளீடு[i*size + s] > 0) ? mean : -mean;
        }
    }
}

முழு convolutional_out_height(convolutional_layer l)
{
    திரும்பு (l.h + 2*l.pad - l.size) / l.stride + 1;
}

முழு convolutional_out_width(convolutional_layer l)
{
    திரும்பு (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    திரும்பு float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    திரும்பு float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

நிலையான size_t get_workspace_size(layer l){
#ifdef CUDNN
    எனில்(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        எனில் (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        எனில் (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        எனில் (s > most) most = s;
        திரும்பு most;
    }
#endif
    திரும்பு (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*அளவு(தசம);
}

#ifdef GPU
#ifdef CUDNN
வெறுமை cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #எனில் CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #இல்லையெனில்
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #எனில் CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_மொத்தம்(l->convDesc, l->groups);
    #இல்லையெனில்
    எனில்(l->groups > 1){
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

convolutional_layer make_convolutional_layer(முழு batch, முழு h, முழு w, முழு c, முழு n, முழு groups, முழு size, முழு stride, முழு padding, ACTIVATION activation, முழு batch_normalize, முழு இருமம், முழு xnor, முழு adam)
{
    முழு i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.இருமம் = இருமம்;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, அளவு(தசம));
    l.weight_updates = calloc(c/groups*n*size*size, அளவு(தசம));

    l.biases = calloc(n, அளவு(தசம));
    l.bias_updates = calloc(n, அளவு(தசம));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    தசம scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    க்கு(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    முழு out_w = convolutional_out_width(l);
    முழு out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, அளவு(தசம));
    l.delta  = calloc(l.batch*l.outputs, அளவு(தசம));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    எனில்(இருமம்){
        l.இருமம்_weights = calloc(l.nweights, அளவு(தசம));
        l.cweights = calloc(l.nweights, அளவு(எழுத்து));
        l.scales = calloc(n, அளவு(தசம));
    }
    எனில்(xnor){
        l.இருமம்_weights = calloc(l.nweights, அளவு(தசம));
        l.இருமம்_உள்ளீடு = calloc(l.inputs*l.batch, அளவு(தசம));
    }

    எனில்(batch_normalize){
        l.scales = calloc(n, அளவு(தசம));
        l.scale_updates = calloc(n, அளவு(தசம));
        க்கு(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, அளவு(தசம));
        l.variance = calloc(n, அளவு(தசம));

        l.mean_delta = calloc(n, அளவு(தசம));
        l.variance_delta = calloc(n, அளவு(தசம));

        l.rolling_mean = calloc(n, அளவு(தசம));
        l.rolling_variance = calloc(n, அளவு(தசம));
        l.x = calloc(l.batch*l.outputs, அளவு(தசம));
        l.x_norm = calloc(l.batch*l.outputs, அளவு(தசம));
    }
    எனில்(adam){
        l.m = calloc(l.nweights, அளவு(தசம));
        l.v = calloc(l.nweights, அளவு(தசம));
        l.bias_m = calloc(n, அளவு(தசம));
        l.scale_m = calloc(n, அளவு(தசம));
        l.bias_v = calloc(n, அளவு(தசம));
        l.scale_v = calloc(n, அளவு(தசம));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    எனில்(gpu_index >= 0){
        எனில் (adam) {
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

        எனில்(இருமம்){
            l.இருமம்_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        எனில்(xnor){
            l.இருமம்_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.இருமம்_உள்ளீடு_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        எனில்(batch_normalize){
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

    திரும்பு l;
}

வெறுமை denormalize_convolutional_layer(convolutional_layer l)
{
    முழு i, j;
    க்கு(i = 0; i < l.n; ++i){
        தசம scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        க்கு(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

வெறுமை resize_convolutional_layer(convolutional_layer *l, முழு w, முழு h)
{
    l->w = w;
    l->h = h;
    முழு out_w = convolutional_out_width(*l);
    முழு out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*அளவு(தசம));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*அளவு(தசம));
    எனில்(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*அளவு(தசம));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*அளவு(தசம));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    எனில்(l->batch_normalize){
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

வெறுமை add_bias(தசம *output, தசம *biases, முழு batch, முழு n, முழு size)
{
    முழு i,j,b;
    க்கு(b = 0; b < batch; ++b){
        க்கு(i = 0; i < n; ++i){
            க்கு(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

வெறுமை scale_bias(தசம *output, தசம *scales, முழு batch, முழு n, முழு size)
{
    முழு i,j,b;
    க்கு(b = 0; b < batch; ++b){
        க்கு(i = 0; i < n; ++i){
            க்கு(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

வெறுமை backward_bias(தசம *bias_updates, தசம *delta, முழு batch, முழு n, முழு size)
{
    முழு i,b;
    க்கு(b = 0; b < batch; ++b){
        க்கு(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

வெறுமை forward_convolutional_layer(convolutional_layer l, network net)
{
    முழு i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    எனில்(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.இருமம்_weights);
        swap_இருமம்(&l);
        binarize_cpu(net.உள்ளீடு, l.c*l.h*l.w*l.batch, l.இருமம்_உள்ளீடு);
        net.உள்ளீடு = l.இருமம்_உள்ளீடு;
    }

    முழு m = l.n/l.groups;
    முழு k = l.size*l.size*l.c/l.groups;
    முழு n = l.out_w*l.out_h;
    க்கு(i = 0; i < l.batch; ++i){
        க்கு(j = 0; j < l.groups; ++j){
            தசம *a = l.weights + j*l.nweights/l.groups;
            தசம *b = net.workspace;
            தசம *c = l.output + (i*l.groups + j)*n*m;
            தசம *im =  net.உள்ளீடு + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            எனில் (l.size == 1) {
                b = im;
            } இல்லையெனில் {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    எனில்(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } இல்லையெனில் {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    எனில்(l.இருமம் || l.xnor) swap_இருமம்(&l);
}

வெறுமை backward_convolutional_layer(convolutional_layer l, network net)
{
    முழு i, j;
    முழு m = l.n/l.groups;
    முழு n = l.size*l.size*l.c/l.groups;
    முழு k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    எனில்(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } இல்லையெனில் {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    க்கு(i = 0; i < l.batch; ++i){
        க்கு(j = 0; j < l.groups; ++j){
            தசம *a = l.delta + (i*l.groups + j)*m*k;
            தசம *b = net.workspace;
            தசம *c = l.weight_updates + j*l.nweights/l.groups;

            தசம *im  = net.உள்ளீடு + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            தசம *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            எனில்(l.size == 1){
                b = im;
            } இல்லையெனில் {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            எனில் (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                எனில் (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                எனில் (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

வெறுமை update_convolutional_layer(convolutional_layer l, update_args a)
{
    தசம learning_rate = a.learning_rate*l.learning_rate_scale;
    தசம momentum = a.momentum;
    தசம decay = a.decay;
    முழு batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    எனில்(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, முழு i)
{
    முழு h = l.size;
    முழு w = l.size;
    முழு c = l.c/l.groups;
    திரும்பு float_to_image(w,h,c,l.weights+i*h*w*c);
}

வெறுமை rgbgr_weights(convolutional_layer l)
{
    முழு i;
    க்கு(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        எனில் (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

வெறுமை rescale_weights(convolutional_layer l, தசம scale, தசம trans)
{
    முழு i;
    க்கு(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        எனில் (im.c == 3) {
            scale_image(im, scale);
            தசம sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, அளவு(image));
    முழு i;
    க்கு(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    திரும்பு weights;
}

image *visualize_convolutional_layer(convolutional_layer l, எழுத்து *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    எழுத்து buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    திரும்பு single_weights;
}

