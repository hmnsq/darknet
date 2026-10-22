#ಅನ್ತರ್ಭಾವ "deconvolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "convolutional_layer.h"
#ಅನ್ತರ್ಭಾವ "batchnorm_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "im2col.h"
#ಅನ್ತರ್ಭಾವ "col2im.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "gemm.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <time.h>


ಸ್ಥಿರ size_t get_workspace_ಗಾತ್ರ(layer l){
    ಹಿಂತಿರುಗಿ (size_t)l.h*l.w*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.n*ಗಾತ್ರ(ದಶಮಾಂಶ);
}

ಶೂನ್ಯ bilinear_init(layer l)
{
    ಪೂರ್ಣಾಂಕ i,j,f;
    ದಶಮಾಂಶ center = (l.ಗಾತ್ರ_2-1) / 2.;
    ಗಾಗಿ(f = 0; f < l.n; ++f){
        ಗಾಗಿ(j = 0; j < l.ಗಾತ್ರ_2; ++j){
            ಗಾಗಿ(i = 0; i < l.ಗಾತ್ರ_2; ++i){
                ದಶಮಾಂಶ val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                ಪೂರ್ಣಾಂಕ c = f%l.c;
                ಪೂರ್ಣಾಂಕ ind = f*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c + c*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2 + j*l.ಗಾತ್ರ_2 + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ padding, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam)
{
    ಪೂರ್ಣಾಂಕ i;
    layer l = {0};
    l.type = DECONVOLUTIONAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.ಗಾತ್ರ_2 = ಗಾತ್ರ_2;

    l.nweights = c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2;
    l.nbiases = n;

    l.weights = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.ತೂಕ_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.biases = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.bias_updates = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    ದಶಮಾಂಶ scale = .02;
    ಗಾಗಿ(i = 0; i < c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2; ++i) l.weights[i] = scale*rand_ಸಾಮಾನ್ಯ();
    //bilinear_init(l);
    ಗಾಗಿ(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.ಹೊರಹೋಗು_h = (l.h - 1) * l.stride + l.ಗಾತ್ರ_2 - 2*l.pad;
    l.ಹೊರಹೋಗು_w = (l.w - 1) * l.stride + l.ಗಾತ್ರ_2 - 2*l.pad;
    l.ಹೊರಹೋಗು_c = n;
    l.outputs = l.ಹೊರಹೋಗು_w * l.ಹೊರಹೋಗು_h * l.ಹೊರಹೋಗು_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (ದಶಮಾಂಶ)l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h/(l.w*l.h), l.weights, 1);

    l.output = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.delta  = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(l.batch*l.outputs, ಗಾತ್ರ(ದಶಮಾಂಶ));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

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
        l.m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.bias_m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.scale_m = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.bias_v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
        l.scale_v = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ));
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    ಆದರೆ(gpu_ಸೂಚ್ಯಂಕ >= 0){

        ಆದರೆ (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2);
            l.v_gpu = cuda_make_array(l.v, c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2);
        l.ತೂಕ_updates_gpu = cuda_make_array(l.ತೂಕ_updates, c*n*ಗಾತ್ರ_2*ಗಾತ್ರ_2);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w*n);

        ಆದರೆ(batch_normalize){
            l.ಸರಾಸರಿ_gpu = cuda_make_array(0, n);
            l.variance_gpu = cuda_make_array(0, n);

            l.rolling_ಸರಾಸರಿ_gpu = cuda_make_array(0, n);
            l.rolling_variance_gpu = cuda_make_array(0, n);

            l.ಸರಾಸರಿ_delta_gpu = cuda_make_array(0, n);
            l.variance_delta_gpu = cuda_make_array(0, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(0, n);

            l.x_gpu = cuda_make_array(0, l.batch*l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w*n);
            l.x_norm_gpu = cuda_make_array(0, l.batch*l.ಹೊರಹೋಗು_h*l.ಹೊರಹೋಗು_w*n);
        }
    }
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnn_ಗಣ_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, l.batch, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, 1, l.ಹೊರಹೋಗು_c, 1, 1); 
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

    l.activation = activation;
    l.workspace_ಗಾತ್ರ = get_workspace_ಗಾತ್ರ(l);

    fprintf(stderr, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, ಗಾತ್ರ_2, ಗಾತ್ರ_2, stride, w, h, c, l.ಹೊರಹೋಗು_w, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_c);

    ಹಿಂತಿರುಗಿ l;
}

