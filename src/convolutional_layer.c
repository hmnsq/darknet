#ضمن "convolutional_layer.h"
#ضمن "utils.h"
#ضمن "batchnorm_layer.h"
#ضمن "im2col.h"
#ضمن "col2im.h"
#ضمن "blas.h"
#ضمن "gemm.h"
#ضمن <stdio.h>
#ضمن <time.h>

#اگر_تعریف_شده AI2
#ضمن "xnor_layer.h"
#پایان_اگر

فراغ جابجا_دودویی(convolutional_layer *l)
{
    عائم *جابجا = l->weights;
    l->weights = l->دودویی_weights;
    l->دودویی_weights = جابجا;

#اگر_تعریف_شده GPU
    جابجا = l->weights_gpu;
    l->weights_gpu = l->دودویی_weights_gpu;
    l->دودویی_weights_gpu = جابجا;
#پایان_اگر
}

فراغ binarize_weights(عائم *weights, صحيح n, صحيح اندازه, عائم *دودویی)
{
    صحيح i, f;
    لكل(f = 0; f < n; ++f){
        عائم میانگین = 0;
        لكل(i = 0; i < اندازه; ++i){
            میانگین += fabs(weights[f*اندازه + i]);
        }
        میانگین = میانگین / اندازه;
        لكل(i = 0; i < اندازه; ++i){
            دودویی[f*اندازه + i] = (weights[f*اندازه + i] > 0) ? میانگین : -میانگین;
        }
    }
}

فراغ binarize_cpu(عائم *درون_داد, صحيح n, عائم *دودویی)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        دودویی[i] = (درون_داد[i] > 0) ? 1 : -1;
    }
}

فراغ binarize_درون_داد(عائم *درون_داد, صحيح n, صحيح اندازه, عائم *دودویی)
{
    صحيح i, s;
    لكل(s = 0; s < اندازه; ++s){
        عائم میانگین = 0;
        لكل(i = 0; i < n; ++i){
            میانگین += fabs(درون_داد[i*اندازه + s]);
        }
        میانگین = میانگین / n;
        لكل(i = 0; i < n; ++i){
            دودویی[i*اندازه + s] = (درون_داد[i*اندازه + s] > 0) ? میانگین : -میانگین;
        }
    }
}

صحيح convolutional_بیرون_ارتفاع(convolutional_layer l)
{
    أرجع (l.h + 2*l.pad - l.اندازه) / l.stride + 1;
}

