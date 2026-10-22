#అన్తర్భావ "deconvolutional_layer.h"
#అన్తర్భావ "convolutional_layer.h"
#అన్తర్భావ "batchnorm_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "im2col.h"
#అన్తర్భావ "col2im.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "gemm.h"

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <time.h>


స్థిర size_t get_workspace_కొలత(layer l){
    తిరిగి (size_t)l.h*l.w*l.కొలత*l.కొలత*l.n*పరిమాణం(దశాంశం);
}

శూన్య bilinear_init(layer l)
{
    పూర్ణాంకం i,j,f;
    దశాంశం center = (l.కొలత-1) / 2.;
    కొరకు(f = 0; f < l.n; ++f){
        కొరకు(j = 0; j < l.కొలత; ++j){
            కొరకు(i = 0; i < l.కొలత; ++i){
                దశాంశం val = (1 - fabs(i - center)) * (1 - fabs(j - center));
                పూర్ణాంకం c = f%l.c;
                పూర్ణాంకం ind = f*l.కొలత*l.కొలత*l.c + c*l.కొలత*l.కొలత + j*l.కొలత + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం n, పూర్ణాంకం కొలత, పూర్ణాంకం stride, పూర్ణాంకం padding, ACTIVATION activation, పూర్ణాంకం batch_normalize, పూర్ణాంకం adam)
{
    పూర్ణాంకం i;
    layer l = {0};
    l.type = DECONVOLUTIONAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.కొలత = కొలత;

    l.nweights = c*n*కొలత*కొలత;
    l.nbiases = n;

    l.weights = సున్నా_జ్ఞాపకం_ఇవ్వు(c*n*కొలత*కొలత, పరిమాణం(దశాంశం));
    l.బరువు_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(c*n*కొలత*కొలత, పరిమాణం(దశాంశం));

    l.biases = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
    l.bias_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    దశాంశం scale = .02;
    కొరకు(i = 0; i < c*n*కొలత*కొలత; ++i) l.weights[i] = scale*rand_సాధారణ();
    //bilinear_init(l);
    కొరకు(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.బహిర్గతం_h = (l.h - 1) * l.stride + l.కొలత - 2*l.pad;
    l.బహిర్గతం_w = (l.w - 1) * l.stride + l.కొలత - 2*l.pad;
    l.బహిర్గతం_c = n;
    l.outputs = l.బహిర్గతం_w * l.బహిర్గతం_h * l.బహిర్గతం_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (దశాంశం)l.బహిర్గతం_w*l.బహిర్గతం_h/(l.w*l.h), l.weights, 1);

    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*l.outputs, పరిమాణం(దశాంశం));
    l.delta  = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*l.outputs, పరిమాణం(దశాంశం));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    ఒకవేళ(batch_normalize){
        l.scales = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.scale_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        కొరకు(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.సగటు = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.variance = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));

        l.సగటు_delta = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.variance_delta = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));

        l.rolling_సగటు = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.rolling_variance = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.x = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*l.outputs, పరిమాణం(దశాంశం));
        l.x_norm = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*l.outputs, పరిమాణం(దశాంశం));
    }
    ఒకవేళ(adam){
        l.m = సున్నా_జ్ఞాపకం_ఇవ్వు(c*n*కొలత*కొలత, పరిమాణం(దశాంశం));
        l.v = సున్నా_జ్ఞాపకం_ఇవ్వు(c*n*కొలత*కొలత, పరిమాణం(దశాంశం));
        l.bias_m = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.scale_m = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.bias_v = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.scale_v = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
    }

#నిర్వచితమైతే GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    ఒకవేళ(gpu_సూచీ >= 0){

        ఒకవేళ (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*కొలత*కొలత);
            l.v_gpu = cuda_make_array(l.v, c*n*కొలత*కొలత);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*కొలత*కొలత);
        l.బరువు_updates_gpu = cuda_make_array(l.బరువు_updates, c*n*కొలత*కొలత);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.బహిర్గతం_h*l.బహిర్గతం_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.బహిర్గతం_h*l.బహిర్గతం_w*n);

        ఒకవేళ(batch_normalize){
            l.సగటు_gpu = cuda_make_array(0, n);
            l.variance_gpu = cuda_make_array(0, n);

            l.rolling_సగటు_gpu = cuda_make_array(0, n);
            l.rolling_variance_gpu = cuda_make_array(0, n);

            l.సగటు_delta_gpu = cuda_make_array(0, n);
            l.variance_delta_gpu = cuda_make_array(0, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(0, n);

            l.x_gpu = cuda_make_array(0, l.batch*l.బహిర్గతం_h*l.బహిర్గతం_w*n);
            l.x_norm_gpu = cuda_make_array(0, l.batch*l.బహిర్గతం_h*l.బహిర్గతం_w*n);
        }
    }
    #నిర్వచితమైతే CUDNN
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnn_సమితి_Tensor4d_Descriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, l.batch, l.బహిర్గతం_c, l.బహిర్గతం_h, l.బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, 1, l.బహిర్గతం_c, 1, 1); 
    #అయితే_ముగింపు
#అయితే_ముగింపు

    l.activation = activation;
    l.workspace_కొలత = get_workspace_కొలత(l);

    fprintf(stderr, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, కొలత, కొలత, stride, w, h, c, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c);

    తిరిగి l;
}

