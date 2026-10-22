#समावेश "deconvolutional_layer.h"
#समावेश "convolutional_layer.h"
#समावेश "batchnorm_layer.h"
#समावेश "utils.h"
#समावेश "im2col.h"
#समावेश "col2im.h"
#समावेश "blas.h"
#समावेश "gemm.h"

#समावेश <stdio.h>
#समावेश <time.h>


अटल माप_प्रकार get_workspace_आकार(layer l){
    वापस (माप_प्रकार)l.h*l.w*l.आकार*l.आकार*l.n*माप(भग्न);
}

व्योम bilinear_init(layer l)
{
    पूर्णांक i,j,f;
    भग्न center = (l.आकार-1) / 2.;
    क्रम(f = 0; f < l.n; ++f){
        क्रम(j = 0; j < l.आकार; ++j){
            क्रम(i = 0; i < l.आकार; ++i){
                भग्न val = (1 - भ_असल(i - center)) * (1 - भ_असल(j - center));
                पूर्णांक c = f%l.c;
                पूर्णांक ind = f*l.आकार*l.आकार*l.c + c*l.आकार*l.आकार + j*l.आकार + i;
                l.weights[ind] = val;
            }
        }
    }
}


layer make_deconvolutional_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक n, पूर्णांक आकार, पूर्णांक stride, पूर्णांक padding, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam)
{
    पूर्णांक i;
    layer l = {0};
    l.type = DECONVOLUTIONAL;

    l.h = h;
    l.w = w;
    l.c = c;
    l.n = n;
    l.batch = batch;
    l.stride = stride;
    l.आकार = आकार;

    l.nweights = c*n*आकार*आकार;
    l.nbiases = n;

    l.weights = सुस्मृति(c*n*आकार*आकार, माप(भग्न));
    l.weight_updates = सुस्मृति(c*n*आकार*आकार, माप(भग्न));

    l.biases = सुस्मृति(n, माप(भग्न));
    l.bias_updates = सुस्मृति(n, माप(भग्न));
    //float scale = n/(size*size*c);
    //printf("scale: %f\n", scale);
    भग्न scale = .02;
    क्रम(i = 0; i < c*n*आकार*आकार; ++i) l.weights[i] = scale*rand_normal();
    //bilinear_init(l);
    क्रम(i = 0; i < n; ++i){
        l.biases[i] = 0;
    }
    l.pad = padding;

    l.out_h = (l.h - 1) * l.stride + l.आकार - 2*l.pad;
    l.out_w = (l.w - 1) * l.stride + l.आकार - 2*l.pad;
    l.out_c = n;
    l.outputs = l.out_w * l.out_h * l.out_c;
    l.inputs = l.w * l.h * l.c;

    scal_cpu(l.nweights, (भग्न)l.out_w*l.out_h/(l.w*l.h), l.weights, 1);

    l.output = सुस्मृति(l.batch*l.outputs, माप(भग्न));
    l.delta  = सुस्मृति(l.batch*l.outputs, माप(भग्न));

    l.forward = forward_deconvolutional_layer;
    l.backward = backward_deconvolutional_layer;
    l.update = update_deconvolutional_layer;

    l.batch_normalize = batch_normalize;

    अगर(batch_normalize){
        l.scales = सुस्मृति(n, माप(भग्न));
        l.scale_updates = सुस्मृति(n, माप(भग्न));
        क्रम(i = 0; i < n; ++i){
            l.scales[i] = 1;
        }

        l.mean = सुस्मृति(n, माप(भग्न));
        l.variance = सुस्मृति(n, माप(भग्न));

        l.mean_delta = सुस्मृति(n, माप(भग्न));
        l.variance_delta = सुस्मृति(n, माप(भग्न));

        l.rolling_mean = सुस्मृति(n, माप(भग्न));
        l.rolling_variance = सुस्मृति(n, माप(भग्न));
        l.x = सुस्मृति(l.batch*l.outputs, माप(भग्न));
        l.x_norm = सुस्मृति(l.batch*l.outputs, माप(भग्न));
    }
    अगर(adam){
        l.m = सुस्मृति(c*n*आकार*आकार, माप(भग्न));
        l.v = सुस्मृति(c*n*आकार*आकार, माप(भग्न));
        l.bias_m = सुस्मृति(n, माप(भग्न));
        l.scale_m = सुस्मृति(n, माप(भग्न));
        l.bias_v = सुस्मृति(n, माप(भग्न));
        l.scale_v = सुस्मृति(n, माप(भग्न));
    }

#अगर_घोषित GPU
    l.forward_gpu = forward_deconvolutional_layer_gpu;
    l.backward_gpu = backward_deconvolutional_layer_gpu;
    l.update_gpu = update_deconvolutional_layer_gpu;

    अगर(gpu_सूचकांक >= 0){

        अगर (adam) {
            l.m_gpu = cuda_make_array(l.m, c*n*आकार*आकार);
            l.v_gpu = cuda_make_array(l.v, c*n*आकार*आकार);
            l.bias_m_gpu = cuda_make_array(l.bias_m, n);
            l.bias_v_gpu = cuda_make_array(l.bias_v, n);
            l.scale_m_gpu = cuda_make_array(l.scale_m, n);
            l.scale_v_gpu = cuda_make_array(l.scale_v, n);
        }
        l.weights_gpu = cuda_make_array(l.weights, c*n*आकार*आकार);
        l.weight_updates_gpu = cuda_make_array(l.weight_updates, c*n*आकार*आकार);

        l.biases_gpu = cuda_make_array(l.biases, n);
        l.bias_updates_gpu = cuda_make_array(l.bias_updates, n);

        l.delta_gpu = cuda_make_array(l.delta, l.batch*l.out_h*l.out_w*n);
        l.output_gpu = cuda_make_array(l.output, l.batch*l.out_h*l.out_w*n);

        अगर(batch_normalize){
            l.mean_gpu = cuda_make_array(0, n);
            l.variance_gpu = cuda_make_array(0, n);

            l.rolling_mean_gpu = cuda_make_array(0, n);
            l.rolling_variance_gpu = cuda_make_array(0, n);

            l.mean_delta_gpu = cuda_make_array(0, n);
            l.variance_delta_gpu = cuda_make_array(0, n);

            l.scales_gpu = cuda_make_array(l.scales, n);
            l.scale_updates_gpu = cuda_make_array(0, n);

            l.x_gpu = cuda_make_array(0, l.batch*l.out_h*l.out_w*n);
            l.x_norm_gpu = cuda_make_array(0, l.batch*l.out_h*l.out_w*n);
        }
    }
    #अगर_घोषित CUDNN
        cudnnCreateTensorDescriptor(&l.dstTensorDesc);
        cudnnCreateTensorDescriptor(&l.normTensorDesc);
        cudnnSetTensor4dDescriptor(l.dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, l.batch, l.out_c, l.out_h, l.out_w); 
        cudnnSetTensor4dDescriptor(l.normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, 1, l.out_c, 1, 1); 
    #पूर्ण_अगर
#पूर्ण_अगर

    l.activation = activation;
    l.workspace_आकार = get_workspace_आकार(l);

    ख_लिखो(मानक_त्रुटि, "deconv%5d %2d x%2d /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", n, आकार, आकार, stride, w, h, c, l.out_w, l.out_h, l.out_c);

    वापस l;
}

