#ਅਨ੍ਤਰ੍ਭਾਵ "convolutional_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "batchnorm_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "im2col.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "col2im.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "gemm.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ AI2
#ਅਨ੍ਤਰ੍ਭਾਵ "xnor_layer.h"
#ਜੇ_ਅੰਤ

ਖਾਲੀ ਅਦਲਾ_ਬਦਲੀ_ਦੁਆਧਾਰੀ(convolutional_layer *l)
{
    ਦਸ਼ਮਲਵ *ਅਦਲਾ_ਬਦਲੀ = l->weights;
    l->weights = l->ਦੁਆਧਾਰੀ_weights;
    l->ਦੁਆਧਾਰੀ_weights = ਅਦਲਾ_ਬਦਲੀ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    ਅਦਲਾ_ਬਦਲੀ = l->weights_gpu;
    l->weights_gpu = l->ਦੁਆਧਾਰੀ_weights_gpu;
    l->ਦੁਆਧਾਰੀ_weights_gpu = ਅਦਲਾ_ਬਦਲੀ;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ binarize_weights(ਦਸ਼ਮਲਵ *weights, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ *ਦੁਆਧਾਰੀ)
{
    ਪੂਰਨ i, f;
    ਲਈ(f = 0; f < n; ++f){
        ਦਸ਼ਮਲਵ ਔਸਤ = 0;
        ਲਈ(i = 0; i < ਮਾਪ; ++i){
            ਔਸਤ += fabs(weights[f*ਮਾਪ + i]);
        }
        ਔਸਤ = ਔਸਤ / ਮਾਪ;
        ਲਈ(i = 0; i < ਮਾਪ; ++i){
            ਦੁਆਧਾਰੀ[f*ਮਾਪ + i] = (weights[f*ਮਾਪ + i] > 0) ? ਔਸਤ : -ਔਸਤ;
        }
    }
}

ਖਾਲੀ binarize_cpu(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ n, ਦਸ਼ਮਲਵ *ਦੁਆਧਾਰੀ)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਦੁਆਧਾਰੀ[i] = (ਨਿਵੇਸ਼[i] > 0) ? 1 : -1;
    }
}

ਖਾਲੀ binarize_ਨਿਵੇਸ਼(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ *ਦੁਆਧਾਰੀ)
{
    ਪੂਰਨ i, s;
    ਲਈ(s = 0; s < ਮਾਪ; ++s){
        ਦਸ਼ਮਲਵ ਔਸਤ = 0;
        ਲਈ(i = 0; i < n; ++i){
            ਔਸਤ += fabs(ਨਿਵੇਸ਼[i*ਮਾਪ + s]);
        }
        ਔਸਤ = ਔਸਤ / n;
        ਲਈ(i = 0; i < n; ++i){
            ਦੁਆਧਾਰੀ[i*ਮਾਪ + s] = (ਨਿਵੇਸ਼[i*ਮਾਪ + s] > 0) ? ਔਸਤ : -ਔਸਤ;
        }
    }
}

ਪੂਰਨ convolutional_ਬਾਹਰਲਾ_ਉਚਾਈ(convolutional_layer l)
{
    ਵਾਪਸ (l.h + 2*l.pad - l.ਮਾਪ) / l.stride + 1;
}

