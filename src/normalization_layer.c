#ضمن "normalization_layer.h"
#ضمن "blas.h"

#ضمن <stdio.h>

layer make_normalization_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح سائيز, عائم alpha, عائم beta, عائم kappa)
{
    fprintf(stderr, "Local Response Normalization Layer: %d x %d x %d image, %d size\n", w,h,c,سائيز);
    layer layer = {0};
    layer.type = NORMALIZATION;
    layer.batch = batch;
    layer.h = layer.ٻاهريون_h = h;
    layer.w = layer.ٻاهريون_w = w;
    layer.c = layer.ٻاهريون_c = c;
    layer.kappa = kappa;
    layer.سائيز = سائيز;
    layer.alpha = alpha;
    layer.beta = beta;
    layer.output = صفر_يادگيري_ڏيو(h * w * c * batch, حجم(عائم));
    layer.delta = صفر_يادگيري_ڏيو(h * w * c * batch, حجم(عائم));
    layer.squared = صفر_يادگيري_ڏيو(h * w * c * batch, حجم(عائم));
    layer.norms = صفر_يادگيري_ڏيو(h * w * c * batch, حجم(عائم));
    layer.inputs = w*h*c;
    layer.outputs = layer.inputs;

    layer.forward = forward_normalization_layer;
    layer.backward = backward_normalization_layer;
    #جيڪڏهن_وصف_ٿيل GPU
    layer.forward_gpu = forward_normalization_layer_gpu;
    layer.backward_gpu = backward_normalization_layer_gpu;

    layer.output_gpu =  cuda_make_array(layer.output, h * w * c * batch);
    layer.delta_gpu =   cuda_make_array(layer.delta, h * w * c * batch);
    layer.squared_gpu = cuda_make_array(layer.squared, h * w * c * batch);
    layer.norms_gpu =   cuda_make_array(layer.norms, h * w * c * batch);
    #جيڪڏهن_پورو
    أرجع layer;
}

فراغ resize_normalization_layer(layer *layer, صحيح w, صحيح h)
{
    صحيح c = layer->c;
    صحيح batch = layer->batch;
    layer->h = h;
    layer->w = w;
    layer->ٻاهريون_h = h;
    layer->ٻاهريون_w = w;
    layer->inputs = w*h*c;
    layer->outputs = layer->inputs;
    layer->output = realloc(layer->output, h * w * c * batch * حجم(عائم));
    layer->delta = realloc(layer->delta, h * w * c * batch * حجم(عائم));
    layer->squared = realloc(layer->squared, h * w * c * batch * حجم(عائم));
    layer->norms = realloc(layer->norms, h * w * c * batch * حجم(عائم));
#جيڪڏهن_وصف_ٿيل GPU
    cuda_free(layer->output_gpu);
    cuda_free(layer->delta_gpu); 
    cuda_free(layer->squared_gpu); 
    cuda_free(layer->norms_gpu);   
    layer->output_gpu =  cuda_make_array(layer->output, h * w * c * batch);
    layer->delta_gpu =   cuda_make_array(layer->delta, h * w * c * batch);
    layer->squared_gpu = cuda_make_array(layer->squared, h * w * c * batch);
    layer->norms_gpu =   cuda_make_array(layer->norms, h * w * c * batch);
#جيڪڏهن_پورو
}

فراغ forward_normalization_layer(ثابت layer layer, ڄار net)
{
    صحيح k,b;
    صحيح w = layer.w;
    صحيح h = layer.h;
    صحيح c = layer.c;
    scal_cpu(w*h*c*layer.batch, 0, layer.squared, 1);

    لكل(b = 0; b < layer.batch; ++b){
        عائم *squared = layer.squared + w*h*c*b;
        عائم *norms   = layer.norms + w*h*c*b;
        عائم *داخلا   = net.داخلا + w*h*c*b;
        pow_cpu(w*h*c, 2, داخلا, 1, squared, 1);

        const_cpu(w*h, layer.kappa, norms, 1);
        لكل(k = 0; k < layer.سائيز/2; ++k){
            axpy_cpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        لكل(k = 1; k < layer.c; ++k){
            copy_cpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            صحيح prev = k - ((layer.سائيز-1)/2) - 1;
            صحيح ايندڙ = k + (layer.سائيز/2);
            إذا(prev >= 0)      axpy_cpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            إذا(ايندڙ < layer.c) axpy_cpu(w*h,  layer.alpha, squared + w*h*ايندڙ, 1, norms + w*h*k, 1);
        }
    }
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, layer.output, 1);
    mul_cpu(w*h*c*layer.batch, net.داخلا, 1, layer.output, 1);
}

فراغ backward_normalization_layer(ثابت layer layer, ڄار net)
{
    // TODO This is approximate ;-)
    // Also this should add in to delta instead of overwritting.

    صحيح w = layer.w;
    صحيح h = layer.h;
    صحيح c = layer.c;
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, net.delta, 1);
    mul_cpu(w*h*c*layer.batch, layer.delta, 1, net.delta, 1);
}

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_normalization_layer_gpu(ثابت layer layer, ڄار net)
{
    صحيح k,b;
    صحيح w = layer.w;
    صحيح h = layer.h;
    صحيح c = layer.c;
    scal_gpu(w*h*c*layer.batch, 0, layer.squared_gpu, 1);

    لكل(b = 0; b < layer.batch; ++b){
        عائم *squared = layer.squared_gpu + w*h*c*b;
        عائم *norms   = layer.norms_gpu + w*h*c*b;
        عائم *داخلا   = net.داخلا_gpu + w*h*c*b;
        pow_gpu(w*h*c, 2, داخلا, 1, squared, 1);

        const_gpu(w*h, layer.kappa, norms, 1);
        لكل(k = 0; k < layer.سائيز/2; ++k){
            axpy_gpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        لكل(k = 1; k < layer.c; ++k){
            copy_gpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            صحيح prev = k - ((layer.سائيز-1)/2) - 1;
            صحيح ايندڙ = k + (layer.سائيز/2);
            إذا(prev >= 0)      axpy_gpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            إذا(ايندڙ < layer.c) axpy_gpu(w*h,  layer.alpha, squared + w*h*ايندڙ, 1, norms + w*h*k, 1);
        }
    }
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, layer.output_gpu, 1);
    mul_gpu(w*h*c*layer.batch, net.داخلا_gpu, 1, layer.output_gpu, 1);
}

فراغ backward_normalization_layer_gpu(ثابت layer layer, ڄار net)
{
    // TODO This is approximate ;-)

    صحيح w = layer.w;
    صحيح h = layer.h;
    صحيح c = layer.c;
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, net.delta_gpu, 1);
    mul_gpu(w*h*c*layer.batch, layer.delta_gpu, 1, net.delta_gpu, 1);
}
#جيڪڏهن_پورو
