#شامل "deconvolutional_layer.h"
#شامل "convolutional_layer.h"
#شامل "batchnorm_layer.h"
#شامل "utils.h"
#شامل "im2col.h"
#شامل "col2im.h"
#شامل "blas.h"
#شامل "gemm.h"

#شامل <stdio.h>
#شامل <time.h>


ساکن size_t get_workspace_سائز(layer l){
    واپس (size_t)l.h*l.w*l.سائز*l.سائز*l.n*حجم(اعشاری);
}

خالی bilinear_init(layer l)
{
    صحیح i,j,f;
    اعشاری center = (l.سائز-1) / 2.;
    برائے(f = 0; f < l.n; ++f){
        برائے(j = 0; j < l.سائز; ++j){
            برائے(i = 0; i < l.سائز; ++i){
                اعشاری val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                صحیح c = f%l.c;
                صحیح ind = f*l.سائز*l.سائز*l.c + c*l.سائز*l.سائز + j*l.سائز + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح سائز, صحیح stride, صحیح padding, ACTIVATION activation, صحیح batch_normalize, صحیح adam)
{
    صحیح i;
    layer l = {0};
    l.type = DECONVOLUTIONAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.سائز = سائز;

    l.nweights = c*n*سائز*سائز;
    l.nbiases = n;

    l.weights = صفر_حافظہ_دو(c*n*سائز*سائز, حجم(اعشاری));
    l.وزن_updates = صفر_حافظہ_دو(c*n*سائز*سائز, حجم(اعشاری));

    l.biases = صفر_حافظہ_دو(n, حجم(اعشاری));
    l.bias_updates = صفر_حافظہ_دو(n, حجم(اعشاری));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    اعشاری scale = .02;
    برائے(i = 0; i < c*n*سائز*سائز; ++i) l.weights[i] = scale*rand_معمول();
    //bilinear_init(l);
    برائے(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.برآمد_h = (l.h - 1) * l.stride + l.سائز - 2*l.pad;
    l.برآمد_w = (l.w - 1) * l.stride + l.سائز - 2*l.pad;
    l.برآمد_c = n;
    l.outputs = l.برآمد_w * l.برآمد_h * l.برآمد_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (اعشاری)l.برآمد_w*l.برآمد_h/(l.w*l.h), l.weights, 1);

    l.output = صفر_حافظہ_دو(l.batch*l.outputs, حجم(اعشاری));
    l.delta  = صفر_حافظہ_دو(l.batch*l.outputs, حجم(اعشاری));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    اگر(batch_normalize){
        l.scales = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.scale_updates = صفر_حافظہ_دو(n, حجم(اعشاری));
        برائے(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.اوسط = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.variance = صفر_حافظہ_دو(n, حجم(اعشاری));

        l.اوسط_delta = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.variance_delta = صفر_حافظہ_دو(n, حجم(اعشاری));

        l.rolling_اوسط = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.rolling_variance = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.x = صفر_حافظہ_دو(l.batch*l.outputs, حجم(اعشاری));
        l.x_norm = صفر_حافظہ_دو(l.batch*l.outputs, حجم(اعشاری));
    }
    اگر(adam){
        l.m = صفر_حافظہ_دو(c*n*سائز*سائز, حجم(اعشاری));
        l.v = صفر_حافظہ_دو(c*n*سائز*سائز, حجم(اعشاری));
        l.bias_m = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.scale_m = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.bias_v = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.scale_v = صفر_حافظہ_دو(n, حجم(اعشاری));
    }

#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    اگر(gpu_اشاریہ >= 0){

        اگر (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*سائز*سائز);
            l.v_gpu = cuda_make_array(l.v, c*n*سائز*سائز);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*سائز*سائز);
        l.وزن_updates_gpu = cuda_make_array(l.وزن_updates, c*n*سائز*سائز);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.برآمد_h*l.برآمد_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.برآمد_h*l.برآمد_w*n);

        اگر(batch_normalize){
            l.اوسط_gpu = cuda_make_array(0, n);
            l.variance_gpu = cuda_make_array(0, n);

            l.rolling_اوسط_gpu = cuda_make_array(0, n);
            l.rolling_variance_gpu = cuda_make_array(0, n);

            l.اوسط_delta_gpu = cuda_make_array(0, n);
            l.variance_delta_gpu = cuda_make_array(0, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(0, n);

            l.x_gpu = cuda_make_array(0, l.batch*l.برآمد_h*l.برآمد_w*n);
            l.x_norm_gpu = cuda_make_array(0, l.batch*l.برآمد_h*l.برآمد_w*n);
        }
    }
    #اگر_تعریف_شدہ CUDNN
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnn_سیٹ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, l.batch, l.برآمد_c, l.برآمد_h, l.برآمد_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, 1, l.برآمد_c, 1, 1); 
    #اگر_ختم
#اگر_ختم

    l.activation = activation;
    l.workspace_سائز = get_workspace_سائز(l);

    fprintf(stderr, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, سائز, سائز, stride, w, h, c, l.برآمد_w, l.برآمد_h, l.برآمد_c);

    واپس l;
}

خالی denormalize_deconvolutional_layer(layer l)
{
    صحیح i, j;
    برائے(i = 0; i < l.n; ++i){
        اعشاری scale = l.scales[i]/جذر(l.rolling_variance[i] + .00001);
        برائے(j = 0; j < l.c*l.سائز*l.سائز; ++j){
            l.weights[i*l.c*l.سائز*l.سائز + j] *= scale;
        }
        l.biases[i] -= l.rolling_اوسط[i] * scale;
        l.scales[i] = 1;
        l.rolling_اوسط[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

خالی resize_deconvolutional_layer(layer *l, صحیح h, صحیح w)
{
    l->h = h;
    l->w = w;
    l->برآمد_h = (l->h - 1) * l->stride + l->سائز - 2*l->pad;
    l->برآمد_w = (l->w - 1) * l->stride + l->سائز - 2*l->pad;

    l->outputs = l->برآمد_h * l->برآمد_w * l->برآمد_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(اعشاری));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*حجم(اعشاری));
    اگر(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*حجم(اعشاری));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*حجم(اعشاری));
    }

#اگر_تعریف_شدہ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    اگر(l->batch_normalize){
        cuda_free(l->x_gpu);
        cuda_free(l->x_norm_gpu);

        l->x_gpu = cuda_make_array(l->output, l->batch*l->outputs);
        l->x_norm_gpu = cuda_make_array(l->output, l->batch*l->outputs);
    }
    #اگر_تعریف_شدہ CUDNN
        cudnn_سیٹ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, l->batch, l->برآمد_c, l->برآمد_h, l->برآمد_w); 
        cudnn_سیٹ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, 1, l->برآمد_c, 1, 1); 
    #اگر_ختم
