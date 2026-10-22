#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "batchnorm_layer.h"
#ಅನ್ತರ್ಭಾವ "im2col.h"
#ಅನ್ತರ್ಭಾವ "col2im.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ AI2
#ಅನ್ತರ್ಭಾವ "xnor_layer.h"
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ಅದಲುಬದಲು_ದ್ವಿಮಾನ(convolutional_layer *l)
{
    ದಶಮಾಂಶ *ಅದಲುಬದಲು = l->weights;
    l->weights = l->ದ್ವಿಮಾನ_weights;
    l->ದ್ವಿಮಾನ_weights = ಅದಲುಬದಲು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    ಅದಲುಬದಲು = l->weights_gpu;
    l->weights_gpu = l->ದ್ವಿಮಾನ_weights_gpu;
    l->ದ್ವಿಮಾನ_weights_gpu = ಅದಲುಬದಲು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ binarize_weights(ದಶಮಾಂಶ *weights, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ *ದ್ವಿಮಾನ)
{
    ಪೂರ್ಣಾಂಕ i, f;
    ಗಾಗಿ(f = 0; f < n; ++f){
        ದಶಮಾಂಶ ಸರಾಸರಿ = 0;
        ಗಾಗಿ(i = 0; i < ಗಾತ್ರ_2; ++i){
            ಸರಾಸರಿ += fabs(weights[f*ಗಾತ್ರ_2 + i]);
        }
        ಸರಾಸರಿ = ಸರಾಸರಿ / ಗಾತ್ರ_2;
        ಗಾಗಿ(i = 0; i < ಗಾತ್ರ_2; ++i){
            ದ್ವಿಮಾನ[f*ಗಾತ್ರ_2 + i] = (weights[f*ಗಾತ್ರ_2 + i] > 0) ? ಸರಾಸರಿ : -ಸರಾಸರಿ;
        }
    }
}

ಶೂನ್ಯ binarize_cpu(ದಶಮಾಂಶ *ಒಳಹರಿವು, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *ದ್ವಿಮಾನ)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದ್ವಿಮಾನ[i] = (ಒಳಹರಿವು[i] > 0) ? 1 : -1;
    }
}

ಶೂನ್ಯ binarize_ಒಳಹರಿವು(ದಶಮಾಂಶ *ಒಳಹರಿವು, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ *ದ್ವಿಮಾನ)
{
    ಪೂರ್ಣಾಂಕ i, s;
    ಗಾಗಿ(s = 0; s < ಗಾತ್ರ_2; ++s){
        ದಶಮಾಂಶ ಸರಾಸರಿ = 0;
        ಗಾಗಿ(i = 0; i < n; ++i){
            ಸರಾಸರಿ += fabs(ಒಳಹರಿವು[i*ಗಾತ್ರ_2 + s]);
        }
        ಸರಾಸರಿ = ಸರಾಸರಿ / n;
        ಗಾಗಿ(i = 0; i < n; ++i){
            ದ್ವಿಮಾನ[i*ಗಾತ್ರ_2 + s] = (ಒಳಹರಿವು[i*ಗಾತ್ರ_2 + s] > 0) ? ಸರಾಸರಿ : -ಸರಾಸರಿ;
        }
    }
}

ಪೂರ್ಣಾಂಕ convolutional_ಹೊರಹೋಗು_ಎತ್ತರ(convolutional_layer l)
{
    ಹಿಂತಿರುಗಿ (l.h + 2*l.pad - l.ಗಾತ್ರ_2) / l.stride + 1;
}

