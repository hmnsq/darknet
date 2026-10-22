#ضمن "convolutional_layer.h"
#ضمن "utils.h"
#ضمن "batchnorm_layer.h"
#ضمن "im2col.h"
#ضمن "col2im.h"
#ضمن "blas.h"
#ضمن "gemm.h"
#ضمن <stdio.h>
#ضمن <time.h>

#که_تعریف_شوی AI2
#ضمن "xnor_layer.h"
#د_که_پای

فراغ بدلون_دوګونی(convolutional_layer *l)
{
    عائم *بدلون = l->weights;
    l->weights = l->دوګونی_weights;
    l->دوګونی_weights = بدلون;

#که_تعریف_شوی GPU
    بدلون = l->weights_gpu;
    l->weights_gpu = l->دوګونی_weights_gpu;
    l->دوګونی_weights_gpu = بدلون;
#د_که_پای
}

فراغ binarize_weights(عائم *weights, صحيح n, صحيح اندازه, عائم *دوګونی)
{
    صحيح i, f;
    لكل(f = 0; f < n; ++f){
        عائم منځنۍ = 0;
        لكل(i = 0; i < اندازه; ++i){
            منځنۍ += fabs(weights[f*اندازه + i]);
        }
        منځنۍ = منځنۍ / اندازه;
        لكل(i = 0; i < اندازه; ++i){
            دوګونی[f*اندازه + i] = (weights[f*اندازه + i] > 0) ? منځنۍ : -منځنۍ;
        }
    }
}

فراغ binarize_cpu(عائم *ننوت, صحيح n, عائم *دوګونی)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        دوګونی[i] = (ننوت[i] > 0) ? 1 : -1;
    }
}

فراغ binarize_ننوت(عائم *ننوت, صحيح n, صحيح اندازه, عائم *دوګونی)
{
    صحيح i, s;
    لكل(s = 0; s < اندازه; ++s){
        عائم منځنۍ = 0;
        لكل(i = 0; i < n; ++i){
            منځنۍ += fabs(ننوت[i*اندازه + s]);
        }
        منځنۍ = منځنۍ / n;
        لكل(i = 0; i < n; ++i){
            دوګونی[i*اندازه + s] = (ننوت[i*اندازه + s] > 0) ? منځنۍ : -منځنۍ;
        }
    }
}

صحيح convolutional_بهر_لوړوالی(convolutional_layer l)
{
    أرجع (l.h + 2*l.pad - l.اندازه) / l.stride + 1;
}