व्योम denormalize_deconvolutional_layer(layer l)
{
    पूर्णांक i, j;
    क्रम(i = 0; i < l.n; ++i){
        भग्न scale = l.scales[i]/वर्ग_मूल(l.rolling_variance[i] + .00001);
        क्रम(j = 0; j < l.c*l.आकार*l.आकार; ++j){
            l.weights[i*l.c*l.आकार*l.आकार + j] *= scale;
        }
        l.biases[i] -= l.rolling_mean[i] * scale;
        l.scales[i] = 1;
        l.rolling_mean[i] = 0;
        l.rolling_variance[i] = 1;
    }
}

व्योम resize_deconvolutional_layer(layer *l, पूर्णांक h, पूर्णांक w)
{
    l->h = h;
    l->w = w;
    l->out_h = (l->h - 1) * l->stride + l->आकार - 2*l->pad;
    l->out_w = (l->w - 1) * l->stride + l->आकार - 2*l->pad;

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->w * l->h * l->c;

    l->output = पुनः_स्मृति(l->output, l->batch*l->outputs*माप(भग्न));
    l->delta  = पुनः_स्मृति(l->delta,  l->batch*l->outputs*माप(भग्न));
    अगर(l->batch_normalize){
        l->x = पुनः_स्मृति(l->x, l->batch*l->outputs*माप(भग्न));
        l->x_norm  = पुनः_स्मृति(l->x_norm, l->batch*l->outputs*माप(भग्न));
    }

#अगर_घोषित GPU
    cuda_free(l->delta_gpu);
    cuda_free(l->output_gpu);

    l->delta_gpu =  cuda_make_array(l->delta,  l->batch*l->outputs);
    l->output_gpu = cuda_make_array(l->output, l->batch*l->outputs);

    अगर(l->batch_normalize){
        cuda_free(l->x_gpu);
        cuda_free(l->x_norm_gpu);

        l->x_gpu = cuda_make_array(l->output, l->batch*l->outputs);
        l->x_norm_gpu = cuda_make_array(l->output, l->batch*l->outputs);
    }
    #अगर_घोषित CUDNN
        cudnnSetTensor4dDescriptor(l->dstTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, l->batch, l->out_c, l->out_h, l->out_w); 
        cudnnSetTensor4dDescriptor(l->normTensorDesc, CUDNN_TENSOR_NCHW, CUDNN_तथ्य_FLOAT, 1, l->out_c, 1, 1); 
    #पूर्ण_अगर
#पूर्ण_अगर
    l->workspace_आकार = get_workspace_आकार(*l);
}