ಪೂರ್ಣಾಂಕ convolutional_ಹೊರಹೋಗು_ಅಗಲ(convolutional_layer l)
{
    ಹಿಂತಿರುಗಿ (l.w + 2*l.pad - l.ಗಾತ್ರ_2) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    ಹಿಂತಿರುಗಿ float_ಗಮ್ಯ_image(l.ಹೊರಹೋಗು_w,l.ಹೊರಹೋಗು_h,l.ಹೊರಹೋಗು_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    ಹಿಂತಿರುಗಿ float_ಗಮ್ಯ_image(l.ಹೊರಹೋಗು_w,l.ಹೊರಹೋಗು_h,l.ಹೊರಹೋಗು_c,l.delta);
}

ಸ್ಥಿರ size_t get_workspace_ಗಾತ್ರ(layer l){
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnn_Get_Convolution_Forward_Workspace_ಗಾತ್ರ(cudnn_handle(),
                l.srcTensorDesc,
                l.ತೂಕ_Desc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        ಆದರೆ (s > most) most = s;
        cudnn_Get_Convolution_Backward_Filter_Workspace_ಗಾತ್ರ(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        ಆದರೆ (s > most) most = s;
        cudnn_Get_Convolution_Backward_ದತ್ತಾಂಶ_Workspace_ಗಾತ್ರ(cudnn_handle(),
                l.ತೂಕ_Desc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        ಆದರೆ (s > most) most = s;
        ಹಿಂತಿರುಗಿ most;
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ (size_t)l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c/l.groups*ಗಾತ್ರ(ದಶಮಾಂಶ);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
ಶೂನ್ಯ cudnn_convolutional_ಸಿದ್ಧತೆ(layer *l)
{
    cudnn_ಗಣ_Tensor4d_Descriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, l->batch, l->ಹೊರಹೋಗು_c, l->ಹೊರಹೋಗು_h, l->ಹೊರಹೋಗು_w); 

    cudnn_ಗಣ_Tensor4d_Descriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, l->batch, l->ಹೊರಹೋಗು_c, l->ಹೊರಹೋಗು_h, l->ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, 1, l->ಹೊರಹೋಗು_c, 1, 1); 

    cudnn_ಗಣ_Filter4d_Descriptor(l->dweightDesc, CUDNN_ದತ್ತಾಂಶ_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->ಗಾತ್ರ_2, l->ಗಾತ್ರ_2); 
    cudnn_ಗಣ_Filter4d_Descriptor(l->ತೂಕ_Desc, CUDNN_ದತ್ತಾಂಶ_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->ಗಾತ್ರ_2, l->ಗಾತ್ರ_2); 
    #ಆದರೆ CUDNN_MAJOR >= 6
    cudnn_ಗಣ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_ದತ್ತಾಂಶ_FLOAT);
    #ಇಲ್ಲದಿದ್ದರೆ
    cudnn_ಗಣ_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ CUDNN_MAJOR >= 7
    cudnn_ಗಣ_Convolution_ಗುಂಪು_ಎಣಿಕೆ(l->convDesc, l->groups);
    #ಇಲ್ಲದಿದ್ದರೆ
    ಆದರೆ(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #ಆದರೆ_ಅಂತ್ಯ

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ತೂಕ_Desc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_ಎಲ್ಲೆ,
            2000000000,
            &l->fw_algo);
    cudnn_Get_Convolution_Backward_ದತ್ತಾಂಶ_Algorithm(cudnn_handle(),
            l->ತೂಕ_Desc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_ದತ್ತಾಂಶ_SPECIFY_WORKSPACE_ಎಲ್ಲೆ,
            2000000000,
            &l->bd_algo);
    cudnnGetConvolutionBackwardFilterAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dweightDesc,
            CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_ಎಲ್ಲೆ,
            2000000000,
            &l->bf_algo);
}
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

convolutional_layer make_convolutional_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ groups, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ padding, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ, ಪೂರ್ಣಾಂಕ xnor, ಪೂರ್ಣಾಂಕ adam)
{
    ಪೂರ್ಣಾಂಕ i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.ದ್ವಿಮಾನ = ದ್ವಿಮಾನ;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.ಗಾತ್ರ_2 = ಗಾತ್ರ_2;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c/groups*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.ತೂಕ_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c/groups*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.biases = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.bias_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.nweights = c/groups*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    ದಶಮಾಂಶ scale = ವರ್ಗಮೂಲ(2./(ಗಾತ್ರ_2*ಗಾತ್ರ_2*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    ಗಾಗಿ(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_ಸಾಮಾನ್ಯ();
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_w = convolutional_ಹೊರಹೋಗು_ಅಗಲ(l);
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_h = convolutional_ಹೊರಹೋಗು_ಎತ್ತರ(l);
    l.ಹೊರಹೋಗು_h = ಹೊರಹೋಗು_h;
    l.ಹೊರಹೋಗು_w = ಹೊರಹೋಗು_w;
    l.ಹೊರಹೋಗು_c = n;
    l.outputs = l.ಹೊರಹೋಗು_h * l.ಹೊರಹೋಗು_w * l.ಹೊರಹೋಗು_c;
    l.inputs = l.w * l.h * l.c;

    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta  = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    ಆದರೆ(ದ್ವಿಮಾನ){
        l.ದ್ವಿಮಾನ_weights = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.nweights, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.cweights = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.nweights, ಗಾತ್ರ(ಅಕ್ಷರ));
        l.scales = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
    }
    ಆದರೆ(xnor){
        l.ದ್ವಿಮಾನ_weights = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.nweights, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.ದ್ವಿಮಾನ_ಒಳಹರಿವು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.inputs*l.batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    }

    ಆದರೆ(batch_normalize){
        l.scales = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.scale_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಗಾಗಿ(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.ಸರಾಸರಿ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.variance = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));

        l.ಸರಾಸರಿ_delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.variance_delta = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));

        l.rolling_ಸರಾಸರಿ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.rolling_variance = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.x = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.x_norm = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    }
    ಆದರೆ(adam){
        l.m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.nweights, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.nweights, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.bias_m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.scale_m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.bias_v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.scale_v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){
        ಆದರೆ (adam) {
            l.m_gpu = cuda_make_array(l.m, l.nweights);
            l.v_gpu = cuda_make_array(l.v, l.nweights);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }

        l.weights_gpu = cuda_make_array(l.weights, l.nweights);
        l.ತೂಕ_updates_gpu = cuda_make_array(l.ತೂಕ_updates, l.nweights);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*ಹೊರಹೋಗು_h*ಹೊರಹೋಗು_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*ಹೊರಹೋಗು_h*ಹೊರಹೋಗು_w*n);

        ಆದರೆ(ದ್ವಿಮಾನ){
            l.ದ್ವಿಮಾನ_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        ಆದರೆ(xnor){
            l.ದ್ವಿಮಾನ_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.ದ್ವಿಮಾನ_ಒಳಹರಿವು_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        ಆದರೆ(batch_normalize){
            l.ಸರಾಸರಿ_gpu = cuda_make_array(l.ಸರಾಸರಿ, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_ಸರಾಸರಿ_gpu = cuda_make_array(l.ಸರಾಸರಿ, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.ಸರಾಸರಿ_delta_gpu = cuda_make_array(l.ಸರಾಸರಿ, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*ಹೊರಹೋಗು_h*ಹೊರಹೋಗು_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*ಹೊರಹೋಗು_h*ಹೊರಹೋಗು_w*n);
        }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.ತೂಕ_Desc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_ಸಿದ್ಧತೆ(&l);
#ಆದರೆ_ಅಂತ್ಯ
    }
#ಆದರೆ_ಅಂತ್ಯ
    l.workspace_ಗಾತ್ರ = get_workspace_ಗಾತ್ರ(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, ಗಾತ್ರ_2, ಗಾತ್ರ_2, stride, w, h, c, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c, (2.0 * l.n * l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c/l.groups * l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w)/1000000000.);

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ denormalize_convolutional_layer(convolutional_layer l)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        ದಶಮಾಂಶ scale = l.scales[i]/ವರ್ಗಮೂಲ(l.rolling_variance[i] + .00001);
        ಗಾಗಿ(j = 0; j < l.c/l.groups*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2; ++j){
            l.weights[i*l.c/l.groups*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2 + j] *= scale;
        }
        l.biases[i] -= l.rolling_ಸರಾಸರಿ[i] * scale;
        l.scales[i] = 1;
        l.rolling_ಸರಾಸರಿ[i] = 0;
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

ಶೂನ್ಯ resize_convolutional_layer(convolutional_layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    l->w = w;
    l->h = h;
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_w = convolutional_ಹೊರಹೋಗು_ಅಗಲ(*l);
    ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_h = convolutional_ಹೊರಹೋಗು_ಎತ್ತರ(*l);

    l->ಹೊರಹೋಗು_w = ಹೊರಹೋಗು_w;
    l->ಹೊರಹೋಗು_h = ಹೊರಹೋಗು_h;

    l->outputs = l->ಹೊರಹೋಗು_h * l->ಹೊರಹೋಗು_w * l->ಹೊರಹೋಗು_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*ಗಾತ್ರ(ದಶಮಾಂಶ));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಆದರೆ(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*ಗಾತ್ರ(ದಶಮಾಂಶ));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*ಗಾತ್ರ(ದಶಮಾಂಶ));
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    ಆದರೆ(l->batch_normalize){
        cuda_free(l->x_gpu);
        cuda_free(l->x_norm_gpu);

        l->x_gpu = cuda_make_array(l->output, l->batch*l->outputs);
        l->x_norm_gpu = cuda_make_array(l->output, l->batch*l->outputs);
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
    cudnn_convolutional_ಸಿದ್ಧತೆ(l);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
    l->workspace_ಗಾತ್ರ = get_workspace_ಗಾತ್ರ(*l);
}

ಶೂನ್ಯ ಕೂಡಿಸು_bias(ದಶಮಾಂಶ *output, ದಶಮಾಂಶ *biases, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    ಪೂರ್ಣಾಂಕ i,j,b;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(i = 0; i < n; ++i){
            ಗಾಗಿ(j = 0; j < ಗಾತ್ರ_2; ++j){
                output[(b*n + i)*ಗಾತ್ರ_2 + j] += biases[i];
            }
        }
    }
}

ಶೂನ್ಯ scale_bias(ದಶಮಾಂಶ *output, ದಶಮಾಂಶ *scales, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    ಪೂರ್ಣಾಂಕ i,j,b;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(i = 0; i < n; ++i){
            ಗಾಗಿ(j = 0; j < ಗಾತ್ರ_2; ++j){
                output[(b*n + i)*ಗಾತ್ರ_2 + j] *= scales[i];
            }
        }
    }
}

