#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "batchnorm_layer.h"
#ಅನ್ತರ್ಭಾವ "im2col.h"
#ಅನ್ತರ್ಭಾವ "col2im.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ifdef AI2
#ಅನ್ತರ್ಭಾವ "xnor_layer.h"
#endif

ಶೂನ್ಯ swap_ದ್ವಿಮಾನ(convolutional_layer *l)
{
    ಭಿನ್ನ *swap = l->weights;
    l->weights = l->ದ್ವಿಮಾನ_weights;
    l->ದ್ವಿಮಾನ_weights = swap;

#ifdef GPU
    swap = l->weights_gpu;
    l->weights_gpu = l->ದ್ವಿಮಾನ_weights_gpu;
    l->ದ್ವಿಮಾನ_weights_gpu = swap;
#endif
}

ಶೂನ್ಯ binarize_weights(ಭಿನ್ನ *weights, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ *ದ್ವಿಮಾನ)
{
    ಪೂರ್ಣಾಙ್ಕ i, f;
    ಪ್ರತಿ(f = 0; f < n; ++f){
        ಭಿನ್ನ mean = 0;
        ಪ್ರತಿ(i = 0; i < size; ++i){
            mean += fabs(weights[f*size + i]);
        }
        mean = mean / size;
        ಪ್ರತಿ(i = 0; i < size; ++i){
            ದ್ವಿಮಾನ[f*size + i] = (weights[f*size + i] > 0) ? mean : -mean;
        }
    }
}

ಶೂನ್ಯ binarize_cpu(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *ದ್ವಿಮಾನ)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ದ್ವಿಮಾನ[i] = (ಒಳಹರಿವು[i] > 0) ? 1 : -1;
    }
}

ಶೂನ್ಯ binarize_ಒಳಹರಿವು(ಭಿನ್ನ *ಒಳಹರಿವು, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ *ದ್ವಿಮಾನ)
{
    ಪೂರ್ಣಾಙ್ಕ i, s;
    ಪ್ರತಿ(s = 0; s < size; ++s){
        ಭಿನ್ನ mean = 0;
        ಪ್ರತಿ(i = 0; i < n; ++i){
            mean += fabs(ಒಳಹರಿವು[i*size + s]);
        }
        mean = mean / n;
        ಪ್ರತಿ(i = 0; i < n; ++i){
            ದ್ವಿಮಾನ[i*size + s] = (ಒಳಹರಿವು[i*size + s] > 0) ? mean : -mean;
        }
    }
}

ಪೂರ್ಣಾಙ್ಕ convolutional_out_height(convolutional_layer l)
{
    ಪ್ರತ್ಯಾಗಮ (l.h + 2*l.pad - l.size) / l.stride + 1;
}

