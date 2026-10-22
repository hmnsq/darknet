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


ساكن size_t get_workspace_ناپ(layer l){
    أرجع (size_t)l.h*l.w*l.ناپ*l.ناپ*l.n*حجم(عائم);
}

فراغ bilinear_init(layer l)
{
    صحيح i,j,f;
    عائم center = (l.ناپ-1) / 2.;
    لكل(f = 0; f < l.n; ++f){
        لكل(j = 0; j < l.ناپ; ++j){
            لكل(i = 0; i < l.ناپ; ++i){
                عائم val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                صحيح c = f%l.c;
                صحيح ind = f*l.ناپ*l.ناپ*l.c + c*l.ناپ*l.ناپ + j*l.ناپ + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح ناپ, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح adam)
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
    l.ناپ = ناپ;

    l.nweights = c*n*ناپ*ناپ;
    l.nbiases = n;

    l.weights = صفر_حافظہ_دیو(c*n*ناپ*ناپ, حجم(عائم));
    l.بھار_updates = صفر_حافظہ_دیو(c*n*ناپ*ناپ, حجم(عائم));

    l.biases = صفر_حافظہ_دیو(n, حجم(عائم));
    l.bias_updates = صفر_حافظہ_دیو(n, حجم(عائم));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    عائم scale = .02;
    لكل(i = 0; i < c*n*ناپ*ناپ; ++i) l.weights[i] = scale*rand_عام();
    //bilinear_init(l);
    لكل(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.باہرلا_h = (l.h - 1) * l.stride + l.ناپ - 2*l.pad;
    l.باہرلا_w = (l.w - 1) * l.stride + l.ناپ - 2*l.pad;
    l.باہرلا_c = n;
    l.outputs = l.باہرلا_w * l.باہرلا_h * l.باہرلا_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (عائم)l.باہرلا_w*l.باہرلا_h/(l.w*l.h), l.weights, 1);

    l.output = صفر_حافظہ_دیو(l.batch*l.outputs, حجم(عائم));
    l.delta  = صفر_حافظہ_دیو(l.batch*l.outputs, حجم(عائم));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    إذا(batch_normalize){
        l.scales = صفر_حافظہ_دیو(n, حجم(عائم));
        l.scale_updates = صفر_حافظہ_دیو(n, حجم(عائم));
        لكل(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.اوسط = صفر_حافظہ_دیو(n, حجم(عائم));
        l.variance = صفر_حافظہ_دیو(n, حجم(عائم));

        l.اوسط_delta = صفر_حافظہ_دیو(n, حجم(عائم));
        l.variance_delta = صفر_حافظہ_دیو(n, حجم(عائم));

        l.rolling_اوسط = صفر_حافظہ_دیو(n, حجم(عائم));
        l.rolling_variance = صفر_حافظہ_دیو(n, حجم(عائم));
        l.x = صفر_حافظہ_دیو(l.batch*l.outputs, حجم(عائم));
        l.x_norm = صفر_حافظہ_دیو(l.batch*l.outputs, حجم(عائم));
    }
    إذا(adam){
        l.m = صفر_حافظہ_دیو(c*n*ناپ*ناپ, حجم(عائم));
        l.v = صفر_حافظہ_دیو(c*n*ناپ*ناپ, حجم(عائم));
        l.bias_m = صفر_حافظہ_دیو(n, حجم(عائم));
        l.scale_m = صفر_حافظہ_دیو(n, حجم(عائم));
        l.bias_v = صفر_حافظہ_دیو(n, حجم(عائم));
        l.scale_v = صفر_حافظہ_دیو(n, حجم(عائم));
    }

#جے_تعریف_شدہ GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    إذا(gpu_اشاریہ >= 0){

        إذا (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*ناپ*ناپ);
            l.v_gpu = cuda_make_array(l.v, c*n*ناپ*ناپ);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*ناپ*ناپ);
        l.بھار_updates_gpu = cuda_make_array(l.بھار_updates, c*n*ناپ*ناپ);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.باہرلا_h*l.باہرلا_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.باہرلا_h*l.باہرلا_w*n);

        إذا(batch_normalize){
            l.اوسط_gpu = cuda_make_array(0, n);
            l.variance_gpu = cuda_make_array(0, n);

            l.rolling_اوسط_gpu = cuda_make_array(0, n);
            l.rolling_variance_gpu = cuda_make_array(0, n);

            l.اوسط_delta_gpu = cuda_make_array(0, n);
            l.variance_delta_gpu = cuda_make_array(0, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(0, n);

            l.x_gpu = cuda_make_array(0, l.batch*l.باہرلا_h*l.باہرلا_w*n);
            l.x_norm_gpu = cuda_make_array(0, l.batch*l.باہرلا_h*l.باہرلا_w*n);
        }
    }
    #جے_تعریف_شدہ CUDNN
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnn_سیٹ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, l.batch, l.باہرلا_c, l.باہرلا_h, l.باہرلا_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, 1, l.باہرلا_c, 1, 1); 
    #جے_انت
#جے_انت

    l.activation = activation;
    l.workspace_ناپ = get_workspace_ناپ(l);

    fprintf(stderr, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, ناپ, ناپ, stride, w, h, c, l.باہرلا_w, l.باہرلا_h, l.باہرلا_c);

    أرجع l;
}