व्योम forward_deconvolutional_layer(स्थिर layer l, network net)
{
    पूर्णांक i;

    पूर्णांक m = l.आकार*l.आकार*l.n;
    पूर्णांक n = l.h*l.w;
    पूर्णांक k = l.c;

    fill_cpu(l.outputs*l.batch, 0, l.output, 1);

    क्रम(i = 0; i < l.batch; ++i){
        भग्न *a = l.weights;
        भग्न *b = net.निवेश + i*l.c*l.h*l.w;
        भग्न *c = net.workspace;

        gemm_cpu(1,0,m,n,k,1,a,m,b,n,0,c,n);

        col2im_cpu(net.workspace, l.out_c, l.out_h, l.out_w, l.आकार, l.stride, l.pad, l.output+i*l.outputs);
    }
    अगर (l.batch_normalize) {
        forward_batchnorm_layer(l, net);
    } अन्यथा {
        add_bias(l.output, l.biases, l.batch, l.n, l.out_w*l.out_h);
    }
    activate_array(l.output, l.batch*l.n*l.out_w*l.out_h, l.activation);
}

व्योम backward_deconvolutional_layer(layer l, network net)
{
    पूर्णांक i;

    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);

    अगर(l.batch_normalize){
        backward_batchnorm_layer(l, net);
    } अन्यथा {
        backward_bias(l.bias_updates, l.delta, l.batch, l.n, l.out_w*l.out_h);
    }

    //if(net.delta) memset(net.delta, 0, l.batch*l.h*l.w*l.c*sizeof(float));

    क्रम(i = 0; i < l.batch; ++i){
        पूर्णांक m = l.c;
        पूर्णांक n = l.आकार*l.आकार*l.n;
        पूर्णांक k = l.h*l.w;

        भग्न *a = net.निवेश + i*m*k;
        भग्न *b = net.workspace;
        भग्न *c = l.weight_updates;

        im2col_cpu(l.delta + i*l.outputs, l.out_c, l.out_h, l.out_w, 
                l.आकार, l.stride, l.pad, b);
        gemm_cpu(0,1,m,n,k,1,a,k,b,k,1,c,n);

        अगर(net.delta){
            पूर्णांक m = l.c;
            पूर्णांक n = l.h*l.w;
            पूर्णांक k = l.आकार*l.आकार*l.n;

            भग्न *a = l.weights;
            भग्न *b = net.workspace;
            भग्न *c = net.delta + i*n*m;

            gemm_cpu(0,0,m,n,k,1,a,k,b,n,1,c,n);
        }
    }
}

व्योम update_deconvolutional_layer(layer l, update_args a)
{
    भग्न learning_rate = a.learning_rate*l.learning_rate_scale;
    भग्न momentum = a.momentum;
    भग्न decay = a.decay;
    पूर्णांक batch = a.batch;

    पूर्णांक आकार = l.आकार*l.आकार*l.c*l.n;
    axpy_cpu(l.n, learning_rate/batch, l.bias_updates, 1, l.biases, 1);
    scal_cpu(l.n, momentum, l.bias_updates, 1);

    अगर(l.scales){
        axpy_cpu(l.n, learning_rate/batch, l.scale_updates, 1, l.scales, 1);
        scal_cpu(l.n, momentum, l.scale_updates, 1);
    }

    axpy_cpu(आकार, -decay*batch, l.weights, 1, l.weight_updates, 1);
    axpy_cpu(आकार, learning_rate/batch, l.weight_updates, 1, l.weights, 1);
    scal_cpu(आकार, momentum, l.weight_updates, 1);
}



