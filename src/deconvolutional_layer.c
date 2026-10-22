#ಅನ್ತರ್ಭಾವ "deconvolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "batchnorm_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "im2col.h"
#ಅನ್ತರ್ಭಾವ "col2im.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>


ಸ್ಥಿರ size_t get_workspace_size(layer l){
    ಪ್ರತ್ಯಾಗಮ (size_t)l.h*l.w*l.size*l.size*l.n*ಪರಿಮಾಣ(ಭಿನ್ನ);
}

ಶೂನ್ಯ bilinear_init(layer l)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,f;
    ಭಿನ್ನ center = (l.size-1) / 2.;
    ಪ್ರತಿ(f = 0; f < l.n; ++f){
        ಪ್ರತಿ(j = 0; j < l.size; ++j){
            ಪ್ರತಿ(i = 0; i < l.size; ++i){
                ಭಿನ್ನ val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                ಪೂರ್ಣಾಙ್ಕ c = f%l.c;
                ಪೂರ್ಣಾಙ್ಕ ind = f*l.size*l.size*l.c + c*l.size*l.size + j*l.size + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ padding, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ adam)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    layer l = {0};
    l.type = DECONVOLUTIONAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.size = size;

    l.nweights = c*n*size*size;
    l.nbiases = n;

    l.weights = calloc(c*n*size*size, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.weight_updates = calloc(c*n*size*size, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.biases = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.bias_updates = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    ಭಿನ್ನ scale = .02;
    ಪ್ರತಿ(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_normal();
    //bilinear_init(l);
    ಪ್ರತಿ(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.out_h = (l.h - 1) * l.stride + l.size - 2*l.pad;
    l.out_w = (l.w - 1) * l.stride + l.size - 2*l.pad;
    l.out_c = n;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (ಭಿನ್ನ)l.out_w*l.out_h/(l.w*l.h), l.weights, 1);

    l.output = calloc(l.batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.delta  = calloc(l.batch*l.outputs, ಪರಿಮಾಣ(ಭಿನ್ನ));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

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
        l.m = calloc(c*n*size*size, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.v = calloc(c*n*size*size, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.bias_m = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.scale_m = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.bias_v = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
        l.scale_v = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ));
    }

#ifdef GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    ಯದಿ(gpu_index >= 0){

        ಯದಿ (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*size*size);
            l.v_gpu = cuda_make_array(l.v, c*n*size*size);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*size*size);
        l.weight_updates_gpu = cuda_make_array(l.weight_updates, c*n*size*size);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.out_h*l.out_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.out_h*l.out_w*n);

        ಯದಿ(batch_normalize){
            l.mean_gpu = cuda_make_array(0, n);
            l.variance_gpu = cuda_make_array(0, n);

            l.rolling_mean_gpu = cuda_make_array(0, n);
            l.rolling_variance_gpu = cuda_make_array(0, n);

            l.mean_delta_gpu = cuda_make_array(0, n);
            l.variance_delta_gpu = cuda_make_array(0, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(0, n);

            l.x_gpu = cuda_make_array(0, l.batch*l.out_h*l.out_w*n);
            l.x_norm_gpu = cuda_make_array(0, l.batch*l.out_h*l.out_w*n);
        }
    }
    #ifdef CUDNN
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnSetTensor4dDescriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l.batch, l.out_c, l.out_h, l.out_w); 
        cudnnSetTensor4dDescriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l.out_c, 1, 1); 
    #endif
#endif

    l.activation = activation;
    l.workspace_size = get_workspace_size(l);

    fprintf(stderr, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, size, size, stride, w, h, c, l.out_w, l.out_h, l.out_c);

    ಪ್ರತ್ಯಾಗಮ l;
}

ಶೂನ್ಯ denormalize_deconvolutional_layer(layer l)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪ್ರತಿ(i = 0; i < l.n; ++i){
        ಭಿನ್ನ scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        ಪ್ರತಿ(j = 0; j < l.c*l.size*l.size; ++j){
            l.weights[i*l.c*l.size*l.size + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

ಶೂನ್ಯ resize_deconvolutional_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w)
{
    l->h = h;
    l->w = w;
    l->out_h = (l->h - 1) * l->stride + l->size - 2*l->pad;
    l->out_w = (l->w - 1) * l->stride + l->size - 2*l->pad;

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
        cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
        cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_DATA_FLOAT, 1, l->out_c, 1, 1); 
    #endif
#endif
    l->workspace_size = get_workspace_size(*l);
}

ಶೂನ್ಯ forward_deconvolutional_layer(ಧ್ರುವ layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i;

    ಪೂರ್ಣಾಙ್ಕ m = l.size*l.size*l.n;
    ಪೂರ್ಣಾಙ್ಕ n = l.h*l.w;
    ಪೂರ್ಣಾಙ್ಕ k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        ಭಿನ್ನ *a = l.weights;
        ಭಿನ್ನ *b = net.ಒಳಹರಿವು + i*l.c*l.h*l.w;
        ಭಿನ್ನ *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.out_c, l.out_h, l.out_w, l.size, l.stride, l.pad, l.output+i*l.outputs);
    }
    ಯದಿ (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } ಅನ್ಯಥಾ {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_w*l.out_h);
    }
    activate_array(l.output, l.batch*l.n*l.out_w*l.out_h, l.activation);
}

ಶೂನ್ಯ backward_deconvolutional_layer(layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ಯದಿ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ಅನ್ಯಥಾ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.out_w*l.out_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    ಪ್ರತಿ(i = 0; i < l.batch; ++i){
        ಪೂರ್ಣಾಙ್ಕ m = l.c;
        ಪೂರ್ಣಾಙ್ಕ n = l.size*l.size*l.n;
        ಪೂರ್ಣಾಙ್ಕ k = l.h*l.w;

        ಭಿನ್ನ *a = net.ಒಳಹರಿವು + i*m*k;
        ಭಿನ್ನ *b = net.workspace;
        ಭಿನ್ನ *c = l.weight_updates;

        im2col_cpu(l.delta + i*l.outputs, l.out_c, l.out_h, l.out_w, 
                l.size, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        ಯದಿ(net.delta){
            ಪೂರ್ಣಾಙ್ಕ m = l.c;
            ಪೂರ್ಣಾಙ್ಕ n = l.h*l.w;
            ಪೂರ್ಣಾಙ್ಕ k = l.size*l.size*l.n;

            ಭಿನ್ನ *a = l.weights;
            ಭಿನ್ನ *b = net.workspace;
            ಭಿನ್ನ *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

ಶೂನ್ಯ update_deconvolutional_layer(layer l, update_args a)
{
    ಭಿನ್ನ learning_rate = a.learning_rate*l.learning_rate_scale;
    ಭಿನ್ನ momentum = a.momentum;
    ಭಿನ್ನ decay = a.decay;
    ಪೂರ್ಣಾಙ್ಕ batch = a.batch;

    ಪೂರ್ಣಾಙ್ಕ size = l.size*l.size*l.c*l.n;
    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ಯದಿ(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}