ਪੂਰਨ convolutional_ਬਾਹਰਲਾ_ਚੌੜਾਈ(convolutional_layer l)
{
    ਵਾਪਸ (l.w + 2*l.pad - l.ਮਾਪ) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    ਵਾਪਸ float_ਮੰਜ਼ਿਲ_image(l.ਬਾਹਰਲਾ_w,l.ਬਾਹਰਲਾ_h,l.ਬਾਹਰਲਾ_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    ਵਾਪਸ float_ਮੰਜ਼ਿਲ_image(l.ਬਾਹਰਲਾ_w,l.ਬਾਹਰਲਾ_h,l.ਬਾਹਰਲਾ_c,l.delta);
}

ਸਥਿਰ size_t get_workspace_ਮਾਪ(layer l){
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnn_Get_Convolution_Forward_Workspace_ਮਾਪ(cudnn_handle(),
                l.srcTensorDesc,
                l.ਭਾਰ_Desc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        ਜੇ (s > most) most = s;
        cudnn_Get_Convolution_Backward_Filter_Workspace_ਮਾਪ(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        ਜੇ (s > most) most = s;
        cudnn_Get_Convolution_Backward_ਅੰਕੜੇ_Workspace_ਮਾਪ(cudnn_handle(),
                l.ਭਾਰ_Desc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        ਜੇ (s > most) most = s;
        ਵਾਪਸ most;
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ (size_t)l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w*l.ਮਾਪ*l.ਮਾਪ*l.c/l.groups*ਆਕਾਰ(ਦਸ਼ਮਲਵ);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
ਖਾਲੀ cudnn_convolutional_ਤਿਆਰੀ(layer *l)
{
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, l->batch, l->ਬਾਹਰਲਾ_c, l->ਬਾਹਰਲਾ_h, l->ਬਾਹਰਲਾ_w); 

    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, l->batch, l->ਬਾਹਰਲਾ_c, l->ਬਾਹਰਲਾ_h, l->ਬਾਹਰਲਾ_w); 
    cudnn_ਸੈੱਟ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ਅੰਕੜੇ_FLOAT, 1, l->ਬਾਹਰਲਾ_c, 1, 1); 

    cudnn_ਸੈੱਟ_Filter4d_Descriptor(l->dweightDesc, CUDNN_ਅੰਕੜੇ_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->ਮਾਪ, l->ਮਾਪ); 
    cudnn_ਸੈੱਟ_Filter4d_Descriptor(l->ਭਾਰ_Desc, CUDNN_ਅੰਕੜੇ_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->ਮਾਪ, l->ਮਾਪ); 
    #ਜੇ CUDNN_MAJOR >= 6
    cudnn_ਸੈੱਟ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_ਅੰਕੜੇ_FLOAT);
    #ਨਹੀਂਤਾਂ
    cudnn_ਸੈੱਟ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #ਜੇ_ਅੰਤ

    #ਜੇ CUDNN_MAJOR >= 7
    cudnn_ਸੈੱਟ_Convolution_ਸਮੂਹ_ਗਿਣਤੀ(l->convDesc, l->groups);
    #ਨਹੀਂਤਾਂ
    ਜੇ(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #ਜੇ_ਅੰਤ

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ਭਾਰ_Desc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_ਹੱਦਬੰਦੀ,
            2000000000,
            &l->fw_algo);
    cudnn_Get_Convolution_Backward_ਅੰਕੜੇ_Algorithm(cudnn_handle(),
            l->ਭਾਰ_Desc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_ਅੰਕੜੇ_SPECIFY_WORKSPACE_ਹੱਦਬੰਦੀ,
            2000000000,
            &l->bd_algo);
    cudnnGetConvolutionBackwardFilterAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dweightDesc,
            CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_ਹੱਦਬੰਦੀ,
            2000000000,
            &l->bf_algo);
}
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