ಶೂನ್ಯ backward_bias(ದಶಮಾಂಶ *bias_updates, ದಶಮಾಂಶ *delta, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    ಪೂರ್ಣಾಂಕ i,b;
    ಗಾಗಿ(b = 0; b < batch; ++b){
        ಗಾಗಿ(i = 0; i < n; ++i){
            bias_updates[i] += ಮೊತ್ತ_array(delta+ಗಾತ್ರ_2*(i+b*n), ಗಾತ್ರ_2);
        }
    }
}

ಶೂನ್ಯ forward_convolutional_layer(convolutional_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ಆದರೆ(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2, l.ದ್ವಿಮಾನ_weights);
        ಅದಲುಬದಲು_ದ್ವಿಮಾನ(&l);
        binarize_cpu(net.ಒಳಹರಿವು, l.c*l.h*l.w*l.batch, l.ದ್ವಿಮಾನ_ಒಳಹರಿವು);
        net.ಒಳಹರಿವು = l.ದ್ವಿಮಾನ_ಒಳಹರಿವು;
    }

    ಪೂರ್ಣಾಂಕ m = l.n/l.groups;
    ಪೂರ್ಣಾಂಕ k = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c/l.groups;
    ಪೂರ್ಣಾಂಕ n = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;
    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        ಗಾಗಿ(j = 0; j < l.groups; ++j){
            ದಶಮಾಂಶ *a = l.weights + j*l.nweights/l.groups;
            ದಶಮಾಂಶ *b = net.workspace;
            ದಶಮಾಂಶ *c = l.output + (i*l.groups + j)*n*m;
            ದಶಮಾಂಶ *im =  net.ಒಳಹರಿವು + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ಆದರೆ (l.ಗಾತ್ರ_2 == 1) {
                b = im;
            } ಇಲ್ಲದಿದ್ದರೆ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.ಗಾತ್ರ_2, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    ಆದರೆ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಕೂಡಿಸು_bias(l.output, l.biases, l.batch, l.n, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    ಆದರೆ(l.ದ್ವಿಮಾನ || l.xnor) ಅದಲುಬದಲು_ದ್ವಿಮಾನ(&l);
}

ಶೂನ್ಯ backward_convolutional_layer(convolutional_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಪೂರ್ಣಾಂಕ m = l.n/l.groups;
    ಪೂರ್ಣಾಂಕ n = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c/l.groups;
    ಪೂರ್ಣಾಂಕ k = l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ಆದರೆ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ಇಲ್ಲದಿದ್ದರೆ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        ಗಾಗಿ(j = 0; j < l.groups; ++j){
            ದಶಮಾಂಶ *a = l.delta + (i*l.groups + j)*m*k;
            ದಶಮಾಂಶ *b = net.workspace;
            ದಶಮಾಂಶ *c = l.ತೂಕ_updates + j*l.nweights/l.groups;

            ದಶಮಾಂಶ *im  = net.ಒಳಹರಿವು + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            ದಶಮಾಂಶ *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ಆದರೆ(l.ಗಾತ್ರ_2 == 1){
                b = im;
            } ಇಲ್ಲದಿದ್ದರೆ {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.ಗಾತ್ರ_2, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            ಆದರೆ (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                ಆದರೆ (l.ಗಾತ್ರ_2 == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                ಆದರೆ (l.ಗಾತ್ರ_2 != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.ಗಾತ್ರ_2, l.stride, l.pad, imd);
                }
            }
        }
    }
}

ಶೂನ್ಯ update_convolutional_layer(convolutional_layer l, update_ವಾದಗಳು a)
{
    ದಶಮಾಂಶ learning_ದರ = a.learning_ದರ*l.learning_ದರ_scale;
    ದಶಮಾಂಶ momentum = a.momentum;
    ದಶಮಾಂಶ decay = a.decay;
    ಪೂರ್ಣಾಂಕ batch = a.batch;

    axpy_cpu(l.n, learning_ದರ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ಆದರೆ(l.scales){
        axpy_cpu(l.n, learning_ದರ/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.ತೂಕ_updates, 1);
    axpy_cpu(l.nweights, learning_ದರ/batch, l.ತೂಕ_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.ತೂಕ_updates, 1);
}


image get_convolutional_ತೂಕ(convolutional_layer l, ಪೂರ್ಣಾಂಕ i)
{
    ಪೂರ್ಣಾಂಕ h = l.ಗಾತ್ರ_2;
    ಪೂರ್ಣಾಂಕ w = l.ಗಾತ್ರ_2;
    ಪೂರ್ಣಾಂಕ c = l.c/l.groups;
    ಹಿಂತಿರುಗಿ float_ಗಮ್ಯ_image(w,h,c,l.weights+i*h*w*c);
}

ಶೂನ್ಯ rgbgr_weights(convolutional_layer l)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        image im = get_convolutional_ತೂಕ(l, i);
        ಆದರೆ (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

ಶೂನ್ಯ rescale_weights(convolutional_layer l, ದಶಮಾಂಶ scale, ದಶಮಾಂಶ trans)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        image im = get_convolutional_ತೂಕ(l, i);
        ಆದರೆ (im.c == 3) {
            scale_image(im, scale);
            ದಶಮಾಂಶ ಮೊತ್ತ = ಮೊತ್ತ_array(im.ದತ್ತಾಂಶ, im.w*im.h*im.c);
            l.biases[i] += ಮೊತ್ತ*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.n, ಗಾತ್ರ(image));
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_ತೂಕ(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    ಹಿಂತಿರುಗಿ weights;
}

image *visualize_convolutional_layer(convolutional_layer l, ಅಕ್ಷರ *ಕಿಟಕಿ, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, ಕಿಟಕಿ);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    ಅಕ್ಷರ buff[256];
    sprintf(buff, "%s: Output", ಕಿಟಕಿ);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    ಹಿಂತಿರುಗಿ single_weights;
}

