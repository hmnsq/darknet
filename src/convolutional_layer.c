#అన్తర్భావ "convolutional_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "batchnorm_layer.h"
#అన్తర్భావ "im2col.h"
#అన్తర్భావ "col2im.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "gemm.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <time.h>

#నిర్వచితమైతే AI2
#అన్తర్భావ "xnor_layer.h"
#అయితే_ముగింపు

శూన్య మార్పిడి_ద్విమాన(convolutional_layer *l)
{
    దశాంశం *మార్పిడి = l->weights;
    l->weights = l->ద్విమాన_weights;
    l->ద్విమాన_weights = మార్పిడి;

#నిర్వచితమైతే GPU
    మార్పిడి = l->weights_gpu;
    l->weights_gpu = l->ద్విమాన_weights_gpu;
    l->ద్విమాన_weights_gpu = మార్పిడి;
#అయితే_ముగింపు
}

శూన్య binarize_weights(దశాంశం *weights, పూర్ణాంకం n, పూర్ణాంకం కొలత, దశాంశం *ద్విమాన)
{
    పూర్ణాంకం i, f;
    కొరకు(f = 0; f < n; ++f){
        దశాంశం సగటు = 0;
        కొరకు(i = 0; i < కొలత; ++i){
            సగటు += fabs(weights[f*కొలత + i]);
        }
        సగటు = సగటు / కొలత;
        కొరకు(i = 0; i < కొలత; ++i){
            ద్విమాన[f*కొలత + i] = (weights[f*కొలత + i] > 0) ? సగటు : -సగటు;
        }
    }
}

శూన్య binarize_cpu(దశాంశం *ఆదానం, పూర్ణాంకం n, దశాంశం *ద్విమాన)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        ద్విమాన[i] = (ఆదానం[i] > 0) ? 1 : -1;
    }
}

శూన్య binarize_ఆదానం(దశాంశం *ఆదానం, పూర్ణాంకం n, పూర్ణాంకం కొలత, దశాంశం *ద్విమాన)
{
    పూర్ణాంకం i, s;
    కొరకు(s = 0; s < కొలత; ++s){
        దశాంశం సగటు = 0;
        కొరకు(i = 0; i < n; ++i){
            సగటు += fabs(ఆదానం[i*కొలత + s]);
        }
        సగటు = సగటు / n;
        కొరకు(i = 0; i < n; ++i){
            ద్విమాన[i*కొలత + s] = (ఆదానం[i*కొలత + s] > 0) ? సగటు : -సగటు;
        }
    }
}

పూర్ణాంకం convolutional_బహిర్గతం_ఎత్తు(convolutional_layer l)
{
    తిరిగి (l.h + 2*l.pad - l.కొలత) / l.stride + 1;
}

పూర్ణాంకం convolutional_బహిర్గతం_వెడల్పు(convolutional_layer l)
{
    తిరిగి (l.w + 2*l.pad - l.కొలత) / l.stride + 1;
}

image get_convolutional_image(convolutional_layer l)
{
    తిరిగి float_గమ్యం_image(l.బహిర్గతం_w,l.బహిర్గతం_h,l.బహిర్గతం_c,l.output);
}

image get_convolutional_delta(convolutional_layer l)
{
    తిరిగి float_గమ్యం_image(l.బహిర్గతం_w,l.బహిర్గతం_h,l.బహిర్గతం_c,l.delta);
}

