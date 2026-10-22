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


ساكن size_t get_workspace_اندازه(layer l){
    أرجع (size_t)l.h*l.w*l.اندازه*l.اندازه*l.n*حجم(عائم);
}

فراغ bilinear_init(layer l)
{
    صحيح i,j,f;
    عائم center = (l.اندازه-1) / 2.;
    لكل(f = 0; f < l.n; ++f){
        لكل(j = 0; j < l.اندازه; ++j){
            لكل(i = 0; i < l.اندازه; ++i){
                عائم val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                صحيح c = f%l.c;
                صحيح ind = f*l.اندازه*l.اندازه*l.c + c*l.اندازه*l.اندازه + j*l.اندازه + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح اندازه, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح adam)
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
    l.اندازه = اندازه;

    l.nweights = c*n*اندازه*اندازه;
    l.nbiases = n;

    l.weights = حافظه_صفر_بده(c*n*اندازه*اندازه, حجم(عائم));
    l.وزن_updates = حافظه_صفر_بده(c*n*اندازه*اندازه, حجم(عائم));

    l.biases = حافظه_صفر_بده(n, حجم(عائم));
    l.bias_updates = حافظه_صفر_بده(n, حجم(عائم));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    عائم scale = .02;
    لكل(i = 0; i < c*n*اندازه*اندازه; ++i) l.weights[i] = scale*rand_نرمال();
    //bilinear_init(l);
    لكل(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.بیرون_h = (l.h - 1) * l.stride + l.اندازه - 2*l.pad;
    l.بیرون_w = (l.w - 1) * l.stride + l.اندازه - 2*l.pad;
    l.بیرون_c = n;
    l.outputs = l.بیرون_w * l.بیرون_h * l.بیرون_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (عائم)l.بیرون_w*l.بیرون_h/(l.w*l.h), l.weights, 1);

    l.output = حافظه_صفر_بده(l.batch*l.outputs, حجم(عائم));
    l.delta  = حافظه_صفر_بده(l.batch*l.outputs, حجم(عائم));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    إذا(batch_normalize){
        l.scales = حافظه_صفر_بده(n, حجم(عائم));
        l.scale_updates = حافظه_صفر_بده(n, حجم(عائم));
        لكل(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.میانگین = حافظه_صفر_بده(n, حجم(عائم));
        l.variance = حافظه_صفر_بده(n, حجم(عائم));

        l.میانگین_delta = حافظه_صفر_بده(n, حجم(عائم));
        l.variance_delta = حافظه_صفر_بده(n, حجم(عائم));

        l.rolling_میانگین = حافظه_صفر_بده(n, حجم(عائم));
        l.rolling_variance = حافظه_صفر_بده(n, حجم(عائم));
        l.x = حافظه_صفر_بده(l.batch*l.outputs, حجم(عائم));
        l.x_norm = حافظه_صفر_بده(l.batch*l.outputs, حجم(عائم));
    }
    إذا(adam){
        l.m = حافظه_صفر_بده(c*n*اندازه*اندازه, حجم(عائم));
        l.v = حافظه_صفر_بده(c*n*اندازه*اندازه, حجم(عائم));
        l.bias_m = حافظه_صفر_بده(n, حجم(عائم));
        l.scale_m = حافظه_صفر_بده(n, حجم(عائم));
        l.bias_v = حافظه_صفر_بده(n, حجم(عائم));
        l.scale_v = حافظه_صفر_بده(n, حجم(عائم));
    }

#اگر_تعریف_شده GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    إذا(gpu_شاخص >= 0){

        إذا (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*اندازه*اندازه);
            l.v_gpu = cuda_make_array(l.v, c*n*اندازه*اندازه);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*اندازه*اندازه);
        l.وزن_updates_gpu = cuda_make_array(l.وزن_updates, c*n*اندازه*اندازه);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.بیرون_h*l.بیرون_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.بیرون_h*l.بیرون_w*n);

        إذا(batch_normalize){
            l.میانگین_gpu = cuda_make_array(0, n);
            l.variance_gpu = cuda_make_array(0, n);

            l.rolling_میانگین_gpu = cuda_make_array(0, n);
            l.rolling_variance_gpu = cuda_make_array(0, n);

            l.میانگین_delta_gpu = cuda_make_array(0, n);
            l.variance_delta_gpu = cuda_make_array(0, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(0, n);

            l.x_gpu = cuda_make_array(0, l.batch*l.بیرون_h*l.بیرون_w*n);
            l.x_norm_gpu = cuda_make_array(0, l.batch*l.بیرون_h*l.بیرون_w*n);
        }
    }
    #اگر_تعریف_شده CUDNN
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnn_مجموعه_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, l.batch, l.بیرون_c, l.بیرون_h, l.بیرون_w); 
        cudnn_مجموعه_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, 1, l.بیرون_c, 1, 1); 
    #پایان_اگر
