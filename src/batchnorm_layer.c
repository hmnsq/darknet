#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "batchnorm_layer.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

layer make_batchnorm_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c)
{
    fprintf(stderr, "Batch Normalization Layer: %d x %d x %d image\n", w,h,c);
    layer l = {0};
    l.type = BATCHNORM;
    l.batch = batch;
    l.h = l.ಹೊರಹೋಗು_h = h;
    l.w = l.ಹೊರಹೋಗು_w = w;
    l.c = l.ಹೊರಹೋಗು_c = c;
    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(h * w * c * batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta  = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(h * w * c * batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.inputs = w*h*c;
    l.outputs = l.inputs;

    l.scales = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.scale_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.biases = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.bias_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < c; ++i){
        l.scales[i] = 1;
    }

    l.ಸರಾಸರಿ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.variance = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.rolling_ಸರಾಸರಿ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.rolling_variance = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_batchnorm_layer;
    l.backward = backward_batchnorm_layer;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_batchnorm_layer_gpu;
    l.backward_gpu = backward_batchnorm_layer_gpu;

    l.output_gpu =  cuda_make_array(l.output, h * w * c * batch);
    l.delta_gpu =   cuda_make_array(l.delta, h * w * c * batch);

    l.biases_gpu = cuda_make_array(l.biases, c);
    l.bias_updates_gpu = cuda_make_array(l.bias_updates, c);

    l.scales_gpu = cuda_make_array(l.scales, c);
    l.scale_updates_gpu = cuda_make_array(l.scale_updates, c);

    l.ಸರಾಸರಿ_gpu = cuda_make_array(l.ಸರಾಸರಿ, c);
    l.variance_gpu = cuda_make_array(l.variance, c);

    l.rolling_ಸರಾಸರಿ_gpu = cuda_make_array(l.ಸರಾಸರಿ, c);
    l.rolling_variance_gpu = cuda_make_array(l.variance, c);

    l.ಸರಾಸರಿ_delta_gpu = cuda_make_array(l.ಸರಾಸರಿ, c);
    l.variance_delta_gpu = cuda_make_array(l.variance, c);

    l.x_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    l.x_norm_gpu = cuda_make_array(l.output, l.batch*l.outputs);
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
    cudnnCreateTensorDescriptor(&l.normTensorDesc);
    cudnnCreateTensorDescriptor(&l.dstTensorDesc);
    cudnn_ಗಣ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_w); 
    cudnn_ಗಣ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, 1, l.ಹೊರಹೋಗು_c, 1, 1); 

    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ backward_scale_cpu(ದಶಮಾಂಶ *x_norm, ದಶಮಾಂಶ *delta, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ *scale_updates)
{
    ಪೂರ್ಣಾಂಕ i,b,f;
    ಗಾಗಿ(f = 0; f < n; ++f){
        ದಶಮಾಂಶ ಮೊತ್ತ = 0;
        ಗಾಗಿ(b = 0; b < batch; ++b){
            ಗಾಗಿ(i = 0; i < ಗಾತ್ರ_2; ++i){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = i + ಗಾತ್ರ_2*(f + n*b);
                ಮೊತ್ತ += delta[ಸೂಚ್ಯಂಕ] * x_norm[ಸೂಚ್ಯಂಕ];
            }
        }
        scale_updates[f] += ಮೊತ್ತ;
    }
}

