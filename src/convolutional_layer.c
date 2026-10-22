#شامل "convolutional_layer.h"
#شامل "utils.h"
#شامل "batchnorm_layer.h"
#شامل "im2col.h"
#شامل "col2im.h"
#شامل "blas.h"
#شامل "gemm.h"
#شامل <stdio.h>
#شامل <time.h>

#اگر_تعریف_شدہ AI2
#شامل "xnor_layer.h"
#اگر_ختم

خالی تبادلہ_ثنائی(convolutional_layer *l)
{
    اعشاری *تبادلہ = l->weights;
    l->weights = l->ثنائی_weights;
    l->ثنائی_weights = تبادلہ;

#اگر_تعریف_شدہ GPU
    تبادلہ = l->weights_gpu;
    l->weights_gpu = l->ثنائی_weights_gpu;
    l->ثنائی_weights_gpu = تبادلہ;
#اگر_ختم
}

خالی binarize_weights(اعشاری *weights, صحیح n, صحیح سائز, اعشاری *ثنائی)
{
    صحیح i, f;
    برائے(f = 0; f < n; ++f){
        اعشاری اوسط = 0;
        برائے(i = 0; i < سائز; ++i){
            اوسط += fabs(weights[f*سائز + i]);
        }
        اوسط = اوسط / سائز;
        برائے(i = 0; i < سائز; ++i){
            ثنائی[f*سائز + i] = (weights[f*سائز + i] > 0) ? اوسط : -اوسط;
        }
    }
}

خالی binarize_cpu(اعشاری *داخلہ, صحیح n, اعشاری *ثنائی)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        ثنائی[i] = (داخلہ[i] > 0) ? 1 : -1;
    }
}

خالی binarize_داخلہ(اعشاری *داخلہ, صحیح n, صحیح سائز, اعشاری *ثنائی)
{
    صحیح i, s;
    برائے(s = 0; s < سائز; ++s){
        اعشاری اوسط = 0;
        برائے(i = 0; i < n; ++i){
            اوسط += fabs(داخلہ[i*سائز + s]);
        }
        اوسط = اوسط / n;
        برائے(i = 0; i < n; ++i){
            ثنائی[i*سائز + s] = (داخلہ[i*سائز + s] > 0) ? اوسط : -اوسط;
        }
    }
}

صحیح convolutional_برآمد_اونچائی(convolutional_layer l)
{
    واپس (l.h + 2*l.pad - l.سائز) / l.stride + 1;
}

صحیح convolutional_برآمد_چوڑائی(convolutional_layer l)
{
    واپس (l.w + 2*l.pad - l.سائز) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    واپس float_مقصد_image(l.برآمد_w,l.برآمد_h,l.برآمد_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    واپس float_مقصد_image(l.برآمد_w,l.برآمد_h,l.برآمد_c,l.delta);
}

ساکن size_t get_workspace_سائز(layer l){
#اگر_تعریف_شدہ CUDNN
    اگر(gpu_اشاریہ >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnn_Get_Convolution_Forward_Workspace_سائز(cudnn_handle(),
                l.srcTensorDesc,
                l.وزن_Desc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        اگر (s > most) most = s;
        cudnn_Get_Convolution_Backward_Filter_Workspace_سائز(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        اگر (s > most) most = s;
        cudnn_Get_Convolution_Backward_ڈیٹا_Workspace_سائز(cudnn_handle(),
                l.وزن_Desc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        اگر (s > most) most = s;
        واپس most;
    }
#اگر_ختم
    واپس (size_t)l.برآمد_h*l.برآمد_w*l.سائز*l.سائز*l.c/l.groups*حجم(اعشاری);
}

#اگر_تعریف_شدہ GPU
#اگر_تعریف_شدہ CUDNN
خالی cudnn_convolutional_تیاری(layer *l)
{
    cudnn_سیٹ_Tensor4d_Descriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, l->batch, l->برآمد_c, l->برآمد_h, l->برآمد_w); 

    cudnn_سیٹ_Tensor4d_Descriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, l->batch, l->برآمد_c, l->برآمد_h, l->برآمد_w); 
    cudnn_سیٹ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ڈیٹا_FLOAT, 1, l->برآمد_c, 1, 1); 

    cudnn_سیٹ_Filter4d_Descriptor(l->dweightDesc, CUDNN_ڈیٹا_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->سائز, l->سائز); 
    cudnn_سیٹ_Filter4d_Descriptor(l->وزن_Desc, CUDNN_ڈیٹا_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->سائز, l->سائز); 
    #اگر CUDNN_MAJOR >= 6
    cudnn_سیٹ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_ڈیٹا_FLOAT);
    #ورنہ
    cudnn_سیٹ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #اگر_ختم

    #اگر CUDNN_MAJOR >= 7
    cudnn_سیٹ_Convolution_گروہ_گنتی(l->convDesc, l->groups);
    #ورنہ
    اگر(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #اگر_ختم

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->وزن_Desc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_حد_بندی,
            2000000000,
            &l->fw_algo);
    cudnn_Get_Convolution_Backward_ڈیٹا_Algorithm(cudnn_handle(),
            l->وزن_Desc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_ڈیٹا_SPECIFY_WORKSPACE_حد_بندی,
            2000000000,
            &l->bd_algo);
    cudnnGetConvolutionBackwardFilterAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dweightDesc,
            CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_حد_بندی,
            2000000000,
            &l->bf_algo);
}
#اگر_ختم
#اگر_ختم