స్థిర size_t get_workspace_కొలత(layer l){
#నిర్వచితమైతే CUDNN
    ఒకవేళ(gpu_సూచీ >= 0){
        size_t most = 0;
        size_t s = 0;
        cudnn_Get_Convolution_Forward_Workspace_కొలత(cudnn_handle(),
                l.srcTensorDesc,
                l.బరువు_Desc,
                l.convDesc,
                l.dstTensorDesc,
                l.fw_algo,
                &s);
        ఒకవేళ (s > most) most = s;
        cudnn_Get_Convolution_Backward_Filter_Workspace_కొలత(cudnn_handle(),
                l.srcTensorDesc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dweightDesc,
                l.bf_algo,
                &s);
        ఒకవేళ (s > most) most = s;
        cudnn_Get_Convolution_Backward_దత్తాంశం_Workspace_కొలత(cudnn_handle(),
                l.బరువు_Desc,
                l.ddstTensorDesc,
                l.convDesc,
                l.dsrcTensorDesc,
                l.bd_algo,
                &s);
        ఒకవేళ (s > most) most = s;
        తిరిగి most;
    }
#అయితే_ముగింపు
    తిరిగి (size_t)l.బహిర్గతం_h*l.బహిర్గతం_w*l.కొలత*l.కొలత*l.c/l.groups*పరిమాణం(దశాంశం);
}

#నిర్వచితమైతే GPU
#నిర్వచితమైతే CUDNN
శూన్య cudnn_convolutional_అమరిక(layer *l)
{
    cudnn_సమితి_Tensor4d_Descriptor(l->dsrcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_సమితి_Tensor4d_Descriptor(l->ddstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, l->batch, l->బహిర్గతం_c, l->బహిర్గతం_h, l->బహిర్గతం_w); 

    cudnn_సమితి_Tensor4d_Descriptor(l->srcTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, l->batch, l->c, l->h, l->w); 
    cudnn_సమితి_Tensor4d_Descriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, l->batch, l->బహిర్గతం_c, l->బహిర్గతం_h, l->బహిర్గతం_w); 
    cudnn_సమితి_Tensor4d_Descriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_దత్తాంశం_FLOAT, 1, l->బహిర్గతం_c, 1, 1); 

    cudnn_సమితి_Filter4d_Descriptor(l->dweightDesc, CUDNN_దత్తాంశం_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->కొలత, l->కొలత); 
    cudnn_సమితి_Filter4d_Descriptor(l->బరువు_Desc, CUDNN_దత్తాంశం_FLOAT, CUDNN_TENSOR_NCHW, l->n, l->c/l->groups, l->కొలత, l->కొలత); 
    #ఒకవేళ CUDNN_MAJOR >= 6
    cudnn_సమితి_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION, CUDNN_దత్తాంశం_FLOAT);
    #లేకపోతే
    cudnn_సమితి_Convolution2d_Descriptor(l->convDesc, l->pad, l->pad, l->stride, l->stride, 1, 1, CUDNN_CROSS_CORRELATION);
    #అయితే_ముగింపు

    #ఒకవేళ CUDNN_MAJOR >= 7
    cudnn_సమితి_Convolution_సమూహం_లెక్క(l->convDesc, l->groups);
    #లేకపోతే
    ఒకవేళ(l->groups > 1){
        error("CUDNN < 7 doesn't support groups, please upgrade!");
    }
    #అయితే_ముగింపు

    cudnnGetConvolutionForwardAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->బరువు_Desc,
            l->convDesc,
            l->dstTensorDesc,
            CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_హద్దు,
            2000000000,
            &l->fw_algo);
    cudnn_Get_Convolution_Backward_దత్తాంశం_Algorithm(cudnn_handle(),
            l->బరువు_Desc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dsrcTensorDesc,
            CUDNN_CONVOLUTION_BWD_దత్తాంశం_SPECIFY_WORKSPACE_హద్దు,
            2000000000,
            &l->bd_algo);
    cudnnGetConvolutionBackwardFilterAlgorithm(cudnn_handle(),
            l->srcTensorDesc,
            l->ddstTensorDesc,
            l->convDesc,
            l->dweightDesc,
            CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_హద్దు,
            2000000000,
            &l->bf_algo);
}
#అయితే_ముగింపు
#అయితే_ముగింపు