ಶೂನ್ಯ ಸರಾಸರಿ_delta_cpu(ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ *variance, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ filters, ಪೂರ್ಣಾಂಕ spatial, ದಶಮಾಂಶ *ಸರಾಸರಿ_delta)
{

    ಪೂರ್ಣಾಂಕ i,j,k;
    ಗಾಗಿ(i = 0; i < filters; ++i){
        ಸರಾಸರಿ_delta[i] = 0;
        ಗಾಗಿ (j = 0; j < batch; ++j) {
            ಗಾಗಿ (k = 0; k < spatial; ++k) {
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = j*filters*spatial + i*spatial + k;
                ಸರಾಸರಿ_delta[i] += delta[ಸೂಚ್ಯಂಕ];
            }
        }
        ಸರಾಸರಿ_delta[i] *= (-1./ವರ್ಗಮೂಲ(variance[i] + .00001f));
    }
}
ಶೂನ್ಯ  variance_delta_cpu(ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *delta, ದಶಮಾಂಶ *ಸರಾಸರಿ, ದಶಮಾಂಶ *variance, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ filters, ಪೂರ್ಣಾಂಕ spatial, ದಶಮಾಂಶ *variance_delta)
{

    ಪೂರ್ಣಾಂಕ i,j,k;
    ಗಾಗಿ(i = 0; i < filters; ++i){
        variance_delta[i] = 0;
        ಗಾಗಿ(j = 0; j < batch; ++j){
            ಗಾಗಿ(k = 0; k < spatial; ++k){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = j*filters*spatial + i*spatial + k;
                variance_delta[i] += delta[ಸೂಚ್ಯಂಕ]*(x[ಸೂಚ್ಯಂಕ] - ಸರಾಸರಿ[i]);
            }
        }
        variance_delta[i] *= -.5 * ಘಾತ(variance[i] + .00001f, (ದಶಮಾಂಶ)(-3./2.));
    }
}
ಶೂನ್ಯ normalize_delta_cpu(ದಶಮಾಂಶ *x, ದಶಮಾಂಶ *ಸರಾಸರಿ, ದಶಮಾಂಶ *variance, ದಶಮಾಂಶ *ಸರಾಸರಿ_delta, ದಶಮಾಂಶ *variance_delta, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ filters, ಪೂರ್ಣಾಂಕ spatial, ದಶಮಾಂಶ *delta)
{
    ಪೂರ್ಣಾಂಕ f, j, k;
    ಗಾಗಿ(j = 0; j < batch; ++j){
        ಗಾಗಿ(f = 0; f < filters; ++f){
            ಗಾಗಿ(k = 0; k < spatial; ++k){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = j*filters*spatial + f*spatial + k;
                delta[ಸೂಚ್ಯಂಕ] = delta[ಸೂಚ್ಯಂಕ] * 1./(ವರ್ಗಮೂಲ(variance[f] + .00001f)) + variance_delta[f] * 2. * (x[ಸೂಚ್ಯಂಕ] - ಸರಾಸರಿ[f]) / (spatial * batch) + ಸರಾಸರಿ_delta[f]/(spatial*batch);
            }
        }
    }
}

ಶೂನ್ಯ resize_batchnorm_layer(layer *layer, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    fprintf(stderr, "Not implemented\n");
}

ಶೂನ್ಯ forward_batchnorm_layer(layer l, ಜಾಲ net)
{
    ಆದರೆ(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.ಒಳಹರಿವು, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    ಆದರೆ(net.train){
        ಸರಾಸರಿ_cpu(l.output, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w, l.ಸರಾಸರಿ);
        variance_cpu(l.output, l.ಸರಾಸರಿ, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w, l.variance);

        scal_cpu(l.ಹೊರಹೋಗು_c, .99, l.rolling_ಸರಾಸರಿ, 1);
        axpy_cpu(l.ಹೊರಹೋಗು_c, .01, l.ಸರಾಸರಿ, 1, l.rolling_ಸರಾಸರಿ, 1);
        scal_cpu(l.ಹೊರಹೋಗು_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.ಹೊರಹೋಗು_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.ಸರಾಸರಿ, l.variance, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } ಇಲ್ಲದಿದ್ದರೆ {
        normalize_cpu(l.output, l.rolling_ಸರಾಸರಿ, l.rolling_variance, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);
    }
    scale_bias(l.output, l.scales, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);
    ಕೂಡಿಸು_bias(l.output, l.biases, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);
}

ಶೂನ್ಯ backward_batchnorm_layer(layer l, ಜಾಲ net)
{
    ಆದರೆ(!net.train){
        l.ಸರಾಸರಿ = l.rolling_ಸರಾಸರಿ;
        l.variance = l.rolling_variance;
    }
    backward_bias(l.bias_updates, l.delta, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h);
    backward_scale_cpu(l.x_norm, l.delta, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.scale_updates);

    scale_bias(l.delta, l.scales, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);

    ಸರಾಸರಿ_delta_cpu(l.delta, l.variance, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.ಸರಾಸರಿ_delta);
    variance_delta_cpu(l.x, l.delta, l.ಸರಾಸರಿ, l.variance, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.variance_delta);
    normalize_delta_cpu(l.x, l.ಸರಾಸರಿ, l.variance, l.ಸರಾಸರಿ_delta, l.variance_delta, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.delta);
    ಆದರೆ(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, l.delta, 1, net.delta, 1);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU

ಶೂನ್ಯ pull_batchnorm_layer(layer l)
{
    cuda_pull_array(l.scales_gpu, l.scales, l.c);
    cuda_pull_array(l.rolling_ಸರಾಸರಿ_gpu, l.rolling_ಸರಾಸರಿ, l.c);
    cuda_pull_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}
ಶೂನ್ಯ ತಳ್ಳು_batchnorm_layer(layer l)
{
    cuda_ತಳ್ಳು_array(l.scales_gpu, l.scales, l.c);
    cuda_ತಳ್ಳು_array(l.rolling_ಸರಾಸರಿ_gpu, l.rolling_ಸರಾಸರಿ, l.c);
    cuda_ತಳ್ಳು_array(l.rolling_variance_gpu, l.rolling_variance, l.c);
}

ಶೂನ್ಯ forward_batchnorm_layer_gpu(layer l, ಜಾಲ net)
{
    ಆದರೆ(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, net.ಒಳಹರಿವು_gpu, 1, l.output_gpu, 1);
    copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
    ಆದರೆ (net.train) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
        ದಶಮಾಂಶ one = 1;
        ದಶಮಾಂಶ zero = 0;
        cudnnBatchNormalizationForwardTraining(cudnn_handle(),
                CUDNN_BATCHNORM_SPATIAL,
                &one,
                &zero,
                l.dstTensorDesc,
                l.x_gpu,
                l.dstTensorDesc,
                l.output_gpu,
                l.normTensorDesc,
                l.scales_gpu,
                l.biases_gpu,
                .01,
                l.rolling_ಸರಾಸರಿ_gpu,
                l.rolling_variance_gpu,
                .00001,
                l.ಸರಾಸರಿ_gpu,
                l.variance_gpu);
#ಇಲ್ಲದಿದ್ದರೆ
        fast_ಸರಾಸರಿ_gpu(l.output_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w, l.ಸರಾಸರಿ_gpu);
        fast_variance_gpu(l.output_gpu, l.ಸರಾಸರಿ_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w, l.variance_gpu);

        scal_gpu(l.ಹೊರಹೋಗು_c, .99, l.rolling_ಸರಾಸರಿ_gpu, 1);
        axpy_gpu(l.ಹೊರಹೋಗು_c, .01, l.ಸರಾಸರಿ_gpu, 1, l.rolling_ಸರಾಸರಿ_gpu, 1);
        scal_gpu(l.ಹೊರಹೋಗು_c, .99, l.rolling_variance_gpu, 1);
        axpy_gpu(l.ಹೊರಹೋಗು_c, .01, l.variance_gpu, 1, l.rolling_variance_gpu, 1);

        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_gpu, 1);
        normalize_gpu(l.output_gpu, l.ಸರಾಸರಿ_gpu, l.variance_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);
        copy_gpu(l.outputs*l.batch, l.output_gpu, 1, l.x_norm_gpu, 1);

        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);
        ಕೂಡಿಸು_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h);
#ಆದರೆ_ಅಂತ್ಯ
    } ಇಲ್ಲದಿದ್ದರೆ {
        normalize_gpu(l.output_gpu, l.rolling_ಸರಾಸರಿ_gpu, l.rolling_variance_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);
        scale_bias_gpu(l.output_gpu, l.scales_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);
        ಕೂಡಿಸು_bias_gpu(l.output_gpu, l.biases_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h);
    }

}

