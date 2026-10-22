#شامل "normalization_layer.h"
#شامل "blas.h"

#شامل <stdio.h>

layer make_normalization_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح سائز, اعشاری alpha, اعشاری beta, اعشاری kappa)
{
    fprintf(stderr, "Local Response Normalization Layer: %d x %d x %d image, %d size\n", w,h,c,سائز);
    layer layer = {0};
    layer.type = NORMALIZATION;
    layer.batch = batch;
    layer.h = layer.برآمد_h = h;
    layer.w = layer.برآمد_w = w;
    layer.c = layer.برآمد_c = c;
    layer.kappa = kappa;
    layer.سائز = سائز;
    layer.alpha = alpha;
    layer.beta = beta;
    layer.output = صفر_حافظہ_دو(h * w * c * batch, حجم(اعشاری));
    layer.delta = صفر_حافظہ_دو(h * w * c * batch, حجم(اعشاری));
    layer.squared = صفر_حافظہ_دو(h * w * c * batch, حجم(اعشاری));
    layer.norms = صفر_حافظہ_دو(h * w * c * batch, حجم(اعشاری));
    layer.inputs = w*h*c;
    layer.outputs = layer.inputs;

    layer.forward = forward_normalization_layer;
    layer.backward = backward_normalization_layer;
    #اگر_تعریف_شدہ GPU
    layer.forward_gpu = forward_normalization_layer_gpu;
    layer.backward_gpu = backward_normalization_layer_gpu;

    layer.output_gpu =  cuda_make_array(layer.output, h * w * c * batch);
    layer.delta_gpu =   cuda_make_array(layer.delta, h * w * c * batch);
    layer.squared_gpu = cuda_make_array(layer.squared, h * w * c * batch);
    layer.norms_gpu =   cuda_make_array(layer.norms, h * w * c * batch);
    #اگر_ختم
    واپس layer;
}

خالی resize_normalization_layer(layer *layer, صحیح w, صحیح h)
{
    صحیح c = layer->c;
    صحیح batch = layer->batch;
    layer->h = h;
    layer->w = w;
    layer->برآمد_h = h;
    layer->برآمد_w = w;
    layer->inputs = w*h*c;
    layer->outputs = layer->inputs;
    layer->output = realloc(layer->output, h * w * c * batch * حجم(اعشاری));
    layer->delta = realloc(layer->delta, h * w * c * batch * حجم(اعشاری));
    layer->squared = realloc(layer->squared, h * w * c * batch * حجم(اعشاری));
    layer->norms = realloc(layer->norms, h * w * c * batch * حجم(اعشاری));
#اگر_تعریف_شدہ GPU
    cuda_free(layer->output_gpu);
    cuda_free(layer->delta_gpu); 
    cuda_free(layer->squared_gpu); 
    cuda_free(layer->norms_gpu);   
    layer->output_gpu =  cuda_make_array(layer->output, h * w * c * batch);
    layer->delta_gpu =   cuda_make_array(layer->delta, h * w * c * batch);
    layer->squared_gpu = cuda_make_array(layer->squared, h * w * c * batch);
    layer->norms_gpu =   cuda_make_array(layer->norms, h * w * c * batch);
#اگر_ختم
}

خالی forward_normalization_layer(ثابت layer layer, جال net)
{
    صحیح k,b;
    صحیح w = layer.w;
    صحیح h = layer.h;
    صحیح c = layer.c;
    scal_cpu(w*h*c*layer.batch, 0, layer.squared, 1);

    برائے(b = 0; b < layer.batch; ++b){
        اعشاری *squared = layer.squared + w*h*c*b;
        اعشاری *norms   = layer.norms + w*h*c*b;
        اعشاری *داخلہ   = net.داخلہ + w*h*c*b;
        pow_cpu(w*h*c, 2, داخلہ, 1, squared, 1);

        const_cpu(w*h, layer.kappa, norms, 1);
        برائے(k = 0; k < layer.سائز/2; ++k){
            axpy_cpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        برائے(k = 1; k < layer.c; ++k){
            copy_cpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            صحیح پچھلا = k - ((layer.سائز-1)/2) - 1;
            صحیح اگلا = k + (layer.سائز/2);
            اگر(پچھلا >= 0)      axpy_cpu(w*h, -layer.alpha, squared + w*h*پچھلا, 1, norms + w*h*k, 1);
            اگر(اگلا < layer.c) axpy_cpu(w*h,  layer.alpha, squared + w*h*اگلا, 1, norms + w*h*k, 1);
        }
    }
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, layer.output, 1);
    mul_cpu(w*h*c*layer.batch, net.داخلہ, 1, layer.output, 1);
}

خالی backward_normalization_layer(ثابت layer layer, جال net)
{
    // TODO This is approximate ;-)
    // Also this should add in to delta instead of overwritting.

    صحیح w = layer.w;
    صحیح h = layer.h;
    صحیح c = layer.c;
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, net.delta, 1);
    mul_cpu(w*h*c*layer.batch, layer.delta, 1, net.delta, 1);
}

#اگر_تعریف_شدہ GPU
خالی forward_normalization_layer_gpu(ثابت layer layer, جال net)
{
    صحیح k,b;
    صحیح w = layer.w;
    صحیح h = layer.h;
    صحیح c = layer.c;
    scal_gpu(w*h*c*layer.batch, 0, layer.squared_gpu, 1);

    برائے(b = 0; b < layer.batch; ++b){
        اعشاری *squared = layer.squared_gpu + w*h*c*b;
        اعشاری *norms   = layer.norms_gpu + w*h*c*b;
        اعشاری *داخلہ   = net.داخلہ_gpu + w*h*c*b;
        pow_gpu(w*h*c, 2, داخلہ, 1, squared, 1);

        const_gpu(w*h, layer.kappa, norms, 1);
        برائے(k = 0; k < layer.سائز/2; ++k){
            axpy_gpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        برائے(k = 1; k < layer.c; ++k){
            copy_gpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            صحیح پچھلا = k - ((layer.سائز-1)/2) - 1;
            صحیح اگلا = k + (layer.سائز/2);
            اگر(پچھلا >= 0)      axpy_gpu(w*h, -layer.alpha, squared + w*h*پچھلا, 1, norms + w*h*k, 1);
            اگر(اگلا < layer.c) axpy_gpu(w*h,  layer.alpha, squared + w*h*اگلا, 1, norms + w*h*k, 1);
        }
    }
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, layer.output_gpu, 1);
    mul_gpu(w*h*c*layer.batch, net.داخلہ_gpu, 1, layer.output_gpu, 1);
}

خالی backward_normalization_layer_gpu(ثابت layer layer, جال net)
{
    // TODO This is approximate ;-)

    صحیح w = layer.w;
    صحیح h = layer.h;
    صحیح c = layer.c;
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, net.delta_gpu, 1);
    mul_gpu(w*h*c*layer.batch, layer.delta_gpu, 1, net.delta_gpu, 1);
}
#اگر_ختم