convolutional_layer make_convolutional_layer(పూర్ణాంకం batch, పూర్ణాంకం h, పూర్ణాంకం w, పూర్ణాంకం c, పూర్ణాంకం n, పూర్ణాంకం groups, పూర్ణాంకం కొలత, పూర్ణాంకం stride, పూర్ణాంకం padding, ACTIVATION activation, పూర్ణాంకం batch_normalize, పూర్ణాంకం ద్విమాన, పూర్ణాంకం xnor, పూర్ణాంకం adam)
{
    పూర్ణాంకం i;
    convolutional_layer l = {0};
    l.type = CONVOLUTIONAL;

    l.groups = groups;
    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.ద్విమాన = ద్విమాన;
    l.xnor = xnor;
    l.batch = batch;
    l.stride = stride;
    l.కొలత = కొలత;
    l.pad = padding;
    l.batch_normalize = batch_normalize;

    l.weights = సున్నా_జ్ఞాపకం_ఇవ్వు(c/groups*n*కొలత*కొలత, పరిమాణం(దశాంశం));
    l.బరువు_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(c/groups*n*కొలత*కొలత, పరిమాణం(దశాంశం));

    l.biases = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
    l.bias_updates = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));

    l.nweights = c/groups*n*కొలత*కొలత;
    l.nbiases = n;

    // float scale = 1./sqrt(size*size*c);
    దశాంశం scale = వర్గమూలం(2./(కొలత*కొలత*c/l.groups));
    //printf("convscale %f\n", scale);
    //scale = .02;
    //for(i = 0; i < c*n*size*size; ++i) l.weights[i] = scale*rand_uniform(-1, 1);
    కొరకు(i = 0; i < l.nweights; ++i) l.weights[i] = scale*rand_సాధారణ();
    పూర్ణాంకం బహిర్గతం_w = convolutional_బహిర్గతం_వెడల్పు(l);
    పూర్ణాంకం బహిర్గతం_h = convolutional_బహిర్గతం_ఎత్తు(l);
    l.బహిర్గతం_h = బహిర్గతం_h;
    l.బహిర్గతం_w = బహిర్గతం_w;
    l.బహిర్గతం_c = n;
    l.outputs = l.బహిర్గతం_h * l.బహిర్గతం_w * l.బహిర్గతం_c;
    l.inputs = l.w * l.h * l.c;

    l.output = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*l.outputs, పరిమాణం(దశాంశం));
    l.delta  = సున్నా_జ్ఞాపకం_ఇవ్వు(l.batch*l.outputs, పరిమాణం(దశాంశం));

    l.forward = forward_convolutional_layer;
    l.backward = backward_convolutional_layer;
    l.update = update_convolutional_layer;
    ఒకవేళ(ద్విమాన){
        l.ద్విమాన_weights = సున్నా_జ్ఞాపకం_ఇవ్వు(l.nweights, పరిమాణం(దశాంశం));
        l.cweights = సున్నా_జ్ఞాపకం_ఇవ్వు(l.nweights, పరిమాణం(అక్షరం));
        l.scales = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
    }
    ఒకవేళ(xnor){
        l.ద్విమాన_weights = సున్నా_జ్ఞాపకం_ఇవ్వు(l.nweights, పరిమాణం(దశాంశం));
        l.ద్విమాన_ఆదానం = సున్నా_జ్ఞాపకం_ఇవ్వు(l.inputs*l.batch, పరిమాణం(దశాంశం));
    }

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
        l.m = సున్నా_జ్ఞాపకం_ఇవ్వు(l.nweights, పరిమాణం(దశాంశం));
        l.v = సున్నా_జ్ఞాపకం_ఇవ్వు(l.nweights, పరిమాణం(దశాంశం));
        l.bias_m = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.scale_m = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.bias_v = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
        l.scale_v = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం));
    }