صحيح convolutional_بهر_پلنوالی(convolutional_layer l)
{
    أرجع (l.w + 2*l.pad - l.اندازه) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    أرجع float_منزل_image(l.بهر_w,l.بهر_h,l.بهر_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    أرجع float_منزل_image(l.بهر_w,l.بهر_h,l.بهر_c,l.delta);
}

ساكن size_t get_workspace_اندازه(layer l){
#که_تعریف_شوی CUDNN
    إذا(gpu_شاخص >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnn_Get_Convolution_Forward_Workspace_اندازه(cudnn_handle(),
                l.srcTensorDesc,
                l.وزن_Desc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        إذا (s > most) most = s;
        cudnn_Get_Convolution_Backward_Filter_Workspace_اندازه(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        إذا (s > most) most = s;
        cudnn_Get_Convolution_Backward_معلومات_Workspace_اندازه(cudnn_handle(),
                l.وزن_Desc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        إذا (s > most) most = s;
        أرجع most;
    }
#د_که_پای
    أرجع (size_t)l.بهر_h*l.بهر_w*l.اندازه*l.اندازه*l.c/l.groups*حجم(عائم);
}

#که_تعریف_شوی GPU
#که_تعریف_شوی CUDNN
فراغ cudnn_convolutional_تیاری(layer *l)
{
    cudnn_سېټ_Tensor4d_Descriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_سېټ_Tensor4d_Descriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l->batch, l->بهر_c, l->بهر_h, l->بهر_w); 

    cudnn_سېټ_Tensor4d_Descriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_سېټ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l->batch, l->بهر_c, l->بهر_h, l->بهر_w); 
    cudnn_سېټ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, 1, l->بهر_c, 1, 1); 

    cudnn_سېټ_Filter4d_Descriptor(l->dweightDesc, CUDNN_معلومات_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->اندازه, l->اندازه); 
    cudnn_سېټ_Filter4d_Descriptor(l->وزن_Desc, CUDNN_معلومات_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->اندازه, l->اندازه); 
    #إذا CUDNN_MAJOR >= 6
    cudnn_سېټ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_معلومات_FLOAT);
    #وإلا
    cudnn_سېټ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #د_که_پای

    #إذا CUDNN_MAJOR >= 7
    cudnn_سېټ_Convolution_ډله_شمېر(l->convDesc, l->groups);
    #وإلا
    إذا(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #د_که_پای

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->وزن_Desc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_حد,
            2000000000,
            &l->fw_algo);
    cudnn_Get_Convolution_Backward_معلومات_Algorithm(cudnn_handle(),
            l->وزن_Desc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_معلومات_SPECIFY_WORKSPACE_حد,
            2000000000,
            &l->bd_algo);
    cudnnGetConvolutionBackwardFilterAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dweightDesc,
            CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_حد,
            2000000000,
            &l->bf_algo);
}
#د_که_پای
#د_که_پای

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح اندازه, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح دوګونی, صحيح xnor, صحيح adam)
{
    صحيح i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.دوګونی = دوګونی;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.اندازه = اندازه;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = حافظه_صفر_ورکړه(c/groups*n*اندازه*اندازه, حجم(عائم));
    l.وزن_updates = حافظه_صفر_ورکړه(c/groups*n*اندازه*اندازه, حجم(عائم));

    l.biases = حافظه_صفر_ورکړه(n, حجم(عائم));
    l.bias_updates = حافظه_صفر_ورکړه(n, حجم(عائم));

    l.nweights = c/groups*n*اندازه*اندازه;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    عائم scale = جذر(2./(اندازه*اندازه*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    لكل(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_نورمال();
    صحيح بهر_w = convolutional_بهر_پلنوالی(l);
    صحيح بهر_h = convolutional_بهر_لوړوالی(l);
    l.بهر_h = بهر_h;
    l.بهر_w = بهر_w;
    l.بهر_c = n;
    l.outputs = l.بهر_h * l.بهر_w * l.بهر_c;
    l.inputs = l.w * l.h * l.c;

    l.output = حافظه_صفر_ورکړه(l.batch*l.outputs, حجم(عائم));
    l.delta  = حافظه_صفر_ورکړه(l.batch*l.outputs, حجم(عائم));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    إذا(دوګونی){
        l.دوګونی_weights = حافظه_صفر_ورکړه(l.nweights, حجم(عائم));
        l.cweights = حافظه_صفر_ورکړه(l.nweights, حجم(حرف));
        l.scales = حافظه_صفر_ورکړه(n, حجم(عائم));
    }
    إذا(xnor){
        l.دوګونی_weights = حافظه_صفر_ورکړه(l.nweights, حجم(عائم));
        l.دوګونی_ننوت = حافظه_صفر_ورکړه(l.inputs*l.batch, حجم(عائم));
    }

    إذا(batch_normalize){
        l.scales = حافظه_صفر_ورکړه(n, حجم(عائم));
        l.scale_updates = حافظه_صفر_ورکړه(n, حجم(عائم));
        لكل(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.منځنۍ = حافظه_صفر_ورکړه(n, حجم(عائم));
        l.variance = حافظه_صفر_ورکړه(n, حجم(عائم));

        l.منځنۍ_delta = حافظه_صفر_ورکړه(n, حجم(عائم));
        l.variance_delta = حافظه_صفر_ورکړه(n, حجم(عائم));

        l.rolling_منځنۍ = حافظه_صفر_ورکړه(n, حجم(عائم));
        l.rolling_variance = حافظه_صفر_ورکړه(n, حجم(عائم));
        l.x = حافظه_صفر_ورکړه(l.batch*l.outputs, حجم(عائم));
        l.x_norm = حافظه_صفر_ورکړه(l.batch*l.outputs, حجم(عائم));
    }
    إذا(adam){
        l.m = حافظه_صفر_ورکړه(l.nweights, حجم(عائم));
        l.v = حافظه_صفر_ورکړه(l.nweights, حجم(عائم));
        l.bias_m = حافظه_صفر_ورکړه(n, حجم(عائم));
        l.scale_m = حافظه_صفر_ورکړه(n, حجم(عائم));
        l.bias_v = حافظه_صفر_ورکړه(n, حجم(عائم));
        l.scale_v = حافظه_صفر_ورکړه(n, حجم(عائم));
    }

#که_تعریف_شوی GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    إذا(gpu_شاخص >= 0){
        إذا (adam) {
            l.m_gpu = cuda_make_array(l.m, l.nweights);
            l.v_gpu = cuda_make_array(l.v, l.nweights);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }

        l.weights_gpu = cuda_make_array(l.weights, l.nweights);
        l.وزن_updates_gpu = cuda_make_array(l.وزن_updates, l.nweights);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*بهر_h*بهر_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*بهر_h*بهر_w*n);

        إذا(دوګونی){
            l.دوګونی_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        إذا(xnor){
            l.دوګونی_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.دوګونی_ننوت_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        إذا(batch_normalize){
            l.منځنۍ_gpu = cuda_make_array(l.منځنۍ, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_منځنۍ_gpu = cuda_make_array(l.منځنۍ, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.منځنۍ_delta_gpu = cuda_make_array(l.منځنۍ, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*بهر_h*بهر_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*بهر_h*بهر_w*n);
        }
#که_تعریف_شوی CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.وزن_Desc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_تیاری(&l);
#د_که_پای
    }
#د_که_پای
    l.workspace_اندازه = get_workspace_اندازه(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, اندازه, اندازه, stride, w, h, c, l.بهر_w, l.بهر_h, l.بهر_c, (2.0 * l.n * l.اندازه*l.اندازه*l.c/l.groups * l.بهر_h*l.بهر_w)/1000000000.);

    أرجع l;
}

فراغ denormalize_convolutional_layer(convolutional_layer l)
{
    صحيح i, j;
    لكل(i = 0; i < l.n; ++i){
        عائم scale = l.scales[i]/جذر(l.rolling_variance[i] + .00001);
        لكل(j = 0; j < l.c/l.groups*l.اندازه*l.اندازه; ++j){
            l.weights[i*l.c/l.groups*l.اندازه*l.اندازه + j] *= scale;
        }
        l.biases[i] -= l.rolling_منځنۍ[i] * scale;
        l.scales[i] = 1;
        l.rolling_منځنۍ[i] = 0;
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

فراغ resize_convolutional_layer(convolutional_layer *l, صحيح w, صحيح h)
{
    l->w = w;
    l->h = h;
    صحيح بهر_w = convolutional_بهر_پلنوالی(*l);
    صحيح بهر_h = convolutional_بهر_لوړوالی(*l);

    l->بهر_w = بهر_w;
    l->بهر_h = بهر_h;

    l->outputs = l->بهر_h * l->بهر_w * l->بهر_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*حجم(عائم));
    إذا(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*حجم(عائم));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*حجم(عائم));
    }

#که_تعریف_شوی GPU
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
#که_تعریف_شوی CUDNN
    cudnn_convolutional_تیاری(l);
#د_که_پای
#د_که_پای
    l->workspace_اندازه = get_workspace_اندازه(*l);
}

فراغ جمع_کړه_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه)
{
    صحيح i,j,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            لكل(j = 0; j < اندازه; ++j){
                output[(b*n + i)*اندازه + j] += biases[i];
            }
        }
    }
}

فراغ scale_bias(عائم *output, عائم *scales, صحيح batch, صحيح n, صحيح اندازه)
{
    صحيح i,j,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            لكل(j = 0; j < اندازه; ++j){
                output[(b*n + i)*اندازه + j] *= scales[i];
            }
        }
    }
}

فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح اندازه)
{
    صحيح i,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            bias_updates[i] += مجموعه_array(delta+اندازه*(i+b*n), اندازه);
        }
    }
}

فراغ forward_convolutional_layer(convolutional_layer l, شبکه net)
{
    صحيح i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    إذا(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.اندازه*l.اندازه, l.دوګونی_weights);
        بدلون_دوګونی(&l);
        binarize_cpu(net.ننوت, l.c*l.h*l.w*l.batch, l.دوګونی_ننوت);
        net.ننوت = l.دوګونی_ننوت;
    }

    صحيح m = l.n/l.groups;
    صحيح k = l.اندازه*l.اندازه*l.c/l.groups;
    صحيح n = l.بهر_w*l.بهر_h;
    لكل(i = 0; i < l.batch; ++i){
        لكل(j = 0; j < l.groups; ++j){
            عائم *a = l.weights + j*l.nweights/l.groups;
            عائم *b = net.workspace;
            عائم *c = l.output + (i*l.groups + j)*n*m;
            عائم *im =  net.ننوت + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            إذا (l.اندازه == 1) {
                b = im;
            } وإلا {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.اندازه, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    إذا(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } وإلا {
        جمع_کړه_bias(l.output, l.biases, l.batch, l.n, l.بهر_h*l.بهر_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    إذا(l.دوګونی || l.xnor) بدلون_دوګونی(&l);
}

فراغ backward_convolutional_layer(convolutional_layer l, شبکه net)
{
    صحيح i, j;
    صحيح m = l.n/l.groups;
    صحيح n = l.اندازه*l.اندازه*l.c/l.groups;
    صحيح k = l.بهر_w*l.بهر_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    إذا(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } وإلا {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    لكل(i = 0; i < l.batch; ++i){
        لكل(j = 0; j < l.groups; ++j){
            عائم *a = l.delta + (i*l.groups + j)*m*k;
            عائم *b = net.workspace;
            عائم *c = l.وزن_updates + j*l.nweights/l.groups;

            عائم *im  = net.ننوت + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            عائم *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            إذا(l.اندازه == 1){
                b = im;
            } وإلا {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.اندازه, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            إذا (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                إذا (l.اندازه == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                إذا (l.اندازه != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.اندازه, l.stride, l.pad, imd);
                }
            }
        }
    }
}

فراغ update_convolutional_layer(convolutional_layer l, update_دلیلونه a)
{
    عائم learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    axpy_cpu(l.n, learning_شرح/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    إذا(l.scales){
        axpy_cpu(l.n, learning_شرح/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.وزن_updates, 1);
    axpy_cpu(l.nweights, learning_شرح/batch, l.وزن_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.وزن_updates, 1);
}


image get_convolutional_وزن(convolutional_layer l, صحيح i)
{
    صحيح h = l.اندازه;
    صحيح w = l.اندازه;
    صحيح c = l.c/l.groups;
    أرجع float_منزل_image(w,h,c,l.weights+i*h*w*c);
}

فراغ rgbgr_weights(convolutional_layer l)
{
    صحيح i;
    لكل(i = 0; i < l.n; ++i){
        image im = get_convolutional_وزن(l, i);
        إذا (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

فراغ rescale_weights(convolutional_layer l, عائم scale, عائم trans)
{
    صحيح i;
    لكل(i = 0; i < l.n; ++i){
        image im = get_convolutional_وزن(l, i);
        إذا (im.c == 3) {
            scale_image(im, scale);
            عائم مجموعه = مجموعه_array(im.معلومات, im.w*im.h*im.c);
            l.biases[i] += مجموعه*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = حافظه_صفر_ورکړه(l.n, حجم(image));
    صحيح i;
    لكل(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_وزن(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    أرجع weights;
}

image *visualize_convolutional_layer(convolutional_layer l, حرف *کړکۍ, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, کړکۍ);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    حرف buff[256];
    sprintf(buff, "%s: Output", کړکۍ);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    أرجع single_weights;
}

