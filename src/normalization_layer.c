#अन्तर्भाव "normalization_layer.h"
#अन्तर्भाव "blas.h"

#अन्तर्भाव <stdio.h>

layer make_normalization_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक size, दशांश alpha, दशांश beta, दशांश kappa)
{
    ख_लिखो(मानक_त्रुटि, "Local Response Normalization Layer: %d x %d x %d image, %d size\n", w,h,c,size);
    layer layer = {0};
    layer.type = NORMALIZATION;
    layer.batch = batch;
    layer.h = layer.out_h = h;
    layer.w = layer.out_w = w;
    layer.c = layer.out_c = c;
    layer.kappa = kappa;
    layer.size = size;
    layer.alpha = alpha;
    layer.beta = beta;
    layer.output = सुस्मृति(h * w * c * batch, आकार(दशांश));
    layer.delta = सुस्मृति(h * w * c * batch, आकार(दशांश));
    layer.squared = सुस्मृति(h * w * c * batch, आकार(दशांश));
    layer.norms = सुस्मृति(h * w * c * batch, आकार(दशांश));
    layer.inputs = w*h*c;
    layer.outputs = layer.inputs;

    layer.forward = forward_normalization_layer;
    layer.backward = backward_normalization_layer;
    #ifdef GPU
    layer.forward_gpu = forward_normalization_layer_gpu;
    layer.backward_gpu = backward_normalization_layer_gpu;

    layer.output_gpu =  cuda_make_array(layer.output, h * w * c * batch);
    layer.delta_gpu =   cuda_make_array(layer.delta, h * w * c * batch);
    layer.squared_gpu = cuda_make_array(layer.squared, h * w * c * batch);
    layer.norms_gpu =   cuda_make_array(layer.norms, h * w * c * batch);
    #endif
    परत layer;
}

शून्य resize_normalization_layer(layer *layer, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक c = layer->c;
    पूर्णांक batch = layer->batch;
    layer->h = h;
    layer->w = w;
    layer->out_h = h;
    layer->out_w = w;
    layer->inputs = w*h*c;
    layer->outputs = layer->inputs;
    layer->output = पुनः_स्मृति(layer->output, h * w * c * batch * आकार(दशांश));
    layer->delta = पुनः_स्मृति(layer->delta, h * w * c * batch * आकार(दशांश));
    layer->squared = पुनः_स्मृति(layer->squared, h * w * c * batch * आकार(दशांश));
    layer->norms = पुनः_स्मृति(layer->norms, h * w * c * batch * आकार(दशांश));
#ifdef GPU
    cuda_free(layer->output_gpu);
    cuda_free(layer->delta_gpu); 
    cuda_free(layer->squared_gpu); 
    cuda_free(layer->norms_gpu);   
    layer->output_gpu =  cuda_make_array(layer->output, h * w * c * batch);
    layer->delta_gpu =   cuda_make_array(layer->delta, h * w * c * batch);
    layer->squared_gpu = cuda_make_array(layer->squared, h * w * c * batch);
    layer->norms_gpu =   cuda_make_array(layer->norms, h * w * c * batch);
#endif
}

शून्य forward_normalization_layer(स्थिरांक layer layer, network net)
{
    पूर्णांक k,b;
    पूर्णांक w = layer.w;
    पूर्णांक h = layer.h;
    पूर्णांक c = layer.c;
    scal_cpu(w*h*c*layer.batch, 0, layer.squared, 1);

    साठी(b = 0; b < layer.batch; ++b){
        दशांश *squared = layer.squared + w*h*c*b;
        दशांश *norms   = layer.norms + w*h*c*b;
        दशांश *आदान   = net.आदान + w*h*c*b;
        pow_cpu(w*h*c, 2, आदान, 1, squared, 1);

        const_cpu(w*h, layer.kappa, norms, 1);
        साठी(k = 0; k < layer.size/2; ++k){
            axpy_cpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        साठी(k = 1; k < layer.c; ++k){
            copy_cpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            पूर्णांक prev = k - ((layer.size-1)/2) - 1;
            पूर्णांक next = k + (layer.size/2);
            जर(prev >= 0)      axpy_cpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            जर(next < layer.c) axpy_cpu(w*h,  layer.alpha, squared + w*h*next, 1, norms + w*h*k, 1);
        }
    }
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, layer.output, 1);
    mul_cpu(w*h*c*layer.batch, net.आदान, 1, layer.output, 1);
}

शून्य backward_normalization_layer(स्थिरांक layer layer, network net)
{
    // TODO This is approximate ;-)
    // Also this should add in to delta instead of overwritting.

    पूर्णांक w = layer.w;
    पूर्णांक h = layer.h;
    पूर्णांक c = layer.c;
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, net.delta, 1);
    mul_cpu(w*h*c*layer.batch, layer.delta, 1, net.delta, 1);
}

#ifdef GPU
शून्य forward_normalization_layer_gpu(स्थिरांक layer layer, network net)
{
    पूर्णांक k,b;
    पूर्णांक w = layer.w;
    पूर्णांक h = layer.h;
    पूर्णांक c = layer.c;
    scal_gpu(w*h*c*layer.batch, 0, layer.squared_gpu, 1);

    साठी(b = 0; b < layer.batch; ++b){
        दशांश *squared = layer.squared_gpu + w*h*c*b;
        दशांश *norms   = layer.norms_gpu + w*h*c*b;
        दशांश *आदान   = net.आदान_gpu + w*h*c*b;
        pow_gpu(w*h*c, 2, आदान, 1, squared, 1);

        const_gpu(w*h, layer.kappa, norms, 1);
        साठी(k = 0; k < layer.size/2; ++k){
            axpy_gpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        साठी(k = 1; k < layer.c; ++k){
            copy_gpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            पूर्णांक prev = k - ((layer.size-1)/2) - 1;
            पूर्णांक next = k + (layer.size/2);
            जर(prev >= 0)      axpy_gpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            जर(next < layer.c) axpy_gpu(w*h,  layer.alpha, squared + w*h*next, 1, norms + w*h*k, 1);
        }
    }
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, layer.output_gpu, 1);
    mul_gpu(w*h*c*layer.batch, net.आदान_gpu, 1, layer.output_gpu, 1);
}

शून्य backward_normalization_layer_gpu(स्थिरांक layer layer, network net)
{
    // TODO This is approximate ;-)

    पूर्णांक w = layer.w;
    पूर्णांक h = layer.h;
    पूर्णांक c = layer.c;
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, net.delta_gpu, 1);
    mul_gpu(w*h*c*layer.batch, layer.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