ಪೂರ್ಣಾಙ್ಕ convolutional_out_width(convolutional_layer l)
{
    ಪ್ರತ್ಯಾಗಮ (l.w + 2*l.pad - l.size) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    ಪ್ರತ್ಯಾಗಮ float_to_image(l.out_w,l.out_h,l.out_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    ಪ್ರತ್ಯಾಗಮ float_to_image(l.out_w,l.out_h,l.out_c,l.delta);
}

ಸ್ಥಿರ size_t get_workspace_size(layer l){
#ifdef CUDNN
    ಯದಿ(gpu_index >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnnGetConvolutionForwardWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.weightDesc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        ಯದಿ (s > most) most = s;
        cudnnGetConvolutionBackwardFilterWorkspaceSize(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        ಯದಿ (s > most) most = s;
        cudnnGetConvolutionBackwardDataWorkspaceSize(cudnn_handle(),
                l.weightDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        ಯದಿ (s > most) most = s;
        ಪ್ರತ್ಯಾಗಮ most;
    }
#endif
    ಪ್ರತ್ಯಾಗಮ (size_t)l.out_h*l.out_w*l.size*l.size*l.c/l.groups*ಪರಿಮಾಣ(ಭಿನ್ನ);
}

#ifdef GPU
#ifdef CUDNN
ಶೂನ್ಯ cudnn_convolutional_setup(layer *l)
{
    cudnnSetTensor4dDescriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 

    cudnnSetTensor4dDescriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
    cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 

    cudnnSetFilter4dDescriptor(l->dweightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    cudnnSetFilter4dDescriptor(l->weightDesc, CUDNN_DATA_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->size, l->size); 
    #ಯದಿ CUDNN_MAJOR >= 6
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
    #ಅನ್ಯಥಾ
    cudnnSetConvolution2dDescriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #endif

    #ಯದಿ CUDNN_MAJOR >= 7
    cudnn_Set_Convolution_Group_ಎಣಿಕೆ(l->convDesc, l->groups);
    #ಅನ್ಯಥಾ
    ಯದಿ(l->groups > 1){
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

convolutional_layer make_convolutional_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ groups, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ padding, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ, ಪೂರ್ಣಾಙ್ಕ xnor, ಪೂರ್ಣಾಙ್ಕ adam)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.ದ್ವಿಮಾನ = ದ್ವಿಮಾನ;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.size = size;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = calloc(c/groups*n*size*size, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.weight_updates = calloc(c/groups*n*size*size, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.biases = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.bias_updates = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.nweights = c/groups*n*size*size;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    ಭಿನ್ನ scale = sqrt(2./(size*size*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    ಪ್ರತಿ(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_normal();
    ಪೂರ್ಣಾಙ್ಕ out_w = convolutional_out_width(l);
    ಪೂರ್ಣಾಙ್ಕ out_h = convolutional_out_height(l);
    l.out_h = out_h;
    l.out_w = out_w;
    l.out_c = n;
    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = l.w * l.h * l.c;

    l.output = calloc(l.batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta  = calloc(l.batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    ಯದಿ(ದ್ವಿಮಾನ){
        l.ದ್ವಿಮಾನ_weights = calloc(l.nweights, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.cweights = calloc(l.nweights, ಪರಿಮಾಣ(ವರ್ಣ));
        l.scales = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
    }
    ಯದಿ(xnor){
        l.ದ್ವಿಮಾನ_weights = calloc(l.nweights, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.ದ್ವಿಮಾನ_ಒಳಹರಿವು = calloc(l.inputs*l.batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    }

    ಯದಿ(batch_normalize){
        l.scales = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.scale_updates = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಪ್ರತಿ(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.variance = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));

        l.mean_delta = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.variance_delta = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));

        l.rolling_mean = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.rolling_variance = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.x = calloc(l.batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.x_norm = calloc(l.batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    }
    ಯದಿ(adam){
        l.m = calloc(l.nweights, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.v = calloc(l.nweights, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.bias_m = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.scale_m = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.bias_v = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.scale_v = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
    }

#ifdef GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    ಯದಿ(gpu_index >= 0){
        ಯದಿ (adam) {
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

        ಯದಿ(ದ್ವಿಮಾನ){
            l.ದ್ವಿಮಾನ_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        ಯದಿ(xnor){
            l.ದ್ವಿಮಾನ_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.ದ್ವಿಮಾನ_ಒಳಹರಿವು_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        ಯದಿ(batch_normalize){
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

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ denormalize_convolutional_layer(convolutional_layer l)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        ಭಿನ್ನ scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        ಪ್ರತಿ(j = 0; j < l.c/l.groups*l.size*l.size; ++j){
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

ಶೂನ್ಯ resize_convolutional_layer(convolutional_layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    l->w = w;
    l->h = h;
    ಪೂರ್ಣಾಙ್ಕ out_w = convolutional_out_width(*l);
    ಪೂರ್ಣಾಙ್ಕ out_h = convolutional_out_height(*l);

    l->out_w = out_w;
    l->out_h = out_h;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಯದಿ(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*ಪರಿಮಾಣ(ಭಿನ್ನ));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    ಯದಿ(l->batch_normalize){
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

ಶೂನ್ಯ add_bias(ಭಿನ್ನ *output, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,b;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(i = 0; i < n; ++i){
            ಪ್ರತಿ(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] += biases[i];
            }
        }
    }
}

ಶೂನ್ಯ scale_bias(ಭಿನ್ನ *output, ಭಿನ್ನ *scales, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,b;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(i = 0; i < n; ++i){
            ಪ್ರತಿ(j = 0; j < size; ++j){
                output[(b*n + i)*size + j] *= scales[i];
            }
        }
    }
}

ಶೂನ್ಯ backward_bias(ಭಿನ್ನ *bias_updates, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size)
{
    ಪೂರ್ಣಾಙ್ಕ i,b;
    ಪ್ರತಿ(b = 0; b < batch; ++b){
        ಪ್ರತಿ(i = 0; i < n; ++i){
            bias_updates[i] += sum_array(delta+size*(i+b*n), size);
        }
    }
}

ಶೂನ್ಯ forward_convolutional_layer(convolutional_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ಯದಿ(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.size*l.size, l.ದ್ವಿಮಾನ_weights);
        swap_ದ್ವಿಮಾನ(&l);
        binarize_cpu(net.ಒಳಹರಿವು, l.c*l.h*l.w*l.batch, l.ದ್ವಿಮಾನ_ಒಳಹರಿವು);
        net.ಒಳಹರಿವು = l.ದ್ವಿಮಾನ_ಒಳಹರಿವು;
    }

    ಪೂರ್ಣಾಙ್ಕ m = l.n/l.groups;
    ಪೂರ್ಣಾಙ್ಕ k = l.size*l.size*l.c/l.groups;
    ಪೂರ್ಣಾಙ್ಕ n = l.out_w*l.out_h;
    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        ಪ್ರತಿ(j = 0; j < l.groups; ++j){
            ಭಿನ್ನ *a = l.weights + j*l.nweights/l.groups;
            ಭಿನ್ನ *b = net.workspace;
            ಭಿನ್ನ *c = l.output + (i*l.groups + j)*n*m;
            ಭಿನ್ನ *im =  net.ಒಳಹರಿವು + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ಯದಿ (l.size == 1) {
                b = im;
            } ಅನ್ಯಥಾ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    ಯದಿ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ಅನ್ಯಥಾ {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_h*l.out_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    ಯದಿ(l.ದ್ವಿಮಾನ || l.xnor) swap_ದ್ವಿಮಾನ(&l);
}

ಶೂನ್ಯ backward_convolutional_layer(convolutional_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪೂರ್ಣಾಙ್ಕ m = l.n/l.groups;
    ಪೂರ್ಣಾಙ್ಕ n = l.size*l.size*l.c/l.groups;
    ಪೂರ್ಣಾಙ್ಕ k = l.out_w*l.out_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ಯದಿ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ಅನ್ಯಥಾ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        ಪ್ರತಿ(j = 0; j < l.groups; ++j){
            ಭಿನ್ನ *a = l.delta + (i*l.groups + j)*m*k;
            ಭಿನ್ನ *b = net.workspace;
            ಭಿನ್ನ *c = l.weight_updates + j*l.nweights/l.groups;

            ಭಿನ್ನ *im  = net.ಒಳಹರಿವು + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            ಭಿನ್ನ *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ಯದಿ(l.size == 1){
                b = im;
            } ಅನ್ಯಥಾ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.size, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            ಯದಿ (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                ಯದಿ (l.size == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                ಯದಿ (l.size != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.size, l.stride, l.pad, imd);
                }
            }
        }
    }
}

ಶೂನ್ಯ update_convolutional_layer(convolutional_layer l, update_args a)
{
    ಭಿನ್ನ learning_rate = a.learning_rate*l.learning_rate_scale;
    ಭಿನ್ನ momentum = a.momentum;
    ಭಿನ್ನ decay = a.decay;
    ಪೂರ್ಣಾಙ್ಕ batch = a.batch;

    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ಯದಿ(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(l.nweights, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.weight_updates, 1);
}


image get_convolutional_weight(convolutional_layer l, ಪೂರ್ಣಾಙ್ಕ i)
{
    ಪೂರ್ಣಾಙ್ಕ h = l.size;
    ಪೂರ್ಣಾಙ್ಕ w = l.size;
    ಪೂರ್ಣಾಙ್ಕ c = l.c/l.groups;
    ಪ್ರತ್ಯಾಗಮ float_to_image(w,h,c,l.weights+i*h*w*c);
}

ಶೂನ್ಯ rgbgr_weights(convolutional_layer l)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        ಯದಿ (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

ಶೂನ್ಯ rescale_weights(convolutional_layer l, ಭಿನ್ನ scale, ಭಿನ್ನ trans)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        image im = get_convolutional_weight(l, i);
        ಯದಿ (im.c == 3) {
            scale_image(im, scale);
            ಭಿನ್ನ sum = sum_array(im.data, im.w*im.h*im.c);
            l.biases[i] += sum*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = calloc(l.n, ಪರಿಮಾಣ(image));
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_weight(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    ಪ್ರತ್ಯಾಗಮ weights;
}

image *visualize_convolutional_layer(convolutional_layer l, ವರ್ಣ *window, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, window);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    ವರ್ಣ buff[256];
    sprintf(buff, "%s: Output", window);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    ಪ್ರತ್ಯಾಗಮ single_weights;
}