ಶೂನ್ಯ backward_batchnorm_layer_gpu(layer l, ಜಾಲ net)
{
    ಆದರೆ(!net.train){
        l.ಸರಾಸರಿ_gpu = l.rolling_ಸರಾಸರಿ_gpu;
        l.variance_gpu = l.rolling_variance_gpu;
    }
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
    ದಶಮಾಂಶ one = 1;
    ದಶಮಾಂಶ zero = 0;
    cudnnBatchNormalizationBackward(cudnn_handle(),
            CUDNN_BATCHNORM_SPATIAL,
            &one,
            &zero,
            &one,
            &one,
            l.dstTensorDesc,
            l.x_gpu,
            l.dstTensorDesc,
            l.delta_gpu,
            l.dstTensorDesc,
            l.x_norm_gpu,
            l.normTensorDesc,
            l.scales_gpu,
            l.scale_updates_gpu,
            l.bias_updates_gpu,
            .00001,
            l.ಸರಾಸರಿ_gpu,
            l.variance_gpu);
    copy_gpu(l.outputs*l.batch, l.x_norm_gpu, 1, l.delta_gpu, 1);
#ಇಲ್ಲದಿದ್ದರೆ
    backward_bias_gpu(l.bias_updates_gpu, l.delta_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h);
    backward_scale_gpu(l.x_norm_gpu, l.delta_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.scale_updates_gpu);

    scale_bias_gpu(l.delta_gpu, l.scales_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w);

    fast_ಸರಾಸರಿ_delta_gpu(l.delta_gpu, l.variance_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.ಸರಾಸರಿ_delta_gpu);
    fast_variance_delta_gpu(l.x_gpu, l.delta_gpu, l.ಸರಾಸರಿ_gpu, l.variance_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.variance_delta_gpu);
    normalize_delta_gpu(l.x_gpu, l.ಸರಾಸರಿ_gpu, l.variance_gpu, l.ಸರಾಸರಿ_delta_gpu, l.variance_delta_gpu, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.delta_gpu);
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ(l.type == BATCHNORM) copy_gpu(l.outputs*l.batch, l.delta_gpu, 1, net.delta_gpu, 1);
}
#ಆದರೆ_ಅಂತ್ಯ