convolutional_layer make_convolutional_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح groups, صحیح سائز, صحیح stride, صحیح padding, ACTIVATION activation, صحیح batch_normalize, صحیح ثنائی, صحیح xnor, صحیح adam)
{
    صحیح i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.ثنائی = ثنائی;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.سائز = سائز;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = صفر_حافظہ_دو(c/groups*n*سائز*سائز, حجم(اعشاری));
    l.وزن_updates = صفر_حافظہ_دو(c/groups*n*سائز*سائز, حجم(اعشاری));

    l.biases = صفر_حافظہ_دو(n, حجم(اعشاری));
    l.bias_updates = صفر_حافظہ_دو(n, حجم(اعشاری));

    l.nweights = c/groups*n*سائز*سائز;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    اعشاری scale = جذر(2./(سائز*سائز*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    برائے(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_معمول();
    صحیح برآمد_w = convolutional_برآمد_چوڑائی(l);
    صحیح برآمد_h = convolutional_برآمد_اونچائی(l);
    l.برآمد_h = برآمد_h;
    l.برآمد_w = برآمد_w;
    l.برآمد_c = n;
    l.outputs = l.برآمد_h * l.برآمد_w * l.برآمد_c;
    l.inputs = l.w * l.h * l.c;

    l.output = صفر_حافظہ_دو(l.batch*l.outputs, حجم(اعشاری));
    l.delta  = صفر_حافظہ_دو(l.batch*l.outputs, حجم(اعشاری));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    اگر(ثنائی){
        l.ثنائی_weights = صفر_حافظہ_دو(l.nweights, حجم(اعشاری));
        l.cweights = صفر_حافظہ_دو(l.nweights, حجم(حرف));
        l.scales = صفر_حافظہ_دو(n, حجم(اعشاری));
    }
    اگر(xnor){
        l.ثنائی_weights = صفر_حافظہ_دو(l.nweights, حجم(اعشاری));
        l.ثنائی_داخلہ = صفر_حافظہ_دو(l.inputs*l.batch, حجم(اعشاری));
    }

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
        l.m = صفر_حافظہ_دو(l.nweights, حجم(اعشاری));
        l.v = صفر_حافظہ_دو(l.nweights, حجم(اعشاری));
        l.bias_m = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.scale_m = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.bias_v = صفر_حافظہ_دو(n, حجم(اعشاری));
        l.scale_v = صفر_حافظہ_دو(n, حجم(اعشاری));
    }

#اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    اگر(gpu_اشاریہ >= 0){
        اگر (adam) {
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

        l.delta_gpu = cuda_make_array(l.delta, l.batch*برآمد_h*برآمد_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*برآمد_h*برآمد_w*n);

        اگر(ثنائی){
            l.ثنائی_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        اگر(xnor){
            l.ثنائی_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.ثنائی_داخلہ_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        اگر(batch_normalize){
            l.اوسط_gpu = cuda_make_array(l.اوسط, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_اوسط_gpu = cuda_make_array(l.اوسط, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.اوسط_delta_gpu = cuda_make_array(l.اوسط, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*برآمد_h*برآمد_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*برآمد_h*برآمد_w*n);
        }
#اگر_تعریف_شدہ CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.وزن_Desc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_تیاری(&l);
#اگر_ختم
    }
#اگر_ختم
    l.workspace_سائز = get_workspace_سائز(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, سائز, سائز, stride, w, h, c, l.برآمد_w, l.برآمد_h, l.برآمد_c, (2.0 * l.n * l.سائز*l.سائز*l.c/l.groups * l.برآمد_h*l.برآمد_w)/1000000000.);

    واپس l;
}

خالی denormalize_convolutional_layer(convolutional_layer l)
{
    صحیح i, j;
    برائے(i = 0; i < l.n; ++i){
        اعشاری scale = l.scales[i]/جذر(l.rolling_variance[i] + .00001);
        برائے(j = 0; j < l.c/l.groups*l.سائز*l.سائز; ++j){
            l.weights[i*l.c/l.groups*l.سائز*l.سائز + j] *= scale;
        }
        l.biases[i] -= l.rolling_اوسط[i] * scale;
        l.scales[i] = 1;
        l.rolling_اوسط[i] = 0;
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

خالی resize_convolutional_layer(convolutional_layer *l, صحیح w, صحیح h)
{
    l->w = w;
    l->h = h;
    صحیح برآمد_w = convolutional_برآمد_چوڑائی(*l);
    صحیح برآمد_h = convolutional_برآمد_اونچائی(*l);

    l->برآمد_w = برآمد_w;
    l->برآمد_h = برآمد_h;

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
    cudnn_convolutional_تیاری(l);
#اگر_ختم
#اگر_ختم
    l->workspace_سائز = get_workspace_سائز(*l);
}

خالی جوڑو_bias(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح سائز)
{
    صحیح i,j,b;
    برائے(b = 0; b < batch; ++b){
        برائے(i = 0; i < n; ++i){
            برائے(j = 0; j < سائز; ++j){
                output[(b*n + i)*سائز + j] += biases[i];
            }
        }
    }
}

خالی scale_bias(اعشاری *output, اعشاری *scales, صحیح batch, صحیح n, صحیح سائز)
{
    صحیح i,j,b;
    برائے(b = 0; b < batch; ++b){
        برائے(i = 0; i < n; ++i){
            برائے(j = 0; j < سائز; ++j){
                output[(b*n + i)*سائز + j] *= scales[i];
            }
        }
    }
}

خالی backward_bias(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح سائز)
{
    صحیح i,b;
    برائے(b = 0; b < batch; ++b){
        برائے(i = 0; i < n; ++i){
            bias_updates[i] += مجموع_array(delta+سائز*(i+b*n), سائز);
        }
    }
}

خالی forward_convolutional_layer(convolutional_layer l, جال net)
{
    صحیح i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    اگر(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.سائز*l.سائز, l.ثنائی_weights);
        تبادلہ_ثنائی(&l);
        binarize_cpu(net.داخلہ, l.c*l.h*l.w*l.batch, l.ثنائی_داخلہ);
        net.داخلہ = l.ثنائی_داخلہ;
    }

    صحیح m = l.n/l.groups;
    صحیح k = l.سائز*l.سائز*l.c/l.groups;
    صحیح n = l.برآمد_w*l.برآمد_h;
    برائے(i = 0; i < l.batch; ++i){
        برائے(j = 0; j < l.groups; ++j){
            اعشاری *a = l.weights + j*l.nweights/l.groups;
            اعشاری *b = net.workspace;
            اعشاری *c = l.output + (i*l.groups + j)*n*m;
            اعشاری *im =  net.داخلہ + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            اگر (l.سائز == 1) {
                b = im;
            } ورنہ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.سائز, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    اگر(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ورنہ {
        جوڑو_bias(l.output, l.biases, l.batch, l.n, l.برآمد_h*l.برآمد_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    اگر(l.ثنائی || l.xnor) تبادلہ_ثنائی(&l);
}

خالی backward_convolutional_layer(convolutional_layer l, جال net)
{
    صحیح i, j;
    صحیح m = l.n/l.groups;
    صحیح n = l.سائز*l.سائز*l.c/l.groups;
    صحیح k = l.برآمد_w*l.برآمد_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    اگر(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ورنہ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    برائے(i = 0; i < l.batch; ++i){
        برائے(j = 0; j < l.groups; ++j){
            اعشاری *a = l.delta + (i*l.groups + j)*m*k;
            اعشاری *b = net.workspace;
            اعشاری *c = l.وزن_updates + j*l.nweights/l.groups;

            اعشاری *im  = net.داخلہ + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            اعشاری *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            اگر(l.سائز == 1){
                b = im;
            } ورنہ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.سائز, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            اگر (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                اگر (l.سائز == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                اگر (l.سائز != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.سائز, l.stride, l.pad, imd);
                }
            }
        }
    }
}

خالی update_convolutional_layer(convolutional_layer l, update_دلائل a)
{
    اعشاری learning_شرح = a.learning_شرح*l.learning_شرح_scale;
    اعشاری momentum = a.momentum;
    اعشاری decay = a.decay;
    صحیح batch = a.batch;

    axpy_cpu(l.n, learning_شرح/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    اگر(l.scales){
        axpy_cpu(l.n, learning_شرح/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.وزن_updates, 1);
    axpy_cpu(l.nweights, learning_شرح/batch, l.وزن_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.وزن_updates, 1);
}


image get_convolutional_وزن(convolutional_layer l, صحیح i)
{
    صحیح h = l.سائز;
    صحیح w = l.سائز;
    صحیح c = l.c/l.groups;
    واپس float_مقصد_image(w,h,c,l.weights+i*h*w*c);
}

خالی rgbgr_weights(convolutional_layer l)
{
    صحیح i;
    برائے(i = 0; i < l.n; ++i){
        image im = get_convolutional_وزن(l, i);
        اگر (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

خالی rescale_weights(convolutional_layer l, اعشاری scale, اعشاری trans)
{
    صحیح i;
    برائے(i = 0; i < l.n; ++i){
        image im = get_convolutional_وزن(l, i);
        اگر (im.c == 3) {
            scale_image(im, scale);
            اعشاری مجموع = مجموع_array(im.ڈیٹا, im.w*im.h*im.c);
            l.biases[i] += مجموع*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = صفر_حافظہ_دو(l.n, حجم(image));
    صحیح i;
    برائے(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_وزن(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    واپس weights;
}

image *visualize_convolutional_layer(convolutional_layer l, حرف *کھڑکی, image *پچھلا_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, کھڑکی);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    حرف buff[256];
    sprintf(buff, "%s: Output", کھڑکی);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    واپس single_weights;
}

