#ਅਨ੍ਤਰ੍ਭਾਵ "deconvolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "convolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "batchnorm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "im2col.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "col2im.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>


ਸਥਿਰ size_t get_workspace_ਮਾਪ(layer l){
    ਵਾਪਸ (size_t)l.h*l.w*l.ਮਾਪ*l.ਮਾਪ*l.n*ਆਕਾਰ(ਦਸ਼ਮਲਵ);
}

ਖਾਲੀ bilinear_init(layer l)
{
    ਪੂਰਨ i,j,f;
    ਦਸ਼ਮਲਵ center = (l.ਮਾਪ-1) / 2.;
    ਲਈ(f = 0; f < l.n; ++f){
        ਲਈ(j = 0; j < l.ਮਾਪ; ++j){
            ਲਈ(i = 0; i < l.ਮਾਪ; ++i){
                ਦਸ਼ਮਲਵ val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                ਪੂਰਨ c = f%l.c;
                ਪੂਰਨ ind = f*l.ਮਾਪ*l.ਮਾਪ*l.c + c*l.ਮਾਪ*l.ਮਾਪ + j*l.ਮਾਪ + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ stride, ਪੂਰਨ padding, ACTIVATION activation, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam)
{
    ਪੂਰਨ i;
    layer l = {0};
    l.type = DECONVOLUTIONAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.ਮਾਪ = ਮਾਪ;

    l.nweights = c*n*ਮਾਪ*ਮਾਪ;
    l.nbiases = n;

    l.weights = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c*n*ਮਾਪ*ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.ਭਾਰ_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c*n*ਮਾਪ*ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.biases = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.bias_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    ਦਸ਼ਮਲਵ scale = .02;
    ਲਈ(i = 0; i < c*n*ਮਾਪ*ਮਾਪ; ++i) l.weights[i] = scale*rand_ਸਾਧਾਰਨ();
    //bilinear_init(l);
    ਲਈ(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.ਬਾਹਰਲਾ_h = (l.h - 1) * l.stride + l.ਮਾਪ - 2*l.pad;
    l.ਬਾਹਰਲਾ_w = (l.w - 1) * l.stride + l.ਮਾਪ - 2*l.pad;
    l.ਬਾਹਰਲਾ_c = n;
    l.outputs = l.ਬਾਹਰਲਾ_w * l.ਬਾਹਰਲਾ_h * l.ਬਾਹਰਲਾ_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (ਦਸ਼ਮਲਵ)l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h/(l.w*l.h), l.weights, 1);

    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta  = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    ਜੇ(batch_normalize){
        l.scales = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.scale_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਲਈ(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.ਔਸਤ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.variance = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

        l.ਔਸਤ_delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.variance_delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

        l.rolling_ਔਸਤ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.rolling_variance = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.x = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.x_norm = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }
    ਜੇ(adam){
        l.m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c*n*ਮਾਪ*ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c*n*ਮਾਪ*ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.bias_m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.scale_m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.bias_v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.scale_v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){

        ਜੇ (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*ਮਾਪ*ਮਾਪ);
            l.v_gpu = cuda_make_array(l.v, c*n*ਮਾਪ*ਮਾਪ);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*ਮਾਪ*ਮਾਪ);
        l.ਭਾਰ_updates_gpu = cuda_make_array(l.ਭਾਰ_updates, c*n*ਮਾਪ*ਮਾਪ);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w*n);

        ਜੇ(batch_normalize){
            l.ਔਸਤ_gpu = cuda_make_array(0, n);
            l.variance_gpu = cuda_make_array(0, n);

            l.rolling_ਔਸਤ_gpu = cuda_make_array(0, n);
            l.rolling_variance_gpu = cuda_make_array(0, n);

            l.ਔਸਤ_delta_gpu = cuda_make_array(0, n);
            l.variance_delta_gpu = cuda_make_array(0, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(0, n);

            l.x_gpu = cuda_make_array(0, l.batch*l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w*n);
            l.x_norm_gpu = cuda_make_array(0, l.batch*l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w*n);
        }
    }
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, l.batch, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, 1, l.ਬਾਹਰਲਾ_c, 1, 1); 
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

    l.activation = activation;
    l.workspace_ਮਾਪ = get_workspace_ਮਾਪ(l);

    fprintf(stderr, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, ਮਾਪ, ਮਾਪ, stride, w, h, c, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c);

    ਵਾਪਸ l;
}

ਖਾਲੀ denormalize_deconvolutional_layer(layer l)
{
    ਪੂਰਨ i, j;
    ਲਈ(i = 0; i < l.n; ++i){
        ਦਸ਼ਮਲਵ scale = l.scales[i]/ਵਰਗਮੂਲ(l.rolling_variance[i] + .00001);
        ਲਈ(j = 0; j < l.c*l.ਮਾਪ*l.ਮਾਪ; ++j){
            l.weights[i*l.c*l.ਮਾਪ*l.ਮਾਪ + j] *= scale;
        }
        l.biases[i] -= l.rolling_ਔਸਤ[i] * scale;
        l.scales[i] = 1;
        l.rolling_ਔਸਤ[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

ਖਾਲੀ resize_deconvolutional_layer(layer *l, ਪੂਰਨ h, ਪੂਰਨ w)
{
    l->h = h;
    l->w = w;
    l->ਬਾਹਰਲਾ_h = (l->h - 1) * l->stride + l->ਮਾਪ - 2*l->pad;
    l->ਬਾਹਰਲਾ_w = (l->w - 1) * l->stride + l->ਮਾਪ - 2*l->pad;

    l->outputs = l->ਬਾਹਰਲਾ_h * l->ਬਾਹਰਲਾ_w * l->ਬਾਹਰਲਾ_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਜੇ(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    ਜੇ(l->batch_normalize){
        cuda_free(l->x_gpu);
        cuda_free(l->x_norm_gpu);

        l->x_gpu = cuda_make_array(l->output, l->batch*l->outputs);
        l->x_norm_gpu = cuda_make_array(l->output, l->batch*l->outputs);
    }
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, l->batch, l->ਬਾਹਰਲਾ_c, l->ਬਾਹਰਲਾ_h, l->ਬਾਹਰਲਾ_w); 
        cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, 1, l->ਬਾਹਰਲਾ_c, 1, 1); 
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
    l->workspace_ਮਾਪ = get_workspace_ਮਾਪ(*l);
}

ਖਾਲੀ forward_deconvolutional_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    ਪੂਰਨ i;

    ਪੂਰਨ m = l.ਮਾਪ*l.ਮਾਪ*l.n;
    ਪੂਰਨ n = l.h*l.w;
    ਪੂਰਨ k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ਲਈ(i = 0; i < l.batch; ++i){
        ਦਸ਼ਮਲਵ *a = l.weights;
        ਦਸ਼ਮਲਵ *b = net.ਨਿਵੇਸ਼ + i*l.c*l.h*l.w;
        ਦਸ਼ਮਲਵ *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_w, l.ਮਾਪ, l.stride, l.pad, l.output+i*l.outputs);
    }
    ਜੇ (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } ਨਹੀਂਤਾਂ {
        ਜੋੜੋ_bias(l.output, l.biases, l.batch, l.n, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h);
    }
    activate_array(l.output, l.batch*l.n*l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h, l.activation);
}

ਖਾਲੀ backward_deconvolutional_layer(layer l, ਜਾਲ net)
{
    ਪੂਰਨ i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ਜੇ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ਨਹੀਂਤਾਂ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    ਲਈ(i = 0; i < l.batch; ++i){
        ਪੂਰਨ m = l.c;
        ਪੂਰਨ n = l.ਮਾਪ*l.ਮਾਪ*l.n;
        ਪੂਰਨ k = l.h*l.w;

        ਦਸ਼ਮਲਵ *a = net.ਨਿਵੇਸ਼ + i*m*k;
        ਦਸ਼ਮਲਵ *b = net.workspace;
        ਦਸ਼ਮਲਵ *c = l.ਭਾਰ_updates;

        im2col_cpu(l.delta + i*l.outputs, l.ਬਾਹਰਲਾ_c, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_w, 
                l.ਮਾਪ, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        ਜੇ(net.delta){
            ਪੂਰਨ m = l.c;
            ਪੂਰਨ n = l.h*l.w;
            ਪੂਰਨ k = l.ਮਾਪ*l.ਮਾਪ*l.n;

            ਦਸ਼ਮਲਵ *a = l.weights;
            ਦਸ਼ਮਲਵ *b = net.workspace;
            ਦਸ਼ਮਲਵ *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

ਖਾਲੀ update_deconvolutional_layer(layer l, update_ਦਲੀਲਾਂ a)
{
    ਦਸ਼ਮਲਵ learning_ਦਰ = a.learning_ਦਰ*l.learning_ਦਰ_scale;
    ਦਸ਼ਮਲਵ momentum = a.momentum;
    ਦਸ਼ਮਲਵ decay = a.decay;
    ਪੂਰਨ batch = a.batch;

    ਪੂਰਨ ਮਾਪ = l.ਮਾਪ*l.ਮਾਪ*l.c*l.n;
    axpy_cpu(l.n, learning_ਦਰ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ਜੇ(l.scales){
        axpy_cpu(l.n, learning_ਦਰ/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(ਮਾਪ, -decay*batch, l.weights, 1, l.ਭਾਰ_updates, 1);
    axpy_cpu(ਮਾਪ, learning_ਦਰ/batch, l.ਭਾਰ_updates, 1, l.weights, 1);
    scal_cpu(ਮਾਪ, momentum, l.ਭਾਰ_updates, 1);
}