#پایان_اگر

    l.activation = activation;
    l.workspace_اندازه = get_workspace_اندازه(l);

    fprintf(stderr, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, اندازه, اندازه, stride, w, h, c, l.بیرون_w, l.بیرون_h, l.بیرون_c);

    أرجع l;
}

فراغ denormalize_deconvolutional_layer(layer l)
{
    صحيح i, j;
    لكل(i = 0; i < l.n; ++i){
        عائم scale = l.scales[i]/جذر(l.rolling_variance[i] + .00001);
        لكل(j = 0; j < l.c*l.اندازه*l.اندازه; ++j){
            l.weights[i*l.c*l.اندازه*l.اندازه + j] *= scale;
        }
        l.biases[i] -= l.rolling_میانگین[i] * scale;
        l.scales[i] = 1;
        l.rolling_میانگین[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

فراغ resize_deconvolutional_layer(layer *l, صحيح h, صحيح w)
{
    l->h = h;
    l->w = w;
    l->بیرون_h = (l->h - 1) * l->stride + l->اندازه - 2*l->pad;
    l->بیرون_w = (l->w - 1) * l->stride + l->اندازه - 2*l->pad;

    l->outputs = l->بیرون_h * l->بیرون_w * l->بیرون_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*حجم(عائم));
    إذا(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*حجم(عائم));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*حجم(عائم));
    }

#اگر_تعریف_شده GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    إذا(l->batch_normalize){
        cuda_free(l->x_gpu);
        cuda_free(l->x_norm_gpu);

        l->x_gpu = cuda_make_array(l->output, l->batch*l->outputs);
        l->x_norm_gpu = cuda_make_array(l->output, l->batch*l->outputs);
    }
    #اگر_تعریف_شده CUDNN
        cudnn_مجموعه_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, l->batch, l->بیرون_c, l->بیرون_h, l->بیرون_w); 
        cudnn_مجموعه_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, 1, l->بیرون_c, 1, 1); 
    #پایان_اگر
#پایان_اگر
    l->workspace_اندازه = get_workspace_اندازه(*l);
}

فراغ forward_deconvolutional_layer(ثابت layer l, شبکه net)
{
    صحيح i;

    صحيح m = l.اندازه*l.اندازه*l.n;
    صحيح n = l.h*l.w;
    صحيح k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    لكل(i = 0; i < l.batch; ++i){
        عائم *a = l.weights;
        عائم *b = net.درون_داد + i*l.c*l.h*l.w;
        عائم *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.بیرون_c, l.بیرون_h, l.بیرون_w, l.اندازه, l.stride, l.pad, l.output+i*l.outputs);
    }
    إذا (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } وإلا {
        جمع_کن_bias(l.output, l.biases, l.batch, l.n, l.بیرون_w*l.بیرون_h);
    }
    activate_array(l.output, l.batch*l.n*l.بیرون_w*l.بیرون_h, l.activation);
}

فراغ backward_deconvolutional_layer(layer l, شبکه net)
{
    صحيح i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    إذا(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } وإلا {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.بیرون_w*l.بیرون_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    لكل(i = 0; i < l.batch; ++i){
        صحيح m = l.c;
        صحيح n = l.اندازه*l.اندازه*l.n;
        صحيح k = l.h*l.w;

        عائم *a = net.درون_داد + i*m*k;
        عائم *b = net.workspace;
        عائم *c = l.وزن_updates;

        im2col_cpu(l.delta + i*l.outputs, l.بیرون_c, l.بیرون_h, l.بیرون_w, 
                l.اندازه, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        إذا(net.delta){
            صحيح m = l.c;
            صحيح n = l.h*l.w;
            صحيح k = l.اندازه*l.اندازه*l.n;

            عائم *a = l.weights;
            عائم *b = net.workspace;
            عائم *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

فراغ update_deconvolutional_layer(layer l, update_آرگومانها a)
{
    عائم learning_نرخ = a.learning_نرخ*l.learning_نرخ_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    صحيح اندازه = l.اندازه*l.اندازه*l.c*l.n;
    axpy_cpu(l.n, learning_نرخ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    إذا(l.scales){
        axpy_cpu(l.n, learning_نرخ/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(اندازه, -decay*batch, l.weights, 1, l.وزن_updates, 1);
    axpy_cpu(اندازه, learning_نرخ/batch, l.وزن_updates, 1, l.weights, 1);
    scal_cpu(اندازه, momentum, l.وزن_updates, 1);
}