convolutional_layer make_convolutional_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ n, ਪੂਰਨ groups, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ stride, ਪੂਰਨ padding, ACTIVATION activation, ਪੂਰਨ batch_normalize, ਪੂਰਨ ਦੁਆਧਾਰੀ, ਪੂਰਨ xnor, ਪੂਰਨ adam)
{
    ਪੂਰਨ i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.ਦੁਆਧਾਰੀ = ਦੁਆਧਾਰੀ;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.ਮਾਪ = ਮਾਪ;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c/groups*n*ਮਾਪ*ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.ਭਾਰ_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(c/groups*n*ਮਾਪ*ਮਾਪ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.biases = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.bias_updates = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.nweights = c/groups*n*ਮਾਪ*ਮਾਪ;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    ਦਸ਼ਮਲਵ scale = ਵਰਗਮੂਲ(2./(ਮਾਪ*ਮਾਪ*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    ਲਈ(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_ਸਾਧਾਰਨ();
    ਪੂਰਨ ਬਾਹਰਲਾ_w = convolutional_ਬਾਹਰਲਾ_ਚੌੜਾਈ(l);
    ਪੂਰਨ ਬਾਹਰਲਾ_h = convolutional_ਬਾਹਰਲਾ_ਉਚਾਈ(l);
    l.ਬਾਹਰਲਾ_h = ਬਾਹਰਲਾ_h;
    l.ਬਾਹਰਲਾ_w = ਬਾਹਰਲਾ_w;
    l.ਬਾਹਰਲਾ_c = n;
    l.outputs = l.ਬਾਹਰਲਾ_h * l.ਬਾਹਰਲਾ_w * l.ਬਾਹਰਲਾ_c;
    l.inputs = l.w * l.h * l.c;

    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.delta  = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.batch*l.outputs, ਆਕਾਰ(ਦਸ਼ਮਲਵ));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    ਜੇ(ਦੁਆਧਾਰੀ){
        l.ਦੁਆਧਾਰੀ_weights = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.nweights, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.cweights = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.nweights, ਆਕਾਰ(ਅੱਖਰ));
        l.scales = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }
    ਜੇ(xnor){
        l.ਦੁਆਧਾਰੀ_weights = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.nweights, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.ਦੁਆਧਾਰੀ_ਨਿਵੇਸ਼ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.inputs*l.batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }

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
        l.m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.nweights, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.nweights, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.bias_m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.scale_m = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.bias_v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        l.scale_v = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    ਜੇ(gpu_ਸੂਚਕਾਂਕ >= 0){
        ਜੇ (adam) {
            l.m_gpu = cuda_make_array(l.m, l.nweights);
            l.v_gpu = cuda_make_array(l.v, l.nweights);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }

        l.weights_gpu = cuda_make_array(l.weights, l.nweights);
        l.ਭਾਰ_updates_gpu = cuda_make_array(l.ਭਾਰ_updates, l.nweights);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*ਬਾਹਰਲਾ_h*ਬਾਹਰਲਾ_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*ਬਾਹਰਲਾ_h*ਬਾਹਰਲਾ_w*n);

        ਜੇ(ਦੁਆਧਾਰੀ){
            l.ਦੁਆਧਾਰੀ_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        ਜੇ(xnor){
            l.ਦੁਆਧਾਰੀ_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.ਦੁਆਧਾਰੀ_ਨਿਵੇਸ਼_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        ਜੇ(batch_normalize){
            l.ਔਸਤ_gpu = cuda_make_array(l.ਔਸਤ, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_ਔਸਤ_gpu = cuda_make_array(l.ਔਸਤ, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.ਔਸਤ_delta_gpu = cuda_make_array(l.ਔਸਤ, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*ਬਾਹਰਲਾ_h*ਬਾਹਰਲਾ_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*ਬਾਹਰਲਾ_h*ਬਾਹਰਲਾ_w*n);
        }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.ਭਾਰ_Desc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_ਤਿਆਰੀ(&l);
#ਜੇ_ਅੰਤ
    }
#ਜੇ_ਅੰਤ
    l.workspace_ਮਾਪ = get_workspace_ਮਾਪ(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, ਮਾਪ, ਮਾਪ, stride, w, h, c, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c, (2.0 * l.n * l.ਮਾਪ*l.ਮਾਪ*l.c/l.groups * l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w)/1000000000.);

    ਵਾਪਸ l;
}

ਖਾਲੀ denormalize_convolutional_layer(convolutional_layer l)
{
    ਪੂਰਨ i, j;
    ਲਈ(i = 0; i < l.n; ++i){
        ਦਸ਼ਮਲਵ scale = l.scales[i]/ਵਰਗਮੂਲ(l.rolling_variance[i] + .00001);
        ਲਈ(j = 0; j < l.c/l.groups*l.ਮਾਪ*l.ਮਾਪ; ++j){
            l.weights[i*l.c/l.groups*l.ਮਾਪ*l.ਮਾਪ + j] *= scale;
        }
        l.biases[i] -= l.rolling_ਔਸਤ[i] * scale;
        l.scales[i] = 1;
        l.rolling_ਔਸਤ[i] = 0;
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

ਖਾਲੀ resize_convolutional_layer(convolutional_layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    l->w = w;
    l->h = h;
    ਪੂਰਨ ਬਾਹਰਲਾ_w = convolutional_ਬਾਹਰਲਾ_ਚੌੜਾਈ(*l);
    ਪੂਰਨ ਬਾਹਰਲਾ_h = convolutional_ਬਾਹਰਲਾ_ਉਚਾਈ(*l);

    l->ਬਾਹਰਲਾ_w = ਬਾਹਰਲਾ_w;
    l->ਬਾਹਰਲਾ_h = ਬਾਹਰਲਾ_h;

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
    cudnn_convolutional_ਤਿਆਰੀ(l);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
    l->workspace_ਮਾਪ = get_workspace_ਮਾਪ(*l);
}

ਖਾਲੀ ਜੋੜੋ_bias(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ)
{
    ਪੂਰਨ i,j,b;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(i = 0; i < n; ++i){
            ਲਈ(j = 0; j < ਮਾਪ; ++j){
                output[(b*n + i)*ਮਾਪ + j] += biases[i];
            }
        }
    }
}

ਖਾਲੀ scale_bias(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *scales, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ)
{
    ਪੂਰਨ i,j,b;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(i = 0; i < n; ++i){
            ਲਈ(j = 0; j < ਮਾਪ; ++j){
                output[(b*n + i)*ਮਾਪ + j] *= scales[i];
            }
        }
    }
}

ਖਾਲੀ backward_bias(ਦਸ਼ਮਲਵ *bias_updates, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ)
{
    ਪੂਰਨ i,b;
    ਲਈ(b = 0; b < batch; ++b){
        ਲਈ(i = 0; i < n; ++i){
            bias_updates[i] += ਜੋੜ_array(delta+ਮਾਪ*(i+b*n), ਮਾਪ);
        }
    }
}

ਖਾਲੀ forward_convolutional_layer(convolutional_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ਜੇ(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.ਮਾਪ*l.ਮਾਪ, l.ਦੁਆਧਾਰੀ_weights);
        ਅਦਲਾ_ਬਦਲੀ_ਦੁਆਧਾਰੀ(&l);
        binarize_cpu(net.ਨਿਵੇਸ਼, l.c*l.h*l.w*l.batch, l.ਦੁਆਧਾਰੀ_ਨਿਵੇਸ਼);
        net.ਨਿਵੇਸ਼ = l.ਦੁਆਧਾਰੀ_ਨਿਵੇਸ਼;
    }

    ਪੂਰਨ m = l.n/l.groups;
    ਪੂਰਨ k = l.ਮਾਪ*l.ਮਾਪ*l.c/l.groups;
    ਪੂਰਨ n = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;
    ਲਈ(i = 0; i < l.batch; ++i){
        ਲਈ(j = 0; j < l.groups; ++j){
            ਦਸ਼ਮਲਵ *a = l.weights + j*l.nweights/l.groups;
            ਦਸ਼ਮਲਵ *b = net.workspace;
            ਦਸ਼ਮਲਵ *c = l.output + (i*l.groups + j)*n*m;
            ਦਸ਼ਮਲਵ *im =  net.ਨਿਵੇਸ਼ + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ਜੇ (l.ਮਾਪ == 1) {
                b = im;
            } ਨਹੀਂਤਾਂ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.ਮਾਪ, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    ਜੇ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ਨਹੀਂਤਾਂ {
        ਜੋੜੋ_bias(l.output, l.biases, l.batch, l.n, l.ਬਾਹਰਲਾ_h*l.ਬਾਹਰਲਾ_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    ਜੇ(l.ਦੁਆਧਾਰੀ || l.xnor) ਅਦਲਾ_ਬਦਲੀ_ਦੁਆਧਾਰੀ(&l);
}

ਖਾਲੀ backward_convolutional_layer(convolutional_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i, j;
    ਪੂਰਨ m = l.n/l.groups;
    ਪੂਰਨ n = l.ਮਾਪ*l.ਮਾਪ*l.c/l.groups;
    ਪੂਰਨ k = l.ਬਾਹਰਲਾ_w*l.ਬਾਹਰਲਾ_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ਜੇ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ਨਹੀਂਤਾਂ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    ਲਈ(i = 0; i < l.batch; ++i){
        ਲਈ(j = 0; j < l.groups; ++j){
            ਦਸ਼ਮਲਵ *a = l.delta + (i*l.groups + j)*m*k;
            ਦਸ਼ਮਲਵ *b = net.workspace;
            ਦਸ਼ਮਲਵ *c = l.ਭਾਰ_updates + j*l.nweights/l.groups;

            ਦਸ਼ਮਲਵ *im  = net.ਨਿਵੇਸ਼ + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            ਦਸ਼ਮਲਵ *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ਜੇ(l.ਮਾਪ == 1){
                b = im;
            } ਨਹੀਂਤਾਂ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.ਮਾਪ, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            ਜੇ (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                ਜੇ (l.ਮਾਪ == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                ਜੇ (l.ਮਾਪ != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.ਮਾਪ, l.stride, l.pad, imd);
                }
            }
        }
    }
}

ਖਾਲੀ update_convolutional_layer(convolutional_layer l, update_ਦਲੀਲਾਂ a)
{
    ਦਸ਼ਮਲਵ learning_ਦਰ = a.learning_ਦਰ*l.learning_ਦਰ_scale;
    ਦਸ਼ਮਲਵ momentum = a.momentum;
    ਦਸ਼ਮਲਵ decay = a.decay;
    ਪੂਰਨ batch = a.batch;

    axpy_cpu(l.n, learning_ਦਰ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ਜੇ(l.scales){
        axpy_cpu(l.n, learning_ਦਰ/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.ਭਾਰ_updates, 1);
    axpy_cpu(l.nweights, learning_ਦਰ/batch, l.ਭਾਰ_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.ਭਾਰ_updates, 1);
}


image get_convolutional_ਭਾਰ(convolutional_layer l, ਪੂਰਨ i)
{
    ਪੂਰਨ h = l.ਮਾਪ;
    ਪੂਰਨ w = l.ਮਾਪ;
    ਪੂਰਨ c = l.c/l.groups;
    ਵਾਪਸ float_ਮੰਜ਼ਿਲ_image(w,h,c,l.weights+i*h*w*c);
}

ਖਾਲੀ rgbgr_weights(convolutional_layer l)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < l.n; ++i){
        image im = get_convolutional_ਭਾਰ(l, i);
        ਜੇ (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

ਖਾਲੀ rescale_weights(convolutional_layer l, ਦਸ਼ਮਲਵ scale, ਦਸ਼ਮਲਵ trans)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < l.n; ++i){
        image im = get_convolutional_ਭਾਰ(l, i);
        ਜੇ (im.c == 3) {
            scale_image(im, scale);
            ਦਸ਼ਮਲਵ ਜੋੜ = ਜੋੜ_array(im.ਅੰਕੜੇ, im.w*im.h*im.c);
            l.biases[i] += ਜੋੜ*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.n, ਆਕਾਰ(image));
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_ਭਾਰ(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    ਵਾਪਸ weights;
}

image *visualize_convolutional_layer(convolutional_layer l, ਅੱਖਰ *ਖਿੜਕੀ, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, ਖਿੜਕੀ);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    ਅੱਖਰ buff[256];
    sprintf(buff, "%s: Output", ਖਿੜਕੀ);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    ਵਾਪਸ single_weights;
}