శూన్య denormalize_deconvolutional_layer(layer l)
{
    పూర్ణాంకం i, j;
    కొరకు(i = 0; i < l.n; ++i){
        దశాంశం scale = l.scales[i]/వర్గమూలం(l.rolling_variance[i] + .00001);
        కొరకు(j = 0; j < l.c*l.కొలత*l.కొలత; ++j){
            l.weights[i*l.c*l.కొలత*l.కొలత + j] *= scale;
        }
        l.biases[i] -= l.rolling_సగటు[i] * scale;
        l.scales[i] = 1;
        l.rolling_సగటు[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

శూన్య resize_deconvolutional_layer(layer *l, పూర్ణాంకం h, పూర్ణాంకం w)
{
    l->h = h;
    l->w = w;
    l->బహిర్గతం_h = (l->h - 1) * l->stride + l->కొలత - 2*l->pad;
    l->బహిర్గతం_w = (l->w - 1) * l->stride + l->కొలత - 2*l->pad;

    l->outputs = l->బహిర్గతం_h * l->బహిర్గతం_w * l->బహిర్గతం_c;
    l->inputs = l->w * l->h * l->c;

    l->output = realloc(l->output, l->batch*l->outputs*పరిమాణం(దశాంశం));
    l->delta  = realloc(l->delta,  l->batch*l->outputs*పరిమాణం(దశాంశం));
    ఒకవేళ(l->batch_normalize){
        l->x = realloc(l->x, l->batch*l->outputs*పరిమాణం(దశాంశం));
        l->x_norm  = realloc(l->x_norm, l->batch*l->outputs*పరిమాణం(దశాంశం));
    }

#నిర్వచితమైతే GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    ఒకవేళ(l->batch_normalize){
        cuda_free(l->x_gpu);
        cuda_free(l->x_norm_gpu);

        l->x_gpu = cuda_make_array(l->output, l->batch*l->outputs);
        l->x_norm_gpu = cuda_make_array(l->output, l->batch*l->outputs);
    }
    #నిర్వచితమైతే CUDNN
        cudnn_సమితి_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, l->batch, l->బహిర్గతం_c, l->బహిర్గతం_h, l->బహిర్గతం_w); 
        cudnn_సమితి_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, 1, l->బహిర్గతం_c, 1, 1); 
    #అయితే_ముగింపు
#అయితే_ముగింపు
    l->workspace_కొలత = get_workspace_కొలత(*l);
}

శూన్య forward_deconvolutional_layer(స్థిరరాశి layer l, జాలం net)
{
    పూర్ణాంకం i;

    పూర్ణాంకం m = l.కొలత*l.కొలత*l.n;
    పూర్ణాంకం n = l.h*l.w;
    పూర్ణాంకం k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    కొరకు(i = 0; i < l.batch; ++i){
        దశాంశం *a = l.weights;
        దశాంశం *b = net.ఆదానం + i*l.c*l.h*l.w;
        దశాంశం *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.బహిర్గతం_c, l.బహిర్గతం_h, l.బహిర్గతం_w, l.కొలత, l.stride, l.pad, l.output+i*l.outputs);
    }
    ఒకవేళ (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } లేకపోతే {
        కూడు_bias(l.output, l.biases, l.batch, l.n, l.బహిర్గతం_w*l.బహిర్గతం_h);
    }
    activate_array(l.output, l.batch*l.n*l.బహిర్గతం_w*l.బహిర్గతం_h, l.activation);
}

శూన్య backward_deconvolutional_layer(layer l, జాలం net)
{
    పూర్ణాంకం i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ఒకవేళ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } లేకపోతే {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.బహిర్గతం_w*l.బహిర్గతం_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    కొరకు(i = 0; i < l.batch; ++i){
        పూర్ణాంకం m = l.c;
        పూర్ణాంకం n = l.కొలత*l.కొలత*l.n;
        పూర్ణాంకం k = l.h*l.w;

        దశాంశం *a = net.ఆదానం + i*m*k;
        దశాంశం *b = net.workspace;
        దశాంశం *c = l.బరువు_updates;

        im2col_cpu(l.delta + i*l.outputs, l.బహిర్గతం_c, l.బహిర్గతం_h, l.బహిర్గతం_w, 
                l.కొలత, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        ఒకవేళ(net.delta){
            పూర్ణాంకం m = l.c;
            పూర్ణాంకం n = l.h*l.w;
            పూర్ణాంకం k = l.కొలత*l.కొలత*l.n;

            దశాంశం *a = l.weights;
            దశాంశం *b = net.workspace;
            దశాంశం *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

శూన్య update_deconvolutional_layer(layer l, update_వాదనలు a)
{
    దశాంశం learning_రేటు = a.learning_రేటు*l.learning_రేటు_scale;
    దశాంశం momentum = a.momentum;
    దశాంశం decay = a.decay;
    పూర్ణాంకం batch = a.batch;

    పూర్ణాంకం కొలత = l.కొలత*l.కొలత*l.c*l.n;
    axpy_cpu(l.n, learning_రేటు/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ఒకవేళ(l.scales){
        axpy_cpu(l.n, learning_రేటు/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(కొలత, -decay*batch, l.weights, 1, l.బరువు_updates, 1);
    axpy_cpu(కొలత, learning_రేటు/batch, l.బరువు_updates, 1, l.weights, 1);
    scal_cpu(కొలత, momentum, l.బరువు_updates, 1);
}



