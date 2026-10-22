#अन्तर्भाव "deconvolutional_layer.h"
#अन्तर्भाव "convolutional_layer.h"
#अन्तर्भाव "batchnorm_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "im2col.h"
#अन्तर्भाव "col2im.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "gemm.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <time.h>


स्थिर size_t get_workspace_size(layer l){
    प्रत्यागम (size_t)l.h*l.w*l.size*l.size*l.n*परिमाण(भिन्न);
}

शून्य bilinear_init(layer l)
{
    पूर्णाङ्क i,j,f;
    भिन्न center = (l.size-1) / 2.;
    प्रति(f = 0; f < l.n; ++f){
        प्रति(j = 0; j < l.size; ++j){
            प्रति(i = 0; i < l.size; ++i){
                भिन्न val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                पूर्णाङ्क c = f%l.c;
                पूर्णाङ्क ind = f*l.size*l.size*l.c + c*l.size*l.size + j*l.size + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क size, पूर्णाङ्क stride, पूर्णाङ्क padding, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam)
{
    पूर्णाङ्क i;
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

    l.weights = calloc(c*n*size*size, परिमाण(भिन्न));
    l.weight_updates = calloc(c*n*size*size, परिमाण(भिन्न));

    l.biases = calloc(n, परिमाण(भिन्न));
    l.bias_updates = calloc(n, परिमाण(भिन्न));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    भिन्न scale = .02;
    प्रति(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_normal();
    //bilinear_init(l);
    प्रति(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.out_h = (l.h - 1) * l.stride + l.size - 2*l.pad;
    l.out_w = (l.w - 1) * l.stride + l.size - 2*l.pad;
    l.out_c = n;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (भिन्न)l.out_w*l.out_h/(l.w*l.h), l.weights, 1);

    l.output = calloc(l.batch*l.outputs, परिमाण(भिन्न));
    l.delta  = calloc(l.batch*l.outputs, परिमाण(भिन्न));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    यदि(batch_normalize){
        l.scales = calloc(n, परिमाण(भिन्न));
        l.scale_updates = calloc(n, परिमाण(भिन्न));
        प्रति(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, परिमाण(भिन्न));
        l.variance = calloc(n, परिमाण(भिन्न));

        l.mean_delta = calloc(n, परिमाण(भिन्न));
        l.variance_delta = calloc(n, परिमाण(भिन्न));

        l.rolling_mean = calloc(n, परिमाण(भिन्न));
        l.rolling_variance = calloc(n, परिमाण(भिन्न));
        l.x = calloc(l.batch*l.outputs, परिमाण(भिन्न));
        l.x_norm = calloc(l.batch*l.outputs, परिमाण(भिन्न));
    }
    यदि(adam){
        l.m = calloc(c*n*size*size, परिमाण(भिन्न));
        l.v = calloc(c*n*size*size, परिमाण(भिन्न));
        l.bias_m = calloc(n, परिमाण(भिन्न));
        l.scale_m = calloc(n, परिमाण(भिन्न));
        l.bias_v = calloc(n, परिमाण(भिन्न));
        l.scale_v = calloc(n, परिमाण(भिन्न));
    }

#ifdef GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    यदि(gpu_index >= 0){

        यदि (adam) {
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

        यदि(batch_normalize){
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

    प्रत्यागम l;
}

शून्य denormalize_deconvolutional_layer(layer l)
{
    पूर्णाङ्क i, j;
    प्रति(i = 0; i < l.n; ++i){
        भिन्न scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        प्रति(j = 0; j < l.c*l.size*l.size; ++j){
            l.weights[i*l.c*l.size*l.size + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

शून्य resize_deconvolutional_layer(layer *l, पूर्णाङ्क h, पूर्णाङ्क w)
{
    l->h = h;
    l->w = w;
    l->out_h = (l->h - 1) * l->stride + l->size - 2*l->pad;
    l->out_w = (l->w - 1) * l->stride + l->size - 2*l->pad;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*परिमाण(भिन्न));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*परिमाण(भिन्न));
    यदि(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*परिमाण(भिन्न));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*परिमाण(भिन्न));
    }

#ifdef GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    यदि(l->batch_normalize){
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

शून्य forward_deconvolutional_layer(ध्रुव layer l, network net)
{
    पूर्णाङ्क i;

    पूर्णाङ्क m = l.size*l.size*l.n;
    पूर्णाङ्क n = l.h*l.w;
    पूर्णाङ्क k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    प्रति(i = 0; i < l.batch; ++i){
        भिन्न *a = l.weights;
        भिन्न *b = net.आदान + i*l.c*l.h*l.w;
        भिन्न *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.out_c, l.out_h, l.out_w, l.size, l.stride, l.pad, l.output+i*l.outputs);
    }
    यदि (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } अन्यथा {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_w*l.out_h);
    }
    activate_array(l.output, l.batch*l.n*l.out_w*l.out_h, l.activation);
}

शून्य backward_deconvolutional_layer(layer l, network net)
{
    पूर्णाङ्क i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    यदि(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } अन्यथा {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.out_w*l.out_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    प्रति(i = 0; i < l.batch; ++i){
        पूर्णाङ्क m = l.c;
        पूर्णाङ्क n = l.size*l.size*l.n;
        पूर्णाङ्क k = l.h*l.w;

        भिन्न *a = net.आदान + i*m*k;
        भिन्न *b = net.workspace;
        भिन्न *c = l.weight_updates;

        im2col_cpu(l.delta + i*l.outputs, l.out_c, l.out_h, l.out_w, 
                l.size, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        यदि(net.delta){
            पूर्णाङ्क m = l.c;
            पूर्णाङ्क n = l.h*l.w;
            पूर्णाङ्क k = l.size*l.size*l.n;

            भिन्न *a = l.weights;
            भिन्न *b = net.workspace;
            भिन्न *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

शून्य update_deconvolutional_layer(layer l, update_args a)
{
    भिन्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भिन्न momentum = a.momentum;
    भिन्न decay = a.decay;
    पूर्णाङ्क batch = a.batch;

    पूर्णाङ्क size = l.size*l.size*l.c*l.n;
    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    यदि(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(size, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(size, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(size, momentum, l.weight_updates, 1);
}