#నిర్వచితమైతే GPU
    l.forward_gpu = forward_convolutional_layer_gpu;
    l.backward_gpu = backward_convolutional_layer_gpu;
    l.update_gpu = update_convolutional_layer_gpu;

    ఒకవేళ(gpu_సూచీ >= 0){
        ఒకవేళ (adam) {
            l.m_gpu = cuda_make_array(l.m, l.nweights);
            l.v_gpu = cuda_make_array(l.v, l.nweights);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }

        l.weights_gpu = cuda_make_array(l.weights, l.nweights);
        l.బరువు_updates_gpu = cuda_make_array(l.బరువు_updates, l.nweights);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*బహిర్గతం_h*బహిర్గతం_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*బహిర్గతం_h*బహిర్గతం_w*n);

        ఒకవేళ(ద్విమాన){
            l.ద్విమాన_weights_gpu = cuda_make_array(l.weights, l.nweights);
        }
        ఒకవేళ(xnor){
            l.ద్విమాన_weights_gpu = cuda_make_array(l.weights, l.nweights);
            l.ద్విమాన_ఆదానం_gpu = cuda_make_array(0, l.inputs*l.batch);
        }

        ఒకవేళ(batch_normalize){
            l.సగటు_gpu = cuda_make_array(l.సగటు, n);
            l.variance_gpu = cuda_make_array(l.variance, n);

            l.rolling_సగటు_gpu = cuda_make_array(l.సగటు, n);
            l.rolling_variance_gpu = cuda_make_array(l.variance, n);

            l.సగటు_delta_gpu = cuda_make_array(l.సగటు, n);
            l.variance_delta_gpu = cuda_make_array(l.variance, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(l.scale_updates, n);

            l.x_gpu = cuda_make_array(l.output, l.batch*బహిర్గతం_h*బహిర్గతం_w*n);
            l.x_norm_gpu = cuda_make_array(l.output, l.batch*బహిర్గతం_h*బహిర్గతం_w*n);
        }
#నిర్వచితమైతే CUDNN
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnCreateTensorDescriptor(&l.srcTensorDesc);
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateFilterDescriptor(&l.బరువు_Desc);
        cudnnCreateTensorDescriptor(&l.dsrcTensorDesc);
        cudnnCreateTensorDescriptor(&l.ddstTensorDesc);
        cudnnCreateFilterDescriptor(&l.dweightDesc);
        cudnnCreateConvolutionDescriptor(&l.convDesc);
        cudnn_convolutional_అమరిక(&l);
#అయితే_ముగింపు
    }
#అయితే_ముగింపు
    l.workspace_కొలత = get_workspace_కొలత(l);
    l.activation = activation;

    fprintf(stderr, "conv  %5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d  %5.3f BFLOPs\n", n, కొలత, కొలత, stride, w, h, c, l.బహిర్గతం_w, l.బహిర్గతం_h, l.బహిర్గతం_c, (2.0 * l.n * l.కొలత*l.కొలత*l.c/l.groups * l.బహిర్గతం_h*l.బహిర్గతం_w)/1000000000.);

    తిరిగి l;
}