ಶೂನ್ಯ denormalize_deconvolutional_layer(layer l)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಗಾಗಿ(i = 0; i < l.n; ++i){
        ದಶಮಾಂಶ scale = l.scales[i]/ವರ್ಗಮೂಲ(l.rolling_variance[i] + .00001);
        ಗಾಗಿ(j = 0; j < l.c*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2; ++j){
            l.weights[i*l.c*l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2 + j] *= scale;
        }
        l.biases[i] -= l.rolling_ಸರಾಸರಿ[i] * scale;
        l.scales[i] = 1;
        l.rolling_ಸರಾಸರಿ[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

ಶೂನ್ಯ resize_deconvolutional_layer(layer *l, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w)
{
    l->h = h;
    l->w = w;
    l->ಹೊರಹೋಗು_h = (l->h - 1) * l->stride + l->ಗಾತ್ರ_2 - 2*l->pad;
    l->ಹೊರಹೋಗು_w = (l->w - 1) * l->stride + l->ಗಾತ್ರ_2 - 2*l->pad;

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
        cudnn_ಗಣ_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, l->batch, l->ಹೊರಹೋಗು_c, l->ಹೊರಹೋಗು_h, l->ಹೊರಹೋಗು_w); 
        cudnn_ಗಣ_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_ದತ್ತಾಂಶ_FLOAT, 1, l->ಹೊರಹೋಗು_c, 1, 1); 
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
    l->workspace_ಗಾತ್ರ = get_workspace_ಗಾತ್ರ(*l);
}

ಶೂನ್ಯ forward_deconvolutional_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i;

    ಪೂರ್ಣಾಂಕ m = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.n;
    ಪೂರ್ಣಾಂಕ n = l.h*l.w;
    ಪೂರ್ಣಾಂಕ k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        ದಶಮಾಂಶ *a = l.weights;
        ದಶಮಾಂಶ *b = net.ಒಳಹರಿವು + i*l.c*l.h*l.w;
        ದಶಮಾಂಶ *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_w, l.ಗಾತ್ರ_2, l.stride, l.pad, l.output+i*l.outputs);
    }
    ಆದರೆ (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } ಇಲ್ಲದಿದ್ದರೆ {
        ಕೂಡಿಸು_bias(l.output, l.biases, l.batch, l.n, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h);
    }
    activate_array(l.output, l.batch*l.n*l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h, l.activation);
}

ಶೂನ್ಯ backward_deconvolutional_layer(layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ಆದರೆ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } ಇಲ್ಲದಿದ್ದರೆ {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.ಹೊರಹೋಗು_w*l.ಹೊರಹೋಗು_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    ಗಾಗಿ(i = 0; i < l.batch; ++i){
        ಪೂರ್ಣಾಂಕ m = l.c;
        ಪೂರ್ಣಾಂಕ n = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.n;
        ಪೂರ್ಣಾಂಕ k = l.h*l.w;

        ದಶಮಾಂಶ *a = net.ಒಳಹರಿವು + i*m*k;
        ದಶಮಾಂಶ *b = net.workspace;
        ದಶಮಾಂಶ *c = l.ತೂಕ_updates;

        im2col_cpu(l.delta + i*l.outputs, l.ಹೊರಹೋಗು_c, l.ಹೊರಹೋಗು_h, l.ಹೊರಹೋಗು_w, 
                l.ಗಾತ್ರ_2, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        ಆದರೆ(net.delta){
            ಪೂರ್ಣಾಂಕ m = l.c;
            ಪೂರ್ಣಾಂಕ n = l.h*l.w;
            ಪೂರ್ಣಾಂಕ k = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.n;

            ದಶಮಾಂಶ *a = l.weights;
            ದಶಮಾಂಶ *b = net.workspace;
            ದಶಮಾಂಶ *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

ಶೂನ್ಯ update_deconvolutional_layer(layer l, update_ವಾದಗಳು a)
{
    ದಶಮಾಂಶ learning_ದರ = a.learning_ದರ*l.learning_ದರ_scale;
    ದಶಮಾಂಶ momentum = a.momentum;
    ದಶಮಾಂಶ decay = a.decay;
    ಪೂರ್ಣಾಂಕ batch = a.batch;

    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = l.ಗಾತ್ರ_2*l.ಗಾತ್ರ_2*l.c*l.n;
    axpy_cpu(l.n, learning_ದರ/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ಆದರೆ(l.scales){
        axpy_cpu(l.n, learning_ದರ/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(ಗಾತ್ರ_2, -decay*batch, l.weights, 1, l.ತೂಕ_updates, 1);
    axpy_cpu(ಗಾತ್ರ_2, learning_ದರ/batch, l.ತೂಕ_updates, 1, l.weights, 1);
    scal_cpu(ಗಾತ್ರ_2, momentum, l.ತೂಕ_updates, 1);
}



