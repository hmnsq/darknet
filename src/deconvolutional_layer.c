#ضمن "deconvolutional_layer.h"
#ضمن "convolutional_layer.h"
#ضمن "batchnorm_layer.h"
#ضمن "utils.h"
#ضمن "im2col.h"
#ضمن "col2im.h"
#ضمن "blas.h"
#ضمن "gemm.h"

#ضمن <stdio.h>
#ضمن <time.h>


ساكن size_t get_workspace_size(layer l){
    החזר (size_t)l.h*l.w*l.size*l.size*l.n*حجم(عائم);
}

فراغ bilinear_init(layer l)
{
    صحيح i,j,f;
    عائم center = (l.size-1) / 2.;
    עבור(f = 0; f < l.n; ++f){
        עבור(j = 0; j < l.size; ++j){
            עבור(i = 0; i < l.size; ++i){
                عائم val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                صحيح c = f%l.c;
                صحيح ind = f*l.size*l.size*l.c + c*l.size*l.size + j*l.size + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح size, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح adam)
{
    صحيح i;
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

    l.weights = calloc(c*n*size*size, حجم(عائم));
    l.weight_updates = calloc(c*n*size*size, حجم(عائم));

    l.biases = calloc(n, حجم(عائم));
    l.bias_updates = calloc(n, حجم(عائم));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    عائم scale = .02;
    עבור(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_normal();
    //bilinear_init(l);
    עבור(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.out_h = (l.h - 1) * l.stride + l.size - 2*l.pad;
    l.out_w = (l.w - 1) * l.stride + l.size - 2*l.pad;
    l.out_c = n;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (عائم)l.out_w*l.out_h/(l.w*l.h), l.weights, 1);

    l.output = calloc(l.batch*l.outputs, حجم(عائم));
    l.delta  = calloc(l.batch*l.outputs, حجم(عائم));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    אם(batch_normalize){
        l.scales = calloc(n, حجم(عائم));
        l.scale_updates = calloc(n, حجم(عائم));
        עבור(i = 0; i < n; ++i){
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
    אם(adam){
        l.m = calloc(c*n*size*size, حجم(عائم));
        l.v = calloc(c*n*size*size, حجم(عائم));
        l.bias_m = calloc(n, حجم(عائم));
        l.scale_m = calloc(n, حجم(عائم));
        l.bias_v = calloc(n, حجم(عائم));
        l.scale_v = calloc(n, حجم(عائم));
    }

#ifdef GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    אם(gpu_index >= 0){

        אם (adam) {
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

        אם(batch_normalize){
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

    החזר l;
}

فراغ denormalize_deconvolutional_layer(layer l)
{
    صحيح i, j;
    עבור(i = 0; i < l.n; ++i){
        عائم scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        עבור(j = 0; j < l.c*l.size*l.size; ++j){
            l.weights[i*l.c*l.size*l.size + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

فراغ resize_deconvolutional_layer(layer *l, صحيح h, صحيح w)
{
    l->h = h;
    l->w = w;
    l->out_h = (l->h - 1) * l->stride + l->size - 2*l->pad;
    l->out_w = (l->w - 1) * l->stride + l->size - 2*l->pad;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*حجم(عائم));
    אם(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*حجم(عائم));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*حجم(عائم));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    אם(l->batch_normalize){
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

فراغ forward_deconvolutional_layer(ثابت layer l, network net)
{
    صحيح i;

    صحيح m = l.size*l.size*l.n;
    صحيح n = l.h*l.w;
    صحيح k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    עבור(i = 0; i < l.batch; ++i){
        عائم *a = l.weights;
        عائم *b = net.input + i*l.c*l.h*l.w;
        عائم *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.out_c, l.out_h, l.out_w, l.size, l.stride, l.pad, l.output+i*l.outputs);
    }
    אם (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } אחרת {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_w*l.out_h);
    }
    activate_array(l.output, l.batch*l.n*l.out_w*l.out_h, l.activation);
}

فراغ backward_deconvolutional_layer(layer l, network net)
{
    صحيح i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    אם(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } אחרת {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.out_w*l.out_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    עבור(i = 0; i < l.batch; ++i){
        صحيح m = l.c;
        صحيح n = l.size*l.size*l.n;
        صحيح k = l.h*l.w;

        عائم *a = net.input + i*m*k;
        عائم *b = net.workspace;
        عائم *c = l.weight_updates;

        im2col_cpu(l.delta + i*l.outputs, l.out_c, l.out_h, l.out_w, 
                l.size, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        אם(net.delta){
            صحيح m = l.c;
            صحيح n = l.h*l.w;
            صحيح k = l.size*l.size*l.n;

            عائم *a = l.weights;
            عائم *b = net.workspace;
            عائم *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

فراغ update_deconvolutional_layer(layer l, update_args a)
{
    عائم learning_rate = a.learning_rate*l.learning_rate_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    صحيح size = l.size*l.size*l.c*l.n;
    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    אם(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}