صحيح convolutional_بیرون_عرض(convolutional_layer l)
{
    أرجع (l.w + 2*l.pad - l.اندازه) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    أرجع float_مقصد_image(l.بیرون_w,l.بیرون_h,l.بیرون_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    أرجع float_مقصد_image(l.بیرون_w,l.بیرون_h,l.بیرون_c,l.delta);
}

ساكن size_t get_workspace_اندازه(layer l){
#اگر_تعریف_شده CUDNN
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
        cudnn_Get_Convolution_Backward_داده_Workspace_اندازه(cudnn_handle(),
                l.وزن_Desc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        إذا (s > most) most = s;
        أرجع most;
    }
#پایان_اگر
    أرجع (size_t)l.بیرون_h*l.بیرون_w*l.اندازه*l.اندازه*l.c/l.groups*حجم(عائم);
}

#اگر_تعریف_شده GPU
#اگر_تعریف_شده CUDNN
فراغ cudnn_convolutional_راهاندازی(layer *l)
{
    cudnn_مجموعه_Tensor4d_Descriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_مجموعه_Tensor4d_Descriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, l->batch, l->بیرون_c, l->بیرون_h, l->بیرون_w); 

    cudnn_مجموعه_Tensor4d_Descriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_مجموعه_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, l->batch, l->بیرون_c, l->بیرون_h, l->بیرون_w); 
    cudnn_مجموعه_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_داده_FLOAT, 1, l->بیرون_c, 1, 1); 

    cudnn_مجموعه_Filter4d_Descriptor(l->dweightDesc, CUDNN_داده_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->اندازه, l->اندازه); 
    cudnn_مجموعه_Filter4d_Descriptor(l->وزن_Desc, CUDNN_داده_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->اندازه, l->اندازه); 
    #إذا CUDNN_MAJOR >= 6
    cudnn_مجموعه_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_داده_FLOAT);
    #وإلا
    cudnn_مجموعه_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #پایان_اگر

    #إذا CUDNN_MAJOR >= 7
    cudnn_مجموعه_Convolution_گروه_شمار(l->convDesc, l->groups);
    #وإلا
    إذا(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #پایان_اگر

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->وزن_Desc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_حد,
            2000000000,
            &l->fw_algo);
    cudnn_Get_Convolution_Backward_داده_Algorithm(cudnn_handle(),
            l->وزن_Desc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_داده_SPECIFY_WORKSPACE_حد,
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
#پایان_اگر
#پایان_اگر

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح اندازه, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح دودویی, صحيح xnor, صحيح adam)
{
    صحيح i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.دودویی = دودویی;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.اندازه = اندازه;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = حافظه_صفر_بده(c/groups*n*اندازه*اندازه, حجم(عائم));
    l.وزن_updates = حافظه_صفر_بده(c/groups*n*اندازه*اندازه, حجم(عائم));

    l.biases = حافظه_صفر_بده(n, حجم(عائم));
    l.bias_updates = حافظه_صفر_بده(n, حجم(عائم));

    l.nweights = c/groups*n*اندازه*اندازه;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    عائم scale = جذر(2./(اندازه*اندازه*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    لكل(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_نرمال();
    صحيح بیرون_w = convolutional_بیرون_عرض(l);
    صحيح بیرون_h = convolutional_بیرون_ارتفاع(l);
    l.بیرون_h = بیرون_h;
    l.بیرون_w = بیرون_w;
    l.بیرون_c = n;
    l.outputs = l.بیرون_h * l.بیرون_w * l.بیرون_c;
    l.inputs = l.w * l.h * l.c;

    l.output = حافظه_صفر_بده(l.batch*l.outputs, حجم(عائم));
    l.delta  = حافظه_صفر_بده(l.batch*l.outputs, حجم(عائم));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    إذا(دودویی){
        l.دودویی_weights = حافظه_صفر_بده(l.nweights, حجم(عائم));
        l.cweights = حافظه_صفر_بده(l.nweights, حجم(حرف));
        l.scales = حافظه_صفر_بده(n, حجم(عائم));
    }
    إذا(xnor){
        l.دودویی_weights = حافظه_صفر_بده(l.nweights, حجم(عائم));
        l.دودویی_درون_داد = حافظه_صفر_بده(l.inputs*l.batch, حجم(عائم));
    }

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
        l.m = حافظه_صفر_بده(l.nweights, حجم(عائم));
        l.v = حافظه_صفر_بده(l.nweights, حجم(عائم));
        l.bias_m = حافظه_صفر_بده(n, حجم(عائم));
        l.scale_m = حافظه_صفر_بده(n, حجم(عائم));
        l.bias_v = حافظه_صفر_بده(n, حجم(عائم));
        l.scale_v = حافظه_صفر_بده(n, حجم(عائم));
    }

#اگر_تعریف_شده GPU
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

        l.delta_gpu = cuda_make_array(l.delta, l.batch*بیرون_h*بیرون_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*بیرون_h*بیرون_w*n);

        إذا(دودویی){
            l.دودویی_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        إذا(xnor){
            l.دودویی_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.دودویی_درون_داد_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        إذا(batch_normalize){
            l.میانگین_gpu = cuda_make_array(l.میانگین, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_میانگین_gpu = cuda_make_array(l.میانگین, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.میانگین_delta_gpu = cuda_make_array(l.میانگین, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*بیرون_h*بیرون_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*بیرون_h*بیرون_w*n);
        }
#اگر_تعریف_شده CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.وزن_Desc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_راهاندازی(&l);
#پایان_اگر
    }
#پایان_اگر
    l.workspace_اندازه = get_workspace_اندازه(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, اندازه, اندازه, stride, w, h, c, l.بیرون_w, l.بیرون_h, l.بیرون_c, (2.0 * l.n * l.اندازه*l.اندازه*l.c/l.groups * l.بیرون_h*l.بیرون_w)/1000000000.);

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
        l.biases[i] -= l.rolling_میانگین[i] * scale;
        l.scales[i] = 1;
        l.rolling_میانگین[i] = 0;
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
    صحيح بیرون_w = convolutional_بیرون_عرض(*l);
    صحيح بیرون_h = convolutional_بیرون_ارتفاع(*l);

    l->بیرون_w = بیرون_w;
    l->بیرون_h = بیرون_h;

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
    cudnn_convolutional_راهاندازی(l);
#پایان_اگر
#پایان_اگر
    l->workspace_اندازه = get_workspace_اندازه(*l);
}

فراغ جمع_کن_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه)
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
            bias_updates[i] += جمع_array(delta+اندازه*(i+b*n), اندازه);
        }
    }
}

فراغ forward_convolutional_layer(convolutional_layer l, شبکه net)
{
    صحيح i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    إذا(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.اندازه*l.اندازه, l.دودویی_weights);
        جابجا_دودویی(&l);
        binarize_cpu(net.درون_داد, l.c*l.h*l.w*l.batch, l.دودویی_درون_داد);
        net.درون_داد = l.دودویی_درون_داد;
    }

    صحيح m = l.n/l.groups;
    صحيح k = l.اندازه*l.اندازه*l.c/l.groups;
    صحيح n = l.بیرون_w*l.بیرون_h;
    لكل(i = 0; i < l.batch; ++i){
        لكل(j = 0; j < l.groups; ++j){
            عائم *a = l.weights + j*l.nweights/l.groups;
            عائم *b = net.workspace;
            عائم *c = l.output + (i*l.groups + j)*n*m;
            عائم *im =  net.درون_داد + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

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
        جمع_کن_bias(l.output, l.biases, l.batch, l.n, l.بیرون_h*l.بیرون_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    إذا(l.دودویی || l.xnor) جابجا_دودویی(&l);
}

فراغ backward_convolutional_layer(convolutional_layer l, شبکه net)
{
    صحيح i, j;
    صحيح m = l.n/l.groups;
    صحيح n = l.اندازه*l.اندازه*l.c/l.groups;
    صحيح k = l.بیرون_w*l.بیرون_h;

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

            عائم *im  = net.درون_داد + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
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

فراغ update_convolutional_layer(convolutional_layer l, update_آرگومانها a)
{
    عائم learning_نرخ = a.learning_نرخ*l.learning_نرخ_scale;
    عائم momentum = a.momentum;
    عائم decay = a.decay;
    صحيح batch = a.batch;

    axpy_cpu(l.n, learning_نرخ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    إذا(l.scales){
        axpy_cpu(l.n, learning_نرخ/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.وزن_updates, 1);
    axpy_cpu(l.nweights, learning_نرخ/batch, l.وزن_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.وزن_updates, 1);
}


image get_convolutional_وزن(convolutional_layer l, صحيح i)
{
    صحيح h = l.اندازه;
    صحيح w = l.اندازه;
    صحيح c = l.c/l.groups;
    أرجع float_مقصد_image(w,h,c,l.weights+i*h*w*c);
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
            عائم جمع = جمع_array(im.داده, im.w*im.h*im.c);
            l.biases[i] += جمع*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = حافظه_صفر_بده(l.n, حجم(image));
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

image *visualize_convolutional_layer(convolutional_layer l, حرف *پنجره, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, پنجره);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    حرف buff[256];
    sprintf(buff, "%s: Output", پنجره);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    أرجع single_weights;
}

