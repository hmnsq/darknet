#ضمن "convolutional_layer.h"
#ضمن "utils.h"
#ضمن "batchnorm_layer.h"
#ضمن "im2col.h"
#ضمن "col2im.h"
#ضمن "blas.h"
#ضمن "gemm.h"
#ضمن <stdio.h>
#ضمن <time.h>

#جيڪڏهن_وصف_ٿيل AI2
#ضمن "xnor_layer.h"
#جيڪڏهن_پورو

فراغ مٽا_سٽا_ٻٽو(convolutional_layer *l)
{
    عائم *مٽا_سٽا = l->weights;
    l->weights = l->ٻٽو_weights;
    l->ٻٽو_weights = مٽا_سٽا;

#جيڪڏهن_وصف_ٿيل GPU
    مٽا_سٽا = l->weights_gpu;
    l->weights_gpu = l->ٻٽو_weights_gpu;
    l->ٻٽو_weights_gpu = مٽا_سٽا;
#جيڪڏهن_پورو
}

فراغ binarize_weights(عائم *weights, صحيح n, صحيح سائيز, عائم *ٻٽو)
{
    صحيح i, f;
    لكل(f = 0; f < n; ++f){
        عائم سراسري = 0;
        لكل(i = 0; i < سائيز; ++i){
            سراسري += fabs(weights[f*سائيز + i]);
        }
        سراسري = سراسري / سائيز;
        لكل(i = 0; i < سائيز; ++i){
            ٻٽو[f*سائيز + i] = (weights[f*سائيز + i] > 0) ? سراسري : -سراسري;
        }
    }
}

فراغ binarize_cpu(عائم *داخلا, صحيح n, عائم *ٻٽو)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        ٻٽو[i] = (داخلا[i] > 0) ? 1 : -1;
    }
}

فراغ binarize_داخلا(عائم *داخلا, صحيح n, صحيح سائيز, عائم *ٻٽو)
{
    صحيح i, s;
    لكل(s = 0; s < سائيز; ++s){
        عائم سراسري = 0;
        لكل(i = 0; i < n; ++i){
            سراسري += fabs(داخلا[i*سائيز + s]);
        }
        سراسري = سراسري / n;
        لكل(i = 0; i < n; ++i){
            ٻٽو[i*سائيز + s] = (داخلا[i*سائيز + s] > 0) ? سراسري : -سراسري;
        }
    }
}

صحيح convolutional_ٻاهريون_اوچائي(convolutional_layer l)
{
    أرجع (l.h + 2*l.pad - l.سائيز) / l.stride + 1;
}

