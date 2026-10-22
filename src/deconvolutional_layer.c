#অন্তর্ভাব "deconvolutional_layer.h"
#অন্তর্ভাব "convolutional_layer.h"
#অন্তর্ভাব "batchnorm_layer.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "im2col.h"
#অন্তর্ভাব "col2im.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "gemm.h"

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <time.h>


স্থির size_t get_workspace_আকার(layer l){
    ফেরত (size_t)l.h*l.w*l.আকার_2*l.আকার_2*l.n*আকার(দশমিক);
}

শূন্য bilinear_init(layer l)
{
    পূর্ণসংখ্যা i,j,f;
    দশমিক center = (l.আকার_2-1) / 2.;
    জন্য(f = 0; f < l.n; ++f){
        জন্য(j = 0; j < l.আকার_2; ++j){
            জন্য(i = 0; i < l.আকার_2; ++i){
                দশমিক val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                পূর্ণসংখ্যা c = f%l.c;
                পূর্ণসংখ্যা ind = f*l.আকার_2*l.আকার_2*l.c + c*l.আকার_2*l.আকার_2 + j*l.আকার_2 + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা padding, ACTIVATION activation, পূর্ণসংখ্যা batch_normalize, পূর্ণসংখ্যা adam)
{
    পূর্ণসংখ্যা i;
    layer l = {0};
    l.type = DECONVOLUTIONAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.আকার_2 = আকার_2;

    l.nweights = c*n*আকার_2*আকার_2;
    l.nbiases = n;

    l.weights = calloc(c*n*আকার_2*আকার_2, আকার(দশমিক));
    l.weight_updates = calloc(c*n*আকার_2*আকার_2, আকার(দশমিক));

    l.biases = calloc(n, আকার(দশমিক));
    l.bias_updates = calloc(n, আকার(দশমিক));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    দশমিক scale = .02;
    জন্য(i = 0; i < c*n*আকার_2*আকার_2; ++i) l.weights[i] = scale*rand_normal();
    //bilinear_init(l);
    জন্য(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.out_h = (l.h - 1) * l.stride + l.আকার_2 - 2*l.pad;
    l.out_w = (l.w - 1) * l.stride + l.আকার_2 - 2*l.pad;
    l.out_c = n;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (দশমিক)l.out_w*l.out_h/(l.w*l.h), l.weights, 1);

    l.output = calloc(l.batch*l.outputs, আকার(দশমিক));
    l.delta  = calloc(l.batch*l.outputs, আকার(দশমিক));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    যদি(batch_normalize){
        l.scales = calloc(n, আকার(দশমিক));
        l.scale_updates = calloc(n, আকার(দশমিক));
        জন্য(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = calloc(n, আকার(দশমিক));
        l.variance = calloc(n, আকার(দশমিক));

        l.mean_delta = calloc(n, আকার(দশমিক));
        l.variance_delta = calloc(n, আকার(দশমিক));

        l.rolling_mean = calloc(n, আকার(দশমিক));
        l.rolling_variance = calloc(n, আকার(দশমিক));
        l.x = calloc(l.batch*l.outputs, আকার(দশমিক));
        l.x_norm = calloc(l.batch*l.outputs, আকার(দশমিক));
    }
    যদি(adam){
        l.m = calloc(c*n*আকার_2*আকার_2, আকার(দশমিক));
        l.v = calloc(c*n*আকার_2*আকার_2, আকার(দশমিক));
        l.bias_m = calloc(n, আকার(দশমিক));
        l.scale_m = calloc(n, আকার(দশমিক));
        l.bias_v = calloc(n, আকার(দশমিক));
        l.scale_v = calloc(n, আকার(দশমিক));
    }

#ifdef GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    যদি(gpu_সূচক >= 0){

        যদি (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*আকার_2*আকার_2);
            l.v_gpu = cuda_make_array(l.v, c*n*আকার_2*আকার_2);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*আকার_2*আকার_2);
        l.weight_updates_gpu = cuda_make_array(l.weight_updates, c*n*আকার_2*আকার_2);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.out_h*l.out_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.out_h*l.out_w*n);

        যদি(batch_normalize){
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
        cudnnSetTensor4dDescriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_তথ্য_FLOAT, l.batch, l.out_c, l.out_h, l.out_w); 
        cudnnSetTensor4dDescriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_তথ্য_FLOAT, 1, l.out_c, 1, 1); 
    #endif
#endif

    l.activation = activation;
    l.workspace_আকার = get_workspace_আকার(l);

    fprintf(stderr, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, আকার_2, আকার_2, stride, w, h, c, l.out_w, l.out_h, l.out_c);

    ফেরত l;
}

শূন্য denormalize_deconvolutional_layer(layer l)
{
    পূর্ণসংখ্যা i, j;
    জন্য(i = 0; i < l.n; ++i){
        দশমিক scale = l.scales[i]/sqrt(l.rolling_variance[i] + .00001);
        জন্য(j = 0; j < l.c*l.আকার_2*l.আকার_2; ++j){
            l.weights[i*l.c*l.আকার_2*l.আকার_2 + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

শূন্য resize_deconvolutional_layer(layer *l, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w)
{
    l->h = h;
    l->w = w;
    l->out_h = (l->h - 1) * l->stride + l->আকার_2 - 2*l->pad;
    l->out_w = (l->w - 1) * l->stride + l->আকার_2 - 2*l->pad;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*আকার(দশমিক));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*আকার(দশমিক));
    যদি(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*আকার(দশমিক));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*আকার(দশমিক));
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
        cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_তথ্য_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
        cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_তথ্য_FLOAT, 1, l->out_c, 1, 1); 
    #endif
#endif
    l->workspace_আকার = get_workspace_আকার(*l);
}

শূন্য forward_deconvolutional_layer(ধ্রুবক layer l, network net)
{
    পূর্ণসংখ্যা i;

    পূর্ণসংখ্যা m = l.আকার_2*l.আকার_2*l.n;
    পূর্ণসংখ্যা n = l.h*l.w;
    পূর্ণসংখ্যা k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    জন্য(i = 0; i < l.batch; ++i){
        দশমিক *a = l.weights;
        দশমিক *b = net.প্রবেশ + i*l.c*l.h*l.w;
        দশমিক *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.out_c, l.out_h, l.out_w, l.আকার_2, l.stride, l.pad, l.output+i*l.outputs);
    }
    যদি (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } অন্যথা {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_w*l.out_h);
    }
    activate_array(l.output, l.batch*l.n*l.out_w*l.out_h, l.activation);
}