శూన్య denormalize_convolutional_layer(convolutional_layer l)
{
    పూర్ణాంకం i, j;
    కొరకు(i = 0; i < l.n; ++i){
        దశాంశం scale = l.scales[i]/వర్గమూలం(l.rolling_variance[i] + .00001);
        కొరకు(j = 0; j < l.c/l.groups*l.కొలత*l.కొలత; ++j){
            l.weights[i*l.c/l.groups*l.కొలత*l.కొలత + j] *= scale;
        }
        l.biases[i] -= l.rolling_సగటు[i] * scale;
        l.scales[i] = 1;
        l.rolling_సగటు[i] = 0;
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

శూన్య resize_convolutional_layer(convolutional_layer *l, పూర్ణాంకం w, పూర్ణాంకం h)
{
    l->w = w;
    l->h = h;
    పూర్ణాంకం బహిర్గతం_w = convolutional_బహిర్గతం_వెడల్పు(*l);
    పూర్ణాంకం బహిర్గతం_h = convolutional_బహిర్గతం_ఎత్తు(*l);

    l->బహిర్గతం_w = బహిర్గతం_w;
    l->బహిర్గతం_h = బహిర్గతం_h;

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
    cudnn_convolutional_అమరిక(l);
#అయితే_ముగింపు
#అయితే_ముగింపు
    l->workspace_కొలత = get_workspace_కొలత(*l);
}

శూన్య కూడు_bias(దశాంశం *output, దశాంశం *biases, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత)
{
    పూర్ణాంకం i,j,b;
    కొరకు(b = 0; b < batch; ++b){
        కొరకు(i = 0; i < n; ++i){
            కొరకు(j = 0; j < కొలత; ++j){
                output[(b*n + i)*కొలత + j] += biases[i];
            }
        }
    }
}

శూన్య scale_bias(దశాంశం *output, దశాంశం *scales, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత)
{
    పూర్ణాంకం i,j,b;
    కొరకు(b = 0; b < batch; ++b){
        కొరకు(i = 0; i < n; ++i){
            కొరకు(j = 0; j < కొలత; ++j){
                output[(b*n + i)*కొలత + j] *= scales[i];
            }
        }
    }
}

శూన్య backward_bias(దశాంశం *bias_updates, దశాంశం *delta, పూర్ణాంకం batch, పూర్ణాంకం n, పూర్ణాంకం కొలత)
{
    పూర్ణాంకం i,b;
    కొరకు(b = 0; b < batch; ++b){
        కొరకు(i = 0; i < n; ++i){
            bias_updates[i] += కూడిక_array(delta+కొలత*(i+b*n), కొలత);
        }
    }
}

శూన్య forward_convolutional_layer(convolutional_layer l, జాలం net)
{
    పూర్ణాంకం i, j;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    ఒకవేళ(l.xnor){
        binarize_weights(l.weights, l.n, l.c/l.groups*l.కొలత*l.కొలత, l.ద్విమాన_weights);
        మార్పిడి_ద్విమాన(&l);
        binarize_cpu(net.ఆదానం, l.c*l.h*l.w*l.batch, l.ద్విమాన_ఆదానం);
        net.ఆదానం = l.ద్విమాన_ఆదానం;
    }

    పూర్ణాంకం m = l.n/l.groups;
    పూర్ణాంకం k = l.కొలత*l.కొలత*l.c/l.groups;
    పూర్ణాంకం n = l.బహిర్గతం_w*l.బహిర్గతం_h;
    కొరకు(i = 0; i < l.batch; ++i){
        కొరకు(j = 0; j < l.groups; ++j){
            దశాంశం *a = l.weights + j*l.nweights/l.groups;
            దశాంశం *b = net.workspace;
            దశాంశం *c = l.output + (i*l.groups + j)*n*m;
            దశాంశం *im =  net.ఆదానం + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ఒకవేళ (l.కొలత == 1) {
                b = im;
            } లేకపోతే {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, l.కొలత, l.stride, l.pad, b);
            }
            gemm(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }

    ఒకవేళ(l.batch_normalize){
        forward_batchnorm_layer(l, net);
    } లేకపోతే {
        కూడు_bias(l.output, l.biases, l.batch, l.n, l.బహిర్గతం_h*l.బహిర్గతం_w);
    }

    activate_array(l.output, l.outputs*l.batch, l.activation);
    ఒకవేళ(l.ద్విమాన || l.xnor) మార్పిడి_ద్విమాన(&l);
}

శూన్య backward_convolutional_layer(convolutional_layer l, జాలం net)
{
    పూర్ణాంకం i, j;
    పూర్ణాంకం m = l.n/l.groups;
    పూర్ణాంకం n = l.కొలత*l.కొలత*l.c/l.groups;
    పూర్ణాంకం k = l.బహిర్గతం_w*l.బహిర్గతం_h;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    ఒకవేళ(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } లేకపోతే {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, k);
    }

    కొరకు(i = 0; i < l.batch; ++i){
        కొరకు(j = 0; j < l.groups; ++j){
            దశాంశం *a = l.delta + (i*l.groups + j)*m*k;
            దశాంశం *b = net.workspace;
            దశాంశం *c = l.బరువు_updates + j*l.nweights/l.groups;

            దశాంశం *im  = net.ఆదానం + (i*l.groups + j)*l.c/l.groups*l.h*l.w;
            దశాంశం *imd = net.delta + (i*l.groups + j)*l.c/l.groups*l.h*l.w;

            ఒకవేళ(l.కొలత == 1){
                b = im;
            } లేకపోతే {
                im2col_cpu(im, l.c/l.groups, l.h, l.w, 
                        l.కొలత, l.stride, l.pad, b);
            }

            gemm(0,1,m,n,k,1,a,k,b,k,1,c,n);

            ఒకవేళ (net.delta) {
                a = l.weights + j*l.nweights/l.groups;
                b = l.delta + (i*l.groups + j)*m*k;
                c = net.workspace;
                ఒకవేళ (l.కొలత == 1) {
                    c = imd;
                }

                gemm(1,0,n,k,m,1,a,n,b,k,0,c,k);

                ఒకవేళ (l.కొలత != 1) {
                    col2im_cpu(net.workspace, l.c/l.groups, l.h, l.w, l.కొలత, l.stride, l.pad, imd);
                }
            }
        }
    }
}

శూన్య update_convolutional_layer(convolutional_layer l, update_వాదనలు a)
{
    దశాంశం learning_రేటు = a.learning_రేటు*l.learning_రేటు_scale;
    దశాంశం momentum = a.momentum;
    దశాంశం decay = a.decay;
    పూర్ణాంకం batch = a.batch;

    axpy_cpu(l.n, learning_రేటు/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    ఒకవేళ(l.scales){
        axpy_cpu(l.n, learning_రేటు/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(l.nweights, -decay*batch, l.weights, 1, l.బరువు_updates, 1);
    axpy_cpu(l.nweights, learning_రేటు/batch, l.బరువు_updates, 1, l.weights, 1);
    scal_cpu(l.nweights, momentum, l.బరువు_updates, 1);
}


image get_convolutional_బరువు(convolutional_layer l, పూర్ణాంకం i)
{
    పూర్ణాంకం h = l.కొలత;
    పూర్ణాంకం w = l.కొలత;
    పూర్ణాంకం c = l.c/l.groups;
    తిరిగి float_గమ్యం_image(w,h,c,l.weights+i*h*w*c);
}

శూన్య rgbgr_weights(convolutional_layer l)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < l.n; ++i){
        image im = get_convolutional_బరువు(l, i);
        ఒకవేళ (im.c == 3) {
            rgbgr_image(im);
        }
    }
}

శూన్య rescale_weights(convolutional_layer l, దశాంశం scale, దశాంశం trans)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < l.n; ++i){
        image im = get_convolutional_బరువు(l, i);
        ఒకవేళ (im.c == 3) {
            scale_image(im, scale);
            దశాంశం కూడిక = కూడిక_array(im.దత్తాంశం, im.w*im.h*im.c);
            l.biases[i] += కూడిక*trans;
        }
    }
}

image *get_weights(convolutional_layer l)
{
    image *weights = సున్నా_జ్ఞాపకం_ఇవ్వు(l.n, పరిమాణం(image));
    పూర్ణాంకం i;
    కొరకు(i = 0; i < l.n; ++i){
        weights[i] = copy_image(get_convolutional_బరువు(l, i));
        normalize_image(weights[i]);
        /*
           char buff[256];
           sprintf(buff, "filter%d", i);
           save_image(weights[i], buff);
         */
    }
    //error("hey");
    తిరిగి weights;
}

image *visualize_convolutional_layer(convolutional_layer l, అక్షరం *కిటికీ, image *prev_weights)
{
    image *single_weights = get_weights(l);
    show_images(single_weights, l.n, కిటికీ);

    image delta = get_convolutional_image(l);
    image dc = collapse_image_layers(delta, 1);
    అక్షరం buff[256];
    sprintf(buff, "%s: Output", కిటికీ);
    //show_image(dc, buff);
    //save_image(dc, buff);
    free_image(dc);
    తిరిగి single_weights;
}