#اگر_ختم
    l->workspace_سائز = get_workspace_سائز(*l);
}

خالی forward_deconvolutional_layer(ثابت layer l, جال net)
{
    صحیح i;

    صحیح m = l.سائز*l.سائز*l.n;
    صحیح n = l.h*l.w;
    صحیح k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    برائے(i = 0; i < l.batch; ++i){
        اعشاری *a = l.weights;
        اعشاری *b = net.داخلہ + i*l.c*l.h*l.w;
        اعشاری *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.برآمد_c, l.برآمد_h, l.برآمد_w, l.سائز, l.stride, l.pad, l.output+i*l.outputs);
    }
    اگر (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } ورنہ {
        جوڑو_bias(l.output, l.biases, l.batch, l.n, l.برآمد_w*l.برآمد_h);
    }
    activate_array(l.output, l.batch*l.n*l.برآمد_w*l.برآمد_h, l.activation);
}

خالی backward_deconvolutional_layer(layer l, جال net)
{
    صحیح i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    اگر(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ورنہ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.برآمد_w*l.برآمد_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    برائے(i = 0; i < l.batch; ++i){
        صحیح m = l.c;
        صحیح n = l.سائز*l.سائز*l.n;
        صحیح k = l.h*l.w;

        اعشاری *a = net.داخلہ + i*m*k;
        اعشاری *b = net.workspace;
        اعشاری *c = l.وزن_updates;

        im2col_cpu(l.delta + i*l.outputs, l.برآمد_c, l.برآمد_h, l.برآمد_w, 
                l.سائز, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        اگر(net.delta){
            صحیح m = l.c;
            صحیح n = l.h*l.w;
            صحیح k = l.سائز*l.سائز*l.n;

            اعشاری *a = l.weights;
            اعشاری *b = net.workspace;
            اعشاری *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

خالی update_deconvolutional_layer(layer l, update_دلائل a)
{
    اعشاری learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;

    صحیح سائز = l.سائز*l.سائز*l.c*l.n;
    axpy_cpu(l.n, learning_شرح/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    اگر(l.scales){
        axpy_cpu(l.n, learning_شرح/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(سائز, -decay*batch, l.weights, 1, l.وزن_updates, 1);
    axpy_cpu(سائز, learning_شرح/batch, l.وزن_updates, 1, l.weights, 1);
    scal_cpu(سائز, momentum, l.وزن_updates, 1);
}