শূন্য backward_deconvolutional_layer(layer l, network net)
{
    পূর্ণসংখ্যা i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    যদি(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } অন্যথা {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.out_w*l.out_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    জন্য(i = 0; i < l.batch; ++i){
        পূর্ণসংখ্যা m = l.c;
        পূর্ণসংখ্যা n = l.আকার_2*l.আকার_2*l.n;
        পূর্ণসংখ্যা k = l.h*l.w;

        দশমিক *a = net.প্রবেশ + i*m*k;
        দশমিক *b = net.workspace;
        দশমিক *c = l.weight_updates;

        im2col_cpu(l.delta + i*l.outputs, l.out_c, l.out_h, l.out_w, 
                l.আকার_2, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        যদি(net.delta){
            পূর্ণসংখ্যা m = l.c;
            পূর্ণসংখ্যা n = l.h*l.w;
            পূর্ণসংখ্যা k = l.আকার_2*l.আকার_2*l.n;

            দশমিক *a = l.weights;
            দশমিক *b = net.workspace;
            দশমিক *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

শূন্য update_deconvolutional_layer(layer l, update_args a)
{
    দশমিক learning_rate = a.learning_rate*l.learning_rate_scale;
    দশমিক momentum = a.momentum;
    দশমিক decay = a.decay;
    পূর্ণসংখ্যা batch = a.batch;

    পূর্ণসংখ্যা আকার_2 = l.আকার_2*l.আকার_2*l.c*l.n;
    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    যদি(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(আকার_2, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(আকার_2, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(আকার_2, momentum, l.weight_updates, 1);
}



