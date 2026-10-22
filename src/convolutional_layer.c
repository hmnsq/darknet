#അന്തര്ഭാവ "convolutional_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "batchnorm_layer.h"
#അന്തര്ഭാവ "im2col.h"
#അന്തര്ഭാവ "col2im.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "gemm.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <time.h>

#ifdef AI2
#അന്തര്ഭാവ "xnor_layer.h"
#endif

ശൂന്യം swap_ദ്വിമാന(convolutional_layer *l)
{
    ദശാംശം *swap = l->weights;
    l->weights = l->ദ്വിമാന_weights;
    l->ദ്വിമാന_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->ദ്വിമാന_weights_gpu;
    l->ദ്വിമാന_weights_gpu = swap;
#endif
}

ശൂന്യം binarize_weights(ദശാംശം *weights, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, ദശാംശം *ദ്വിമാന)
{
    പൂർണ്ണസംഖ്യ i, f;
    വേണ്ടി(f = 0; f < n; ++f){
        ദശാംശം mean = 0;
        വേണ്ടി(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        വേണ്ടി(i = 0; i < size; ++i){
            ദ്വിമാന[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

ശൂന്യം binarize_cpu(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ n, ദശാംശം *ദ്വിമാന)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        ദ്വിമാന[i] = (നിവേശം[i] > 0) ? 1 : -1;
    }
}

ശൂന്യം binarize_നിവേശം(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, ദശാംശം *ദ്വിമാന)
{
    പൂർണ്ണസംഖ്യ i, s;
    വേണ്ടി(s = 0; s < size; ++s){
        ദശാംശം mean = 0;
        വേണ്ടി(i = 0; i < n; ++i){
            mean += fabs(നിവേശം[i*size + s]);
        }
        mean = mean / n;
        വേണ്ടി(i = 0; i < n; ++i){
            ദ്വിമാന[i*size + s] = (നിവേശം[i*size + s] > 0) ? mean : -mean;
        }
    }
}

പൂർണ്ണസംഖ്യ convolutional_out_height(convolutional_layer l)
{
    തിരികെ (l.h + 2*l.pad - l.size) / l.stride + 1;
}

പൂർണ്ണസംഖ്യ convolutional_out_width(convolutional_layer l)
{
    തിരികെ (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    തിരികെ float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    തിരികെ float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

സ്ഥിതം size_t get_workspace_size(layer l){
#ifdef CUDNN
    എങ്കിൽ(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        എങ്കിൽ (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        എങ്കിൽ (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        എങ്കിൽ (s > most) most = s;
        തിരികെ most;
    }
#endif
    തിരികെ (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*വലുപ്പം(ദശാംശം);
}

#ifdef GPU
#ifdef CUDNN
ശൂന്യം cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #എങ്കിൽ CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #അല്ലെങ്കിൽ
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #എങ്കിൽ CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_എണ്ണം(l->convDesc, l->groups);
    #അല്ലെങ്കിൽ
    എങ്കിൽ(l->groups > 1){
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

convolutional_layer make_convolutional_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ groups, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ padding, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ ദ്വിമാന, പൂർണ്ണസംഖ്യ xnor, പൂർണ്ണസംഖ്യ adam)
{
    പൂർണ്ണസംഖ്യ i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.ദ്വിമാന = ദ്വിമാന;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, വലുപ്പം(ദശാംശം));
    l.weight_updates = calloc(c/groups*n*size*size, വലുപ്പം(ദശാംശം));

    l.biases = calloc(n, വലുപ്പം(ദശാംശം));
    l.bias_updates = calloc(n, വലുപ്പം(ദശാംശം));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    ദശാംശം scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    വേണ്ടി(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    പൂർണ്ണസംഖ്യ out_w = convolutional_out_width(l);
    പൂർണ്ണസംഖ്യ out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, വലുപ്പം(ദശാംശം));
    l.delta  = calloc(l.batch*l.outputs, വലുപ്പം(ദശാംശം));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    എങ്കിൽ(ദ്വിമാന){
        l.ദ്വിമാന_weights = calloc(l.nweights, വലുപ്പം(ദശാംശം));
        l.cweights = calloc(l.nweights, വലുപ്പം(അക്ഷരം));
        l.scales = calloc(n, വലുപ്പം(ദശാംശം));
    }
    എങ്കിൽ(xnor){
        l.ദ്വിമാന_weights = calloc(l.nweights, വലുപ്പം(ദശാംശം));
        l.ദ്വിമാന_നിവേശം = calloc(l.inputs*l.batch, വലുപ്പം(ദശാംശം));
    }

    എങ്കിൽ(batch_normalize){
        l.scales = calloc(n, വലുപ്പം(ദശാംശം));
        l.scale_updates = calloc(n, വലുപ്പം(ദശാംശം));
        വേണ്ടി(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, വലുപ്പം(ദശാംശം));
        l.variance = calloc(n, വലുപ്പം(ദശാംശം));

        l.mean_delta = calloc(n, വലുപ്പം(ദശാംശം));
        l.variance_delta = calloc(n, വലുപ്പം(ദശാംശം));

        l.rolling_mean = calloc(n, വലുപ്പം(ദശാംശം));
        l.rolling_variance = calloc(n, വലുപ്പം(ദശാംശം));
        l.x = calloc(l.batch*l.outputs, വലുപ്പം(ദശാംശം));
        l.x_norm = calloc(l.batch*l.outputs, വലുപ്പം(ദശാംശം));
    }
    എങ്കിൽ(adam){
        l.m = calloc(l.nweights, വലുപ്പം(ദശാംശം));
        l.v = calloc(l.nweights, വലുപ്പം(ദശാംശം));
        l.bias_m = calloc(n, വലുപ്പം(ദശാംശം));
        l.scale_m = calloc(n, വലുപ്പം(ദശാംശം));
        l.bias_v = calloc(n, വലുപ്പം(ദശാംശം));
        l.scale_v = calloc(n, വലുപ്പം(ദശാംശം));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    എങ്കിൽ(gpu_index >= 0){
        എങ്കിൽ (adam) {
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

        എങ്കിൽ(ദ്വിമാന){
            l.ദ്വിമാന_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        എങ്കിൽ(xnor){
            l.ദ്വിമാന_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.ദ്വിമാന_നിവേശം_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        എങ്കിൽ(batch_normalize){
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

    തിരികെ l;
}

ശൂന്യം denormalize_convolutional_layer(convolutional_layer l)
{
    പൂർണ്ണസംഖ്യ i, j;
    വേണ്ടി(i = 0; i < l.n; ++i){
        ദശാംശം scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        വേണ്ടി(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

ശൂന്യം resize_convolutional_layer(convolutional_layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    l->w = w;
    l->h = h;
    പൂർണ്ണസംഖ്യ out_w = convolutional_out_width(*l);
    പൂർണ്ണസംഖ്യ out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*വലുപ്പം(ദശാംശം));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*വലുപ്പം(ദശാംശം));
    എങ്കിൽ(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*വലുപ്പം(ദശാംശം));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*വലുപ്പം(ദശാംശം));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    എങ്കിൽ(l->batch_normalize){
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

ശൂന്യം add_bias(ദശാംശം *output, ദശാംശം *biases, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size)
{
    പൂർണ്ണസംഖ്യ i,j,b;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(i = 0; i < n; ++i){
            വേണ്ടി(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

ശൂന്യം scale_bias(ദശാംശം *output, ദശാംശം *scales, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size)
{
    പൂർണ്ണസംഖ്യ i,j,b;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(i = 0; i < n; ++i){
            വേണ്ടി(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

ശൂന്യം backward_bias(ദശാംശം *bias_updates, ദശാംശം *delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size)
{
    പൂർണ്ണസംഖ്യ i,b;
    വേണ്ടി(b = 0; b < batch; ++b){
        വേണ്ടി(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

ശൂന്യം forward_convolutional_layer(convolutional_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    എങ്കിൽ(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.ദ്വിമാന_weights);
        swap_ദ്വിമാന(&l);
        binarize_cpu(net.നിവേശം, l.c*l.h*l.w*l.batch, l.ദ്വിമാന_നിവേശം);
        net.നിവേശം = l.ദ്വിമാന_നിവേശം;
    }

    പൂർണ്ണസംഖ്യ m = l.n/l.groups;
    പൂർണ്ണസംഖ്യ k = l.size*l.size*l.c/l.groups;
    പൂർണ്ണസംഖ്യ n = l.out_w*l.out_h;
    വേണ്ടി(i = 0; i < l.batch; ++i){
        വേണ്ടി(j = 0; j < l.groups; ++j){
            ദശാംശം *a = l.weights + j*l.nweights/l.groups;
            ദശാംശം *b = net.workspace;
            ദശാംശം *c = l.output + (i*l.groups + j)*n*m;
            ദശാംശം *im =  net.നിവേശം + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            എങ്കിൽ (l.size == 1) {
                b = im;
            } അല്ലെങ്കിൽ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    എങ്കിൽ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } അല്ലെങ്കിൽ {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    എങ്കിൽ(l.ദ്വിമാന || l.xnor) swap_ദ്വിമാന(&l);
}

ശൂന്യം backward_convolutional_layer(convolutional_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i, j;
    പൂർണ്ണസംഖ്യ m = l.n/l.groups;
    പൂർണ്ണസംഖ്യ n = l.size*l.size*l.c/l.groups;
    പൂർണ്ണസംഖ്യ k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    എങ്കിൽ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } അല്ലെങ്കിൽ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    വേണ്ടി(i = 0; i < l.batch; ++i){
        വേണ്ടി(j = 0; j < l.groups; ++j){
            ദശാംശം *a = l.delta + (i*l.groups + j)*m*k;
            ദശാംശം *b = net.workspace;
            ദശാംശം *c = l.weight_updates + j*l.nweights/l.groups;

            ദശാംശം *im  = net.നിവേശം + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            ദശാംശം *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            എങ്കിൽ(l.size == 1){
                b = im;
            } അല്ലെങ്കിൽ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            എങ്കിൽ (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                എങ്കിൽ (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                എങ്കിൽ (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

ശൂന്യം update_convolutional_layer(convolutional_layer l, update_args a)
{
    ദശാംശം learning_rate = a.learning_rate*l.learning_rate_scale;
    ദശാംശം momentum = a.momentum;
    ദശാംശം decay = a.decay;
    പൂർണ്ണസംഖ്യ batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    എങ്കിൽ(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, പൂർണ്ണസംഖ്യ i)
{
    പൂർണ്ണസംഖ്യ h = l.size;
    പൂർണ്ണസംഖ്യ w = l.size;
    പൂർണ്ണസംഖ്യ c = l.c/l.groups;
    തിരികെ float_to_image(w,h,c,l.weights+i*h*w*c);
}

ശൂന്യം rgbgr_weights(convolutional_layer l)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        എങ്കിൽ (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

ശൂന്യം rescale_weights(convolutional_layer l, ദശാംശം scale, ദശാംശം trans)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        എങ്കിൽ (im.c == 3) {
            scale_image(im, scale);
            ദശാംശം sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, വലുപ്പം(image));
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    തിരികെ weights;
}

image *visualize_convolutional_layer(convolutional_layer l, അക്ഷരം *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    അക്ഷരം buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    തിരികെ single_weights;
}

