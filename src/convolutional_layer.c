#અન્તર્ભાવ "convolutional_layer.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "batchnorm_layer.h"
#અન્તર્ભાવ "im2col.h"
#અન્તર્ભાવ "col2im.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "gemm.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <time.h>

#ifdef AI2
#અન્તર્ભાવ "xnor_layer.h"
#endif

શૂન્ય swap_દ્વિઅંકી(convolutional_layer *l)
{
    દશાંશ *swap = l->weights;
    l->weights = l->દ્વિઅંકી_weights;
    l->દ્વિઅંકી_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->દ્વિઅંકી_weights_gpu;
    l->દ્વિઅંકી_weights_gpu = swap;
#endif
}

શૂન્ય binarize_weights(દશાંશ *weights, પૂર્ણાંક n, પૂર્ણાંક size, દશાંશ *દ્વિઅંકી)
{
    પૂર્ણાંક i, f;
    માટે(f = 0; f < n; ++f){
        દશાંશ mean = 0;
        માટે(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        માટે(i = 0; i < size; ++i){
            દ્વિઅંકી[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

શૂન્ય binarize_cpu(દશાંશ *આગત, પૂર્ણાંક n, દશાંશ *દ્વિઅંકી)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        દ્વિઅંકી[i] = (આગત[i] > 0) ? 1 : -1;
    }
}

શૂન્ય binarize_આગત(દશાંશ *આગત, પૂર્ણાંક n, પૂર્ણાંક size, દશાંશ *દ્વિઅંકી)
{
    પૂર્ણાંક i, s;
    માટે(s = 0; s < size; ++s){
        દશાંશ mean = 0;
        માટે(i = 0; i < n; ++i){
            mean += fabs(આગત[i*size + s]);
        }
        mean = mean / n;
        માટે(i = 0; i < n; ++i){
            દ્વિઅંકી[i*size + s] = (આગત[i*size + s] > 0) ? mean : -mean;
        }
    }
}

પૂર્ણાંક convolutional_out_height(convolutional_layer l)
{
    પાછા (l.h + 2*l.pad - l.size) / l.stride + 1;
}

પૂર્ણાંક convolutional_out_width(convolutional_layer l)
{
    પાછા (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    પાછા float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    પાછા float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

સ્થિર size_t get_workspace_size(layer l){
#ifdef CUDNN
    જો(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        જો (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        જો (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        જો (s > most) most = s;
        પાછા most;
    }
#endif
    પાછા (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*કદ(દશાંશ);
}

#ifdef GPU
#ifdef CUDNN
શૂન્ય cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #જો CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #નહીંતર
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #જો CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_ગણતરી(l->convDesc, l->groups);
    #નહીંતર
    જો(l->groups > 1){
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

convolutional_layer make_convolutional_layer(પૂર્ણાંક batch, પૂર્ણાંક h, પૂર્ણાંક w, પૂર્ણાંક c, પૂર્ણાંક n, પૂર્ણાંક groups, પૂર્ણાંક size, પૂર્ણાંક stride, પૂર્ણાંક padding, ACTIVATION activation, પૂર્ણાંક batch_normalize, પૂર્ણાંક દ્વિઅંકી, પૂર્ણાંક xnor, પૂર્ણાંક adam)
{
    પૂર્ણાંક i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.દ્વિઅંકી = દ્વિઅંકી;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, કદ(દશાંશ));
    l.weight_updates = calloc(c/groups*n*size*size, કદ(દશાંશ));

    l.biases = calloc(n, કદ(દશાંશ));
    l.bias_updates = calloc(n, કદ(દશાંશ));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    દશાંશ scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    માટે(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    પૂર્ણાંક out_w = convolutional_out_width(l);
    પૂર્ણાંક out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, કદ(દશાંશ));
    l.delta  = calloc(l.batch*l.outputs, કદ(દશાંશ));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    જો(દ્વિઅંકી){
        l.દ્વિઅંકી_weights = calloc(l.nweights, કદ(દશાંશ));
        l.cweights = calloc(l.nweights, કદ(અક્ષર));
        l.scales = calloc(n, કદ(દશાંશ));
    }
    જો(xnor){
        l.દ્વિઅંકી_weights = calloc(l.nweights, કદ(દશાંશ));
        l.દ્વિઅંકી_આગત = calloc(l.inputs*l.batch, કદ(દશાંશ));
    }

    જો(batch_normalize){
        l.scales = calloc(n, કદ(દશાંશ));
        l.scale_updates = calloc(n, કદ(દશાંશ));
        માટે(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, કદ(દશાંશ));
        l.variance = calloc(n, કદ(દશાંશ));

        l.mean_delta = calloc(n, કદ(દશાંશ));
        l.variance_delta = calloc(n, કદ(દશાંશ));

        l.rolling_mean = calloc(n, કદ(દશાંશ));
        l.rolling_variance = calloc(n, કદ(દશાંશ));
        l.x = calloc(l.batch*l.outputs, કદ(દશાંશ));
        l.x_norm = calloc(l.batch*l.outputs, કદ(દશાંશ));
    }
    જો(adam){
        l.m = calloc(l.nweights, કદ(દશાંશ));
        l.v = calloc(l.nweights, કદ(દશાંશ));
        l.bias_m = calloc(n, કદ(દશાંશ));
        l.scale_m = calloc(n, કદ(દશાંશ));
        l.bias_v = calloc(n, કદ(દશાંશ));
        l.scale_v = calloc(n, કદ(દશાંશ));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    જો(gpu_index >= 0){
        જો (adam) {
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

        જો(દ્વિઅંકી){
            l.દ્વિઅંકી_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        જો(xnor){
            l.દ્વિઅંકી_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.દ્વિઅંકી_આગત_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        જો(batch_normalize){
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

    પાછા l;
}

શૂન્ય denormalize_convolutional_layer(convolutional_layer l)
{
    પૂર્ણાંક i, j;
    માટે(i = 0; i < l.n; ++i){
        દશાંશ scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        માટે(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

શૂન્ય resize_convolutional_layer(convolutional_layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    l->w = w;
    l->h = h;
    પૂર્ણાંક out_w = convolutional_out_width(*l);
    પૂર્ણાંક out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*કદ(દશાંશ));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*કદ(દશાંશ));
    જો(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*કદ(દશાંશ));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*કદ(દશાંશ));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    જો(l->batch_normalize){
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

શૂન્ય add_bias(દશાંશ *output, દશાંશ *biases, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size)
{
    પૂર્ણાંક i,j,b;
    માટે(b = 0; b < batch; ++b){
        માટે(i = 0; i < n; ++i){
            માટે(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

શૂન્ય scale_bias(દશાંશ *output, દશાંશ *scales, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size)
{
    પૂર્ણાંક i,j,b;
    માટે(b = 0; b < batch; ++b){
        માટે(i = 0; i < n; ++i){
            માટે(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

શૂન્ય backward_bias(દશાંશ *bias_updates, દશાંશ *delta, પૂર્ણાંક batch, પૂર્ણાંક n, પૂર્ણાંક size)
{
    પૂર્ણાંક i,b;
    માટે(b = 0; b < batch; ++b){
        માટે(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

શૂન્ય forward_convolutional_layer(convolutional_layer l, network net)
{
    પૂર્ણાંક i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    જો(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.દ્વિઅંકી_weights);
        swap_દ્વિઅંકી(&l);
        binarize_cpu(net.આગત, l.c*l.h*l.w*l.batch, l.દ્વિઅંકી_આગત);
        net.આગત = l.દ્વિઅંકી_આગત;
    }

    પૂર્ણાંક m = l.n/l.groups;
    પૂર્ણાંક k = l.size*l.size*l.c/l.groups;
    પૂર્ણાંક n = l.out_w*l.out_h;
    માટે(i = 0; i < l.batch; ++i){
        માટે(j = 0; j < l.groups; ++j){
            દશાંશ *a = l.weights + j*l.nweights/l.groups;
            દશાંશ *b = net.workspace;
            દશાંશ *c = l.output + (i*l.groups + j)*n*m;
            દશાંશ *im =  net.આગત + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            જો (l.size == 1) {
                b = im;
            } નહીંતર {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    જો(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } નહીંતર {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    જો(l.દ્વિઅંકી || l.xnor) swap_દ્વિઅંકી(&l);
}

શૂન્ય backward_convolutional_layer(convolutional_layer l, network net)
{
    પૂર્ણાંક i, j;
    પૂર્ણાંક m = l.n/l.groups;
    પૂર્ણાંક n = l.size*l.size*l.c/l.groups;
    પૂર્ણાંક k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    જો(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } નહીંતર {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    માટે(i = 0; i < l.batch; ++i){
        માટે(j = 0; j < l.groups; ++j){
            દશાંશ *a = l.delta + (i*l.groups + j)*m*k;
            દશાંશ *b = net.workspace;
            દશાંશ *c = l.weight_updates + j*l.nweights/l.groups;

            દશાંશ *im  = net.આગત + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            દશાંશ *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            જો(l.size == 1){
                b = im;
            } નહીંતર {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            જો (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                જો (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                જો (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

શૂન્ય update_convolutional_layer(convolutional_layer l, update_args a)
{
    દશાંશ learning_rate = a.learning_rate*l.learning_rate_scale;
    દશાંશ momentum = a.momentum;
    દશાંશ decay = a.decay;
    પૂર્ણાંક batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    જો(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, પૂર્ણાંક i)
{
    પૂર્ણાંક h = l.size;
    પૂર્ણાંક w = l.size;
    પૂર્ણાંક c = l.c/l.groups;
    પાછા float_to_image(w,h,c,l.weights+i*h*w*c);
}

શૂન્ય rgbgr_weights(convolutional_layer l)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        જો (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

શૂન્ય rescale_weights(convolutional_layer l, દશાંશ scale, દશાંશ trans)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        જો (im.c == 3) {
            scale_image(im, scale);
            દશાંશ sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, કદ(image));
    પૂર્ણાંક i;
    માટે(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    પાછા weights;
}

image *visualize_convolutional_layer(convolutional_layer l, અક્ષર *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    અક્ષર buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    પાછા single_weights;
}

