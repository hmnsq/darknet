#ਅਨ੍ਤਰ੍ਭਾਵ "normalization_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

layer make_normalization_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ alpha, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ kappa)
{
    fprintf(stderr, "Local Response Normalization Layer: %d x %d x %d image, %d size\n", w,h,c,ਮਾਪ);
    layer layer = {0};
    layer.type = NORMALIZATION;
    layer.batch = batch;
    layer.h = layer.ਬਾਹਰਲਾ_h = h;
    layer.w = layer.ਬਾਹਰਲਾ_w = w;
    layer.c = layer.ਬਾਹਰਲਾ_c = c;
    layer.kappa = kappa;
    layer.ਮਾਪ = ਮਾਪ;
    layer.alpha = alpha;
    layer.beta = beta;
    layer.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(h * w * c * batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    layer.delta = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(h * w * c * batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    layer.squared = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(h * w * c * batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    layer.norms = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(h * w * c * batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    layer.inputs = w*h*c;
    layer.outputs = layer.inputs;

    layer.forward = forward_normalization_layer;
    layer.backward = backward_normalization_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    layer.forward_gpu = forward_normalization_layer_gpu;
    layer.backward_gpu = backward_normalization_layer_gpu;

    layer.output_gpu =  cuda_make_array(layer.output, h * w * c * batch);
    layer.delta_gpu =   cuda_make_array(layer.delta, h * w * c * batch);
    layer.squared_gpu = cuda_make_array(layer.squared, h * w * c * batch);
    layer.norms_gpu =   cuda_make_array(layer.norms, h * w * c * batch);
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ layer;
}

ਖਾਲੀ resize_normalization_layer(layer *layer, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ c = layer->c;
    ਪੂਰਨ batch = layer->batch;
    layer->h = h;
    layer->w = w;
    layer->ਬਾਹਰਲਾ_h = h;
    layer->ਬਾਹਰਲਾ_w = w;
    layer->inputs = w*h*c;
    layer->outputs = layer->inputs;
    layer->output = realloc(layer->output, h * w * c * batch * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    layer->delta = realloc(layer->delta, h * w * c * batch * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    layer->squared = realloc(layer->squared, h * w * c * batch * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    layer->norms = realloc(layer->norms, h * w * c * batch * ਆਕਾਰ(ਦਸ਼ਮਲਵ));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(layer->output_gpu);
    cuda_free(layer->delta_gpu); 
    cuda_free(layer->squared_gpu); 
    cuda_free(layer->norms_gpu);   
    layer->output_gpu =  cuda_make_array(layer->output, h * w * c * batch);
    layer->delta_gpu =   cuda_make_array(layer->delta, h * w * c * batch);
    layer->squared_gpu = cuda_make_array(layer->squared, h * w * c * batch);
    layer->norms_gpu =   cuda_make_array(layer->norms, h * w * c * batch);
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ forward_normalization_layer(ਅਚਲ layer layer, ਜਾਲ net)
{
    ਪੂਰਨ k,b;
    ਪੂਰਨ w = layer.w;
    ਪੂਰਨ h = layer.h;
    ਪੂਰਨ c = layer.c;
    scal_cpu(w*h*c*layer.batch, 0, layer.squared, 1);

    ਲਈ(b = 0; b < layer.batch; ++b){
        ਦਸ਼ਮਲਵ *squared = layer.squared + w*h*c*b;
        ਦਸ਼ਮਲਵ *norms   = layer.norms + w*h*c*b;
        ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼   = net.ਨਿਵੇਸ਼ + w*h*c*b;
        pow_cpu(w*h*c, 2, ਨਿਵੇਸ਼, 1, squared, 1);

        const_cpu(w*h, layer.kappa, norms, 1);
        ਲਈ(k = 0; k < layer.ਮਾਪ/2; ++k){
            axpy_cpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        ਲਈ(k = 1; k < layer.c; ++k){
            copy_cpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            ਪੂਰਨ prev = k - ((layer.ਮਾਪ-1)/2) - 1;
            ਪੂਰਨ ਅਗਲਾ = k + (layer.ਮਾਪ/2);
            ਜੇ(prev >= 0)      axpy_cpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            ਜੇ(ਅਗਲਾ < layer.c) axpy_cpu(w*h,  layer.alpha, squared + w*h*ਅਗਲਾ, 1, norms + w*h*k, 1);
        }
    }
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, layer.output, 1);
    mul_cpu(w*h*c*layer.batch, net.ਨਿਵੇਸ਼, 1, layer.output, 1);
}

ਖਾਲੀ backward_normalization_layer(ਅਚਲ layer layer, ਜਾਲ net)
{
    // TODO This is approximate ;-)
    // Also this should add in to delta instead of overwritting.

    ਪੂਰਨ w = layer.w;
    ਪੂਰਨ h = layer.h;
    ਪੂਰਨ c = layer.c;
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, net.delta, 1);
    mul_cpu(w*h*c*layer.batch, layer.delta, 1, net.delta, 1);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_normalization_layer_gpu(ਅਚਲ layer layer, ਜਾਲ net)
{
    ਪੂਰਨ k,b;
    ਪੂਰਨ w = layer.w;
    ਪੂਰਨ h = layer.h;
    ਪੂਰਨ c = layer.c;
    scal_gpu(w*h*c*layer.batch, 0, layer.squared_gpu, 1);

    ਲਈ(b = 0; b < layer.batch; ++b){
        ਦਸ਼ਮਲਵ *squared = layer.squared_gpu + w*h*c*b;
        ਦਸ਼ਮਲਵ *norms   = layer.norms_gpu + w*h*c*b;
        ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼   = net.ਨਿਵੇਸ਼_gpu + w*h*c*b;
        pow_gpu(w*h*c, 2, ਨਿਵੇਸ਼, 1, squared, 1);

        const_gpu(w*h, layer.kappa, norms, 1);
        ਲਈ(k = 0; k < layer.ਮਾਪ/2; ++k){
            axpy_gpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        ਲਈ(k = 1; k < layer.c; ++k){
            copy_gpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            ਪੂਰਨ prev = k - ((layer.ਮਾਪ-1)/2) - 1;
            ਪੂਰਨ ਅਗਲਾ = k + (layer.ਮਾਪ/2);
            ਜੇ(prev >= 0)      axpy_gpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            ਜੇ(ਅਗਲਾ < layer.c) axpy_gpu(w*h,  layer.alpha, squared + w*h*ਅਗਲਾ, 1, norms + w*h*k, 1);
        }
    }
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, layer.output_gpu, 1);
    mul_gpu(w*h*c*layer.batch, net.ਨਿਵੇਸ਼_gpu, 1, layer.output_gpu, 1);
}

ਖਾਲੀ backward_normalization_layer_gpu(ਅਚਲ layer layer, ਜਾਲ net)
{
    // TODO This is approximate ;-)

    ਪੂਰਨ w = layer.w;
    ਪੂਰਨ h = layer.h;
    ਪੂਰਨ c = layer.c;
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, net.delta_gpu, 1);
    mul_gpu(w*h*c*layer.batch, layer.delta_gpu, 1, net.delta_gpu, 1);
}
#ਜੇ_ਅੰਤ