صحيح convolutional_ٻاهريون_ويڪر(convolutional_layer l)
{
    أرجع (l.w + 2*l.pad - l.سائيز) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    أرجع float_منزل_image(l.ٻاهريون_w,l.ٻاهريون_h,l.ٻاهريون_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    أرجع float_منزل_image(l.ٻاهريون_w,l.ٻاهريون_h,l.ٻاهريون_c,l.delta);
}

ساكن size_t get_workspace_سائيز(layer l){
#جيڪڏهن_وصف_ٿيل CUDNN
    إذا(gpu_اشاريو >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnn_Get_Convolution_Forward_Workspace_سائيز(cudnn_handle(),
                l.srcTensorDesc,
                l.وزن_Desc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        إذا (s > most) most = s;
        cudnn_Get_Convolution_Backward_Filter_Workspace_سائيز(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        إذا (s > most) most = s;
        cudnn_Get_Convolution_Backward_معلومات_Workspace_سائيز(cudnn_handle(),
                l.وزن_Desc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        إذا (s > most) most = s;
        أرجع most;
    }
#جيڪڏهن_پورو
    أرجع (size_t)l.ٻاهريون_h*l.ٻاهريون_w*l.سائيز*l.سائيز*l.c/l.groups*حجم(عائم);
}

#جيڪڏهن_وصف_ٿيل GPU
#جيڪڏهن_وصف_ٿيل CUDNN
فراغ cudnn_convolutional_تياري(layer *l)
{
    cudnn_سيٽ_Tensor4d_Descriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l->batch, l->ٻاهريون_c, l->ٻاهريون_h, l->ٻاهريون_w); 

    cudnn_سيٽ_Tensor4d_Descriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, l->batch, l->ٻاهريون_c, l->ٻاهريون_h, l->ٻاهريون_w); 
    cudnn_سيٽ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_معلومات_FLOAT, 1, l->ٻاهريون_c, 1, 1); 

    cudnn_سيٽ_Filter4d_Descriptor(l->dweightDesc, CUDNN_معلومات_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->سائيز, l->سائيز); 
    cudnn_سيٽ_Filter4d_Descriptor(l->وزن_Desc, CUDNN_معلومات_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->سائيز, l->سائيز); 
    #إذا CUDNN_MAJOR >= 6
    cudnn_سيٽ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_معلومات_FLOAT);
    #وإلا
    cudnn_سيٽ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #جيڪڏهن_پورو

    #إذا CUDNN_MAJOR >= 7
    cudnn_سيٽ_Convolution_گروهه_ڳڻپ(l->convDesc, l->groups);
    #وإلا
    إذا(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #جيڪڏهن_پورو

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->وزن_Desc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_حد_بندي,
            2000000000,
            &l->fw_algo);
    cudnn_Get_Convolution_Backward_معلومات_Algorithm(cudnn_handle(),
            l->وزن_Desc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_معلومات_SPECIFY_WORKSPACE_حد_بندي,
            2000000000,
            &l->bd_algo);
    cudnnGetConvolutionBackwardFilterAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dweightDesc,
            CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_حد_بندي,
            2000000000,
            &l->bf_algo);
}
#جيڪڏهن_پورو
#جيڪڏهن_پورو

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح سائيز, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح ٻٽو, صحيح xnor, صحيح adam)
{
    صحيح i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.ٻٽو = ٻٽو;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.سائيز = سائيز;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = صفر_يادگيري_ڏيو(c/groups*n*سائيز*سائيز, حجم(عائم));
    l.وزن_updates = صفر_يادگيري_ڏيو(c/groups*n*سائيز*سائيز, حجم(عائم));

    l.biases = صفر_يادگيري_ڏيو(n, حجم(عائم));
    l.bias_updates = صفر_يادگيري_ڏيو(n, حجم(عائم));

    l.nweights = c/groups*n*سائيز*سائيز;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    عائم scale = جذر(2./(سائيز*سائيز*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    لكل(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_عام();
    صحيح ٻاهريون_w = convolutional_ٻاهريون_ويڪر(l);
    صحيح ٻاهريون_h = convolutional_ٻاهريون_اوچائي(l);
    l.ٻاهريون_h = ٻاهريون_h;
    l.ٻاهريون_w = ٻاهريون_w;
    l.ٻاهريون_c = n;
    l.outputs = l.ٻاهريون_h * l.ٻاهريون_w * l.ٻاهريون_c;
    l.inputs = l.w * l.h * l.c;

    l.output = صفر_يادگيري_ڏيو(l.batch*l.outputs, حجم(عائم));
    l.delta  = صفر_يادگيري_ڏيو(l.batch*l.outputs, حجم(عائم));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    إذا(ٻٽو){
        l.ٻٽو_weights = صفر_يادگيري_ڏيو(l.nweights, حجم(عائم));
        l.cweights = صفر_يادگيري_ڏيو(l.nweights, حجم(حرف));
        l.scales = صفر_يادگيري_ڏيو(n, حجم(عائم));
    }
    إذا(xnor){
        l.ٻٽو_weights = صفر_يادگيري_ڏيو(l.nweights, حجم(عائم));
        l.ٻٽو_داخلا = صفر_يادگيري_ڏيو(l.inputs*l.batch, حجم(عائم));
    }

    إذا(batch_normalize){
        l.scales = صفر_يادگيري_ڏيو(n, حجم(عائم));
        l.scale_updates = صفر_يادگيري_ڏيو(n, حجم(عائم));
        لكل(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.سراسري = صفر_يادگيري_ڏيو(n, حجم(عائم));
        l.variance = صفر_يادگيري_ڏيو(n, حجم(عائم));

        l.سراسري_delta = صفر_يادگيري_ڏيو(n, حجم(عائم));
        l.variance_delta = صفر_يادگيري_ڏيو(n, حجم(عائم));

        l.rolling_سراسري = صفر_يادگيري_ڏيو(n, حجم(عائم));
        l.rolling_variance = صفر_يادگيري_ڏيو(n, حجم(عائم));
        l.x = صفر_يادگيري_ڏيو(l.batch*l.outputs, حجم(عائم));
        l.x_norm = صفر_يادگيري_ڏيو(l.batch*l.outputs, حجم(عائم));
    }
    إذا(adam){
        l.m = صفر_يادگيري_ڏيو(l.nweights, حجم(عائم));
        l.v = صفر_يادگيري_ڏيو(l.nweights, حجم(عائم));
        l.bias_m = صفر_يادگيري_ڏيو(n, حجم(عائم));
        l.scale_m = صفر_يادگيري_ڏيو(n, حجم(عائم));
        l.bias_v = صفر_يادگيري_ڏيو(n, حجم(عائم));
        l.scale_v = صفر_يادگيري_ڏيو(n, حجم(عائم));
    }

#جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    إذا(gpu_اشاريو >= 0){
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

        l.delta_gpu = cuda_make_array(l.delta, l.batch*ٻاهريون_h*ٻاهريون_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*ٻاهريون_h*ٻاهريون_w*n);

        إذا(ٻٽو){
            l.ٻٽو_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        إذا(xnor){
            l.ٻٽو_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.ٻٽو_داخلا_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        إذا(batch_normalize){
            l.سراسري_gpu = cuda_make_array(l.سراسري, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_سراسري_gpu = cuda_make_array(l.سراسري, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.سراسري_delta_gpu = cuda_make_array(l.سراسري, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*ٻاهريون_h*ٻاهريون_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*ٻاهريون_h*ٻاهريون_w*n);
        }
#جيڪڏهن_وصف_ٿيل CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.وزن_Desc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_تياري(&l);
#جيڪڏهن_پورو
    }
#جيڪڏهن_پورو
    l.workspace_سائيز = get_workspace_سائيز(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, سائيز, سائيز, stride, w, h, c, l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c, (2.0 * l.n * l.سائيز*l.سائيز*l.c/l.groups * l.ٻاهريون_h*l.ٻاهريون_w)/1000000000.);

    أرجع l;
}

فراغ denormalize_convolutional_layer(convolutional_layer l)
{
    صحيح i, j;
    لكل(i = 0; i < l.n; ++i){
        عائم scale = l.scales[i]/جذر(l.rolling_variance[i] + .00001);
        لكل(j = 0; j < l.c/l.groups*l.سائيز*l.سائيز; ++j){
            l.weights[i*l.c/l.groups*l.سائيز*l.سائيز + j] *= scale;
        }
        l.biases[i] -= l.rolling_سراسري[i] * scale;
        l.scales[i] = 1;
        l.rolling_سراسري[i] = 0;
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
    صحيح ٻاهريون_w = convolutional_ٻاهريون_ويڪر(*l);
    صحيح ٻاهريون_h = convolutional_ٻاهريون_اوچائي(*l);

    l->ٻاهريون_w = ٻاهريون_w;
    l->ٻاهريون_h = ٻاهريون_h;

    l->outputs = l->ٻاهريون_h * l->ٻاهريون_w * l->ٻاهريون_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*حجم(عائم));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*حجم(عائم));
    إذا(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*حجم(عائم));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*حجم(عائم));
    }

#جيڪڏهن_وصف_ٿيل GPU
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
#جيڪڏهن_وصف_ٿيل CUDNN
    cudnn_convolutional_تياري(l);
#جيڪڏهن_پورو
#جيڪڏهن_پورو
    l->workspace_سائيز = get_workspace_سائيز(*l);
}

فراغ جوڙيو_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح سائيز)
{
    صحيح i,j,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            لكل(j = 0; j < سائيز; ++j){
                output[(b*n + i)*سائيز + j] += biases[i];
            }
        }
    }
}

فراغ scale_bias(عائم *output, عائم *scales, صحيح batch, صحيح n, صحيح سائيز)
{
    صحيح i,j,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            لكل(j = 0; j < سائيز; ++j){
                output[(b*n + i)*سائيز + j] *= scales[i];
            }
        }
    }
}

فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح سائيز)
{
    صحيح i,b;
    لكل(b = 0; b < batch; ++b){
        لكل(i = 0; i < n; ++i){
            bias_updates[i] += جوڙ_array(delta+سائيز*(i+b*n), سائيز);
        }
    }
}

فراغ forward_convolutional_layer(convolutional_layer l, ڄار net)
{
    صحيح i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    إذا(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.سائيز*l.سائيز, l.ٻٽو_weights);
        مٽا_سٽا_ٻٽو(&l);
        binarize_cpu(net.داخلا, l.c*l.h*l.w*l.batch, l.ٻٽو_داخلا);
        net.داخلا = l.ٻٽو_داخلا;
    }

    صحيح m = l.n/l.groups;
    صحيح k = l.سائيز*l.سائيز*l.c/l.groups;
    صحيح n = l.ٻاهريون_w*l.ٻاهريون_h;
    لكل(i = 0; i < l.batch; ++i){
        لكل(j = 0; j < l.groups; ++j){
            عائم *a = l.weights + j*l.nweights/l.groups;
            عائم *b = net.workspace;
            عائم *c = l.output + (i*l.groups + j)*n*m;
            عائم *im =  net.داخلا + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            إذا (l.سائيز == 1) {
                b = im;
            } وإلا {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.سائيز, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    إذا(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } وإلا {
        جوڙيو_bias(l.output, l.biases, l.batch, l.n, l.ٻاهريون_h*l.ٻاهريون_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    إذا(l.ٻٽو || l.xnor) مٽا_سٽا_ٻٽو(&l);
}

فراغ backward_convolutional_layer(convolutional_layer l, ڄار net)
{
    صحيح i, j;
    صحيح m = l.n/l.groups;
    صحيح n = l.سائيز*l.سائيز*l.c/l.groups;
    صحيح k = l.ٻاهريون_w*l.ٻاهريون_h;

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

            عائم *im  = net.داخلا + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            عائم *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            إذا(l.سائيز == 1){
                b = im;
            } وإلا {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.سائيز, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            إذا (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                إذا (l.سائيز == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                إذا (l.سائيز != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.سائيز, l.stride, l.pad, imd);
                }
            }
        }
    }
}

فراغ update_convolutional_layer(convolutional_layer l, update_دليلون a)
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
    صحيح h = l.سائيز;
    صحيح w = l.سائيز;
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
            عائم جوڙ = جوڙ_array(im.معلومات, im.w*im.h*im.c);
            l.biases[i] += جوڙ*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = صفر_يادگيري_ڏيو(l.n, حجم(image));
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

image *visualize_convolutional_layer(convolutional_layer l, حرف *دري, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, دري);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    حرف buff[256];
    sprintf(buff, "%s: Output", دري);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    أرجع single_weights;
}

