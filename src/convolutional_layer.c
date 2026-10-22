#شامل "convolutional_layer.h"
#شامل "utils.h"
#شامل "batchnorm_layer.h"
#شامل "im2col.h"
#شامل "col2im.h"
#شامل "blas.h"
#شامل "gemm.h"
#شامل <stdio.h>
#شامل <time.h>

#ifdef AI2
#شامل "xnor_layer.h"
#endif

تهی swap_دودویی(convolutional_layer *l)
{
    اعشاری *swap = l->weights;
    l->weights = l->دودویی_weights;
    l->دودویی_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->دودویی_weights_gpu;
    l->دودویی_weights_gpu = swap;
#endif
}

تهی binarize_weights(اعشاری *weights, صحیح n, صحیح size, اعشاری *دودویی)
{
    صحیح i, f;
    برای(f = 0; f < n; ++f){
        اعشاری mean = 0;
        برای(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        برای(i = 0; i < size; ++i){
            دودویی[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

تهی binarize_cpu(اعشاری *درون_داد, صحیح n, اعشاری *دودویی)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        دودویی[i] = (درون_داد[i] > 0) ? 1 : -1;
    }
}

تهی binarize_درون_داد(اعشاری *درون_داد, صحیح n, صحیح size, اعشاری *دودویی)
{
    صحیح i, s;
    برای(s = 0; s < size; ++s){
        اعشاری mean = 0;
        برای(i = 0; i < n; ++i){
            mean += fabs(درون_داد[i*size + s]);
        }
        mean = mean / n;
        برای(i = 0; i < n; ++i){
            دودویی[i*size + s] = (درون_داد[i*size + s] > 0) ? mean : -mean;
        }
    }
}

صحیح convolutional_out_height(convolutional_layer l)
{
    بازگشت (l.h + 2*l.pad - l.size) / l.stride + 1;
}

صحیح convolutional_out_width(convolutional_layer l)
{
    بازگشت (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    بازگشت float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    بازگشت float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

ایستا size_t get_workspace_size(layer l){
#ifdef CUDNN
    اگر(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        اگر (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        اگر (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        اگر (s > most) most = s;
        بازگشت most;
    }
#endif
    بازگشت (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*اندازه(اعشاری);
}

#ifdef GPU
#ifdef CUDNN
تهی cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #اگر CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #وگرنه
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #اگر CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_شمار(l->convDesc, l->groups);
    #وگرنه
    اگر(l->groups > 1){
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

convolutional_layer make_convolutional_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح groups, صحیح size, صحیح stride, صحیح padding, ACTIVATION activation, صحیح batch_normalize, صحیح دودویی, صحیح xnor, صحیح adam)
{
    صحیح i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.دودویی = دودویی;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, اندازه(اعشاری));
    l.weight_updates = calloc(c/groups*n*size*size, اندازه(اعشاری));

    l.biases = calloc(n, اندازه(اعشاری));
    l.bias_updates = calloc(n, اندازه(اعشاری));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    اعشاری scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    برای(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    صحیح out_w = convolutional_out_width(l);
    صحیح out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, اندازه(اعشاری));
    l.delta  = calloc(l.batch*l.outputs, اندازه(اعشاری));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    اگر(دودویی){
        l.دودویی_weights = calloc(l.nweights, اندازه(اعشاری));
        l.cweights = calloc(l.nweights, اندازه(نویسه));
        l.scales = calloc(n, اندازه(اعشاری));
    }
    اگر(xnor){
        l.دودویی_weights = calloc(l.nweights, اندازه(اعشاری));
        l.دودویی_درون_داد = calloc(l.inputs*l.batch, اندازه(اعشاری));
    }

    اگر(batch_normalize){
        l.scales = calloc(n, اندازه(اعشاری));
        l.scale_updates = calloc(n, اندازه(اعشاری));
        برای(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, اندازه(اعشاری));
        l.variance = calloc(n, اندازه(اعشاری));

        l.mean_delta = calloc(n, اندازه(اعشاری));
        l.variance_delta = calloc(n, اندازه(اعشاری));

        l.rolling_mean = calloc(n, اندازه(اعشاری));
        l.rolling_variance = calloc(n, اندازه(اعشاری));
        l.x = calloc(l.batch*l.outputs, اندازه(اعشاری));
        l.x_norm = calloc(l.batch*l.outputs, اندازه(اعشاری));
    }
    اگر(adam){
        l.m = calloc(l.nweights, اندازه(اعشاری));
        l.v = calloc(l.nweights, اندازه(اعشاری));
        l.bias_m = calloc(n, اندازه(اعشاری));
        l.scale_m = calloc(n, اندازه(اعشاری));
        l.bias_v = calloc(n, اندازه(اعشاری));
        l.scale_v = calloc(n, اندازه(اعشاری));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    اگر(gpu_index >= 0){
        اگر (adam) {
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

        اگر(دودویی){
            l.دودویی_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        اگر(xnor){
            l.دودویی_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.دودویی_درون_داد_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        اگر(batch_normalize){
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

    بازگشت l;
}

تهی denormalize_convolutional_layer(convolutional_layer l)
{
    صحیح i, j;
    برای(i = 0; i < l.n; ++i){
        اعشاری scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        برای(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

تهی resize_convolutional_layer(convolutional_layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;
    صحیح out_w = convolutional_out_width(*l);
    صحیح out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*اندازه(اعشاری));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*اندازه(اعشاری));
    اگر(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*اندازه(اعشاری));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*اندازه(اعشاری));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    اگر(l->batch_normalize){
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

تهی add_bias(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح size)
{
    صحیح i,j,b;
    برای(b = 0; b < batch; ++b){
        برای(i = 0; i < n; ++i){
            برای(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

تهی scale_bias(اعشاری *output, اعشاری *scales, صحیح batch, صحیح n, صحیح size)
{
    صحیح i,j,b;
    برای(b = 0; b < batch; ++b){
        برای(i = 0; i < n; ++i){
            برای(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

تهی backward_bias(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح size)
{
    صحیح i,b;
    برای(b = 0; b < batch; ++b){
        برای(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

تهی forward_convolutional_layer(convolutional_layer l, network net)
{
    صحیح i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    اگر(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.دودویی_weights);
        swap_دودویی(&l);
        binarize_cpu(net.درون_داد, l.c*l.h*l.w*l.batch, l.دودویی_درون_داد);
        net.درون_داد = l.دودویی_درون_داد;
    }

    صحیح m = l.n/l.groups;
    صحیح k = l.size*l.size*l.c/l.groups;
    صحیح n = l.out_w*l.out_h;
    برای(i = 0; i < l.batch; ++i){
        برای(j = 0; j < l.groups; ++j){
            اعشاری *a = l.weights + j*l.nweights/l.groups;
            اعشاری *b = net.workspace;
            اعشاری *c = l.output + (i*l.groups + j)*n*m;
            اعشاری *im =  net.درون_داد + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            اگر (l.size == 1) {
                b = im;
            } وگرنه {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    اگر(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } وگرنه {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    اگر(l.دودویی || l.xnor) swap_دودویی(&l);
}

تهی backward_convolutional_layer(convolutional_layer l, network net)
{
    صحیح i, j;
    صحیح m = l.n/l.groups;
    صحیح n = l.size*l.size*l.c/l.groups;
    صحیح k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    اگر(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } وگرنه {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    برای(i = 0; i < l.batch; ++i){
        برای(j = 0; j < l.groups; ++j){
            اعشاری *a = l.delta + (i*l.groups + j)*m*k;
            اعشاری *b = net.workspace;
            اعشاری *c = l.weight_updates + j*l.nweights/l.groups;

            اعشاری *im  = net.درون_داد + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            اعشاری *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            اگر(l.size == 1){
                b = im;
            } وگرنه {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            اگر (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                اگر (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                اگر (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

تهی update_convolutional_layer(convolutional_layer l, update_args a)
{
    اعشاری learning_rate = a.learning_rate*l.learning_rate_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    اگر(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, صحیح i)
{
    صحیح h = l.size;
    صحیح w = l.size;
    صحیح c = l.c/l.groups;
    بازگشت float_to_image(w,h,c,l.weights+i*h*w*c);
}

تهی rgbgr_weights(convolutional_layer l)
{
    صحیح i;
    برای(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        اگر (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

تهی rescale_weights(convolutional_layer l, اعشاری scale, اعشاری trans)
{
    صحیح i;
    برای(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        اگر (im.c == 3) {
            scale_image(im, scale);
            اعشاری sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, اندازه(image));
    صحیح i;
    برای(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    بازگشت weights;
}

image *visualize_convolutional_layer(convolutional_layer l, نویسه *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    نویسه buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    بازگشت single_weights;
}

