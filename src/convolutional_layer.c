#অন্তর্ভাব "convolutional_layer.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "batchnorm_layer.h"
#অন্তর্ভাব "im2col.h"
#অন্তর্ভাব "col2im.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "gemm.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <time.h>

#ifdef AI2
#অন্তর্ভাব "xnor_layer.h"
#endif

শূন্য swap_দ্বিমিক(convolutional_layer *l)
{
    ভিন্ন *swap = l->weights;
    l->weights = l->দ্বিমিক_weights;
    l->দ্বিমিক_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->দ্বিমিক_weights_gpu;
    l->দ্বিমিক_weights_gpu = swap;
#endif
}

শূন্য binarize_weights(ভিন্ন *weights, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, ভিন্ন *দ্বিমিক)
{
    পূর্ণাঙ্ক i, f;
    প্রতি(f = 0; f < n; ++f){
        ভিন্ন mean = 0;
        প্রতি(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        প্রতি(i = 0; i < size; ++i){
            দ্বিমিক[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

শূন্য binarize_cpu(ভিন্ন *প্রবেশ, পূর্ণাঙ্ক n, ভিন্ন *দ্বিমিক)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < n; ++i){
        দ্বিমিক[i] = (প্রবেশ[i] > 0) ? 1 : -1;
    }
}

শূন্য binarize_প্রবেশ(ভিন্ন *প্রবেশ, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, ভিন্ন *দ্বিমিক)
{
    পূর্ণাঙ্ক i, s;
    প্রতি(s = 0; s < size; ++s){
        ভিন্ন mean = 0;
        প্রতি(i = 0; i < n; ++i){
            mean += fabs(প্রবেশ[i*size + s]);
        }
        mean = mean / n;
        প্রতি(i = 0; i < n; ++i){
            দ্বিমিক[i*size + s] = (প্রবেশ[i*size + s] > 0) ? mean : -mean;
        }
    }
}

পূর্ণাঙ্ক convolutional_out_height(convolutional_layer l)
{
    প্রত্যাগম (l.h + 2*l.pad - l.size) / l.stride + 1;
}

পূর্ণাঙ্ক convolutional_out_width(convolutional_layer l)
{
    প্রত্যাগম (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    প্রত্যাগম float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    প্রত্যাগম float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

স্থির size_t get_workspace_size(layer l){
#ifdef CUDNN
    যদি(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        যদি (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        যদি (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        যদি (s > most) most = s;
        প্রত্যাগম most;
    }
#endif
    প্রত্যাগম (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*পরিমাণ(ভিন্ন);
}

#ifdef GPU
#ifdef CUDNN
শূন্য cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #যদি CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #অন্যথা
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #যদি CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_গুনতি(l->convDesc, l->groups);
    #অন্যথা
    যদি(l->groups > 1){
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

convolutional_layer make_convolutional_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক n, পূর্ণাঙ্ক groups, পূর্ণাঙ্ক size, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক padding, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize, পূর্ণাঙ্ক দ্বিমিক, পূর্ণাঙ্ক xnor, পূর্ণাঙ্ক adam)
{
    পূর্ণাঙ্ক i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.দ্বিমিক = দ্বিমিক;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, পরিমাণ(ভিন্ন));
    l.weight_updates = calloc(c/groups*n*size*size, পরিমাণ(ভিন্ন));

    l.biases = calloc(n, পরিমাণ(ভিন্ন));
    l.bias_updates = calloc(n, পরিমাণ(ভিন্ন));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    ভিন্ন scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    প্রতি(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    পূর্ণাঙ্ক out_w = convolutional_out_width(l);
    পূর্ণাঙ্ক out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, পরিমাণ(ভিন্ন));
    l.delta  = calloc(l.batch*l.outputs, পরিমাণ(ভিন্ন));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    যদি(দ্বিমিক){
        l.দ্বিমিক_weights = calloc(l.nweights, পরিমাণ(ভিন্ন));
        l.cweights = calloc(l.nweights, পরিমাণ(বর্ণ));
        l.scales = calloc(n, পরিমাণ(ভিন্ন));
    }
    যদি(xnor){
        l.দ্বিমিক_weights = calloc(l.nweights, পরিমাণ(ভিন্ন));
        l.দ্বিমিক_প্রবেশ = calloc(l.inputs*l.batch, পরিমাণ(ভিন্ন));
    }

    যদি(batch_normalize){
        l.scales = calloc(n, পরিমাণ(ভিন্ন));
        l.scale_updates = calloc(n, পরিমাণ(ভিন্ন));
        প্রতি(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, পরিমাণ(ভিন্ন));
        l.variance = calloc(n, পরিমাণ(ভিন্ন));

        l.mean_delta = calloc(n, পরিমাণ(ভিন্ন));
        l.variance_delta = calloc(n, পরিমাণ(ভিন্ন));

        l.rolling_mean = calloc(n, পরিমাণ(ভিন্ন));
        l.rolling_variance = calloc(n, পরিমাণ(ভিন্ন));
        l.x = calloc(l.batch*l.outputs, পরিমাণ(ভিন্ন));
        l.x_norm = calloc(l.batch*l.outputs, পরিমাণ(ভিন্ন));
    }
    যদি(adam){
        l.m = calloc(l.nweights, পরিমাণ(ভিন্ন));
        l.v = calloc(l.nweights, পরিমাণ(ভিন্ন));
        l.bias_m = calloc(n, পরিমাণ(ভিন্ন));
        l.scale_m = calloc(n, পরিমাণ(ভিন্ন));
        l.bias_v = calloc(n, পরিমাণ(ভিন্ন));
        l.scale_v = calloc(n, পরিমাণ(ভিন্ন));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    যদি(gpu_index >= 0){
        যদি (adam) {
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

        যদি(দ্বিমিক){
            l.দ্বিমিক_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        যদি(xnor){
            l.দ্বিমিক_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.দ্বিমিক_প্রবেশ_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        যদি(batch_normalize){
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

    প্রত্যাগম l;
}

শূন্য denormalize_convolutional_layer(convolutional_layer l)
{
    পূর্ণাঙ্ক i, j;
    প্রতি(i = 0; i < l.n; ++i){
        ভিন্ন scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        প্রতি(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

শূন্য resize_convolutional_layer(convolutional_layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    l->w = w;
    l->h = h;
    পূর্ণাঙ্ক out_w = convolutional_out_width(*l);
    পূর্ণাঙ্ক out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*পরিমাণ(ভিন্ন));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*পরিমাণ(ভিন্ন));
    যদি(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*পরিমাণ(ভিন্ন));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*পরিমাণ(ভিন্ন));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    যদি(l->batch_normalize){
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

শূন্য add_bias(ভিন্ন *output, ভিন্ন *biases, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size)
{
    পূর্ণাঙ্ক i,j,b;
    প্রতি(b = 0; b < batch; ++b){
        প্রতি(i = 0; i < n; ++i){
            প্রতি(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

শূন্য scale_bias(ভিন্ন *output, ভিন্ন *scales, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size)
{
    পূর্ণাঙ্ক i,j,b;
    প্রতি(b = 0; b < batch; ++b){
        প্রতি(i = 0; i < n; ++i){
            প্রতি(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

শূন্য backward_bias(ভিন্ন *bias_updates, ভিন্ন *delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size)
{
    পূর্ণাঙ্ক i,b;
    প্রতি(b = 0; b < batch; ++b){
        প্রতি(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

শূন্য forward_convolutional_layer(convolutional_layer l, network net)
{
    পূর্ণাঙ্ক i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    যদি(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.দ্বিমিক_weights);
        swap_দ্বিমিক(&l);
        binarize_cpu(net.প্রবেশ, l.c*l.h*l.w*l.batch, l.দ্বিমিক_প্রবেশ);
        net.প্রবেশ = l.দ্বিমিক_প্রবেশ;
    }

    পূর্ণাঙ্ক m = l.n/l.groups;
    পূর্ণাঙ্ক k = l.size*l.size*l.c/l.groups;
    পূর্ণাঙ্ক n = l.out_w*l.out_h;
    প্রতি(i = 0; i < l.batch; ++i){
        প্রতি(j = 0; j < l.groups; ++j){
            ভিন্ন *a = l.weights + j*l.nweights/l.groups;
            ভিন্ন *b = net.workspace;
            ভিন্ন *c = l.output + (i*l.groups + j)*n*m;
            ভিন্ন *im =  net.প্রবেশ + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            যদি (l.size == 1) {
                b = im;
            } অন্যথা {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    যদি(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } অন্যথা {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    যদি(l.দ্বিমিক || l.xnor) swap_দ্বিমিক(&l);
}

শূন্য backward_convolutional_layer(convolutional_layer l, network net)
{
    পূর্ণাঙ্ক i, j;
    পূর্ণাঙ্ক m = l.n/l.groups;
    পূর্ণাঙ্ক n = l.size*l.size*l.c/l.groups;
    পূর্ণাঙ্ক k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    যদি(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } অন্যথা {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    প্রতি(i = 0; i < l.batch; ++i){
        প্রতি(j = 0; j < l.groups; ++j){
            ভিন্ন *a = l.delta + (i*l.groups + j)*m*k;
            ভিন্ন *b = net.workspace;
            ভিন্ন *c = l.weight_updates + j*l.nweights/l.groups;

            ভিন্ন *im  = net.প্রবেশ + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            ভিন্ন *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            যদি(l.size == 1){
                b = im;
            } অন্যথা {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            যদি (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                যদি (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                যদি (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

শূন্য update_convolutional_layer(convolutional_layer l, update_args a)
{
    ভিন্ন learning_rate = a.learning_rate*l.learning_rate_scale;
    ভিন্ন momentum = a.momentum;
    ভিন্ন decay = a.decay;
    পূর্ণাঙ্ক batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    যদি(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, পূর্ণাঙ্ক i)
{
    পূর্ণাঙ্ক h = l.size;
    পূর্ণাঙ্ক w = l.size;
    পূর্ণাঙ্ক c = l.c/l.groups;
    প্রত্যাগম float_to_image(w,h,c,l.weights+i*h*w*c);
}

শূন্য rgbgr_weights(convolutional_layer l)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        যদি (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

শূন্য rescale_weights(convolutional_layer l, ভিন্ন scale, ভিন্ন trans)
{
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        যদি (im.c == 3) {
            scale_image(im, scale);
            ভিন্ন sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, পরিমাণ(image));
    পূর্ণাঙ্ক i;
    প্রতি(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    প্রত্যাগম weights;
}

image *visualize_convolutional_layer(convolutional_layer l, বর্ণ *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    বর্ণ buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    প্রত্যাগম single_weights;
}