فراغ denormalize_deconvolutional_layer(layer l)
{
    صحيح i, j;
    لكل(i = 0; i < l.n; ++i){
        عائم scale = l.scales[i]/جذر(l.rolling_variance[i] + .00001);
        لكل(j = 0; j < l.c*l.ناپ*l.ناپ; ++j){
            l.weights[i*l.c*l.ناپ*l.ناپ + j] *= scale;
        }
        l.biases[i] -= l.rolling_اوسط[i] * scale;
        l.scales[i] = 1;
        l.rolling_اوسط[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

فراغ resize_deconvolutional_layer(layer *l, صحيح h, صحيح w)
{
    l->h = h;
    l->w = w;
    l->باہرلا_h = (l->h - 1) * l->stride + l->ناپ - 2*l->pad;
    l->باہرلا_w = (l->w - 1) * l->stride + l->ناپ - 2*l->pad;

    l->outputs = l->باہرلا_h * l->باہرلا_w * l->باہرلا_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*حجم(عائم));
    إذا(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*حجم(عائم));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*حجم(عائم));
    }

#جے_تعریف_شدہ GPU
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
    #جے_تعریف_شدہ CUDNN
        cudnn_سیٹ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, l->batch, l->باہرلا_c, l->باہرلا_h, l->باہرلا_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_انکڑے_FLOAT, 1, l->باہرلا_c, 1, 1); 
    #جے_انت
#جے_انت
    l->workspace_ناپ = get_workspace_ناپ(*l);
}

فراغ forward_deconvolutional_layer(ثابت layer l, جال net)
{
    صحيح i;

    صحيح m = l.ناپ*l.ناپ*l.n;
    صحيح n = l.h*l.w;
    صحيح k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    لكل(i = 0; i < l.batch; ++i){
        عائم *a = l.weights;
        عائم *b = net.داخلہ + i*l.c*l.h*l.w;
        عائم *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.باہرلا_c, l.باہرلا_h, l.باہرلا_w, l.ناپ, l.stride, l.pad, l.output+i*l.outputs);
    }
    إذا (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } وإلا {
        جوڑو_bias(l.output, l.biases, l.batch, l.n, l.باہرلا_w*l.باہرلا_h);
    }
    activate_array(l.output, l.batch*l.n*l.باہرلا_w*l.باہرلا_h, l.activation);
}

فراغ backward_deconvolutional_layer(layer l, جال net)
{
    صحيح i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    إذا(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } وإلا {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.باہرلا_w*l.باہرلا_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    لكل(i = 0; i < l.batch; ++i){
        صحيح m = l.c;
        صحيح n = l.ناپ*l.ناپ*l.n;
        صحيح k = l.h*l.w;

        عائم *a = net.داخلہ + i*m*k;
        عائم *b = net.workspace;
        عائم *c = l.بھار_updates;

        im2col_cpu(l.delta + i*l.outputs, l.باہرلا_c, l.باہرلا_h, l.باہرلا_w, 
                l.ناپ, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        إذا(net.delta){
            صحيح m = l.c;
            صحيح n = l.h*l.w;
            صحيح k = l.ناپ*l.ناپ*l.n;

            عائم *a = l.weights;
            عائم *b = net.workspace;
            عائم *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

فراغ update_deconvolutional_layer(layer l, update_دلیلاں a)
{
    عائم learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    صحيح ناپ = l.ناپ*l.ناپ*l.c*l.n;
    axpy_cpu(l.n, learning_شرح/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    إذا(l.scales){
        axpy_cpu(l.n, learning_شرح/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(ناپ, -decay*batch, l.weights, 1, l.بھار_updates, 1);
    axpy_cpu(ناپ, learning_شرح/batch, l.بھار_updates, 1, l.weights, 1);
    scal_cpu(ناپ, momentum, l.بھار_updates, 1);
}



