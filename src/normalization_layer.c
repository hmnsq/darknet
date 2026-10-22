#ଅନ୍ତର୍ଭାଵ "normalization_layer.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>

layer make_normalization_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ alpha, ଦଶମିକ beta, ଦଶମିକ kappa)
{
    fprintf(stderr, "Local Response Normalization Layer: %d x %d x %d image, %d size\n", w,h,c,size);
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
    layer.output = calloc(h * w * c * batch, ଆକାର(ଦଶମିକ));
    layer.delta = calloc(h * w * c * batch, ଆକାର(ଦଶମିକ));
    layer.squared = calloc(h * w * c * batch, ଆକାର(ଦଶମିକ));
    layer.norms = calloc(h * w * c * batch, ଆକାର(ଦଶମିକ));
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
    ଫେରନ୍ତୁ layer;
}

ଶୂନ୍ୟ resize_normalization_layer(layer *layer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = layer->c;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch = layer->batch;
    layer->h = h;
    layer->w = w;
    layer->out_h = h;
    layer->out_w = w;
    layer->inputs = w*h*c;
    layer->outputs = layer->inputs;
    layer->output = realloc(layer->output, h * w * c * batch * ଆକାର(ଦଶମିକ));
    layer->delta = realloc(layer->delta, h * w * c * batch * ଆକାର(ଦଶମିକ));
    layer->squared = realloc(layer->squared, h * w * c * batch * ଆକାର(ଦଶମିକ));
    layer->norms = realloc(layer->norms, h * w * c * batch * ଆକାର(ଦଶମିକ));
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

ଶୂନ୍ୟ forward_normalization_layer(ଅଚଳ layer layer, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k,b;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = layer.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = layer.h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = layer.c;
    scal_cpu(w*h*c*layer.batch, 0, layer.squared, 1);

    ପାଇଁ(b = 0; b < layer.batch; ++b){
        ଦଶମିକ *squared = layer.squared + w*h*c*b;
        ଦଶମିକ *norms   = layer.norms + w*h*c*b;
        ଦଶମିକ *ନିବେଶ   = net.ନିବେଶ + w*h*c*b;
        pow_cpu(w*h*c, 2, ନିବେଶ, 1, squared, 1);

        const_cpu(w*h, layer.kappa, norms, 1);
        ପାଇଁ(k = 0; k < layer.size/2; ++k){
            axpy_cpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        ପାଇଁ(k = 1; k < layer.c; ++k){
            copy_cpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev = k - ((layer.size-1)/2) - 1;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = k + (layer.size/2);
            ଯଦି(prev >= 0)      axpy_cpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            ଯଦି(next < layer.c) axpy_cpu(w*h,  layer.alpha, squared + w*h*next, 1, norms + w*h*k, 1);
        }
    }
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, layer.output, 1);
    mul_cpu(w*h*c*layer.batch, net.ନିବେଶ, 1, layer.output, 1);
}

ଶୂନ୍ୟ backward_normalization_layer(ଅଚଳ layer layer, network net)
{
    // TODO This is approximate ;-)
    // Also this should add in to delta instead of overwritting.

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = layer.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = layer.h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = layer.c;
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, net.delta, 1);
    mul_cpu(w*h*c*layer.batch, layer.delta, 1, net.delta, 1);
}

#ifdef GPU
ଶୂନ୍ୟ forward_normalization_layer_gpu(ଅଚଳ layer layer, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k,b;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = layer.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = layer.h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = layer.c;
    scal_gpu(w*h*c*layer.batch, 0, layer.squared_gpu, 1);

    ପାଇଁ(b = 0; b < layer.batch; ++b){
        ଦଶମିକ *squared = layer.squared_gpu + w*h*c*b;
        ଦଶମିକ *norms   = layer.norms_gpu + w*h*c*b;
        ଦଶମିକ *ନିବେଶ   = net.ନିବେଶ_gpu + w*h*c*b;
        pow_gpu(w*h*c, 2, ନିବେଶ, 1, squared, 1);

        const_gpu(w*h, layer.kappa, norms, 1);
        ପାଇଁ(k = 0; k < layer.size/2; ++k){
            axpy_gpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        ପାଇଁ(k = 1; k < layer.c; ++k){
            copy_gpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev = k - ((layer.size-1)/2) - 1;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା next = k + (layer.size/2);
            ଯଦି(prev >= 0)      axpy_gpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            ଯଦି(next < layer.c) axpy_gpu(w*h,  layer.alpha, squared + w*h*next, 1, norms + w*h*k, 1);
        }
    }
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, layer.output_gpu, 1);
    mul_gpu(w*h*c*layer.batch, net.ନିବେଶ_gpu, 1, layer.output_gpu, 1);
}

ଶୂନ୍ୟ backward_normalization_layer_gpu(ଅଚଳ layer layer, network net)
{
    // TODO This is approximate ;-)

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = layer.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = layer.h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = layer.c;
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, net.delta_gpu, 1);
    mul_gpu(w*h*c*layer.batch, layer.delta_gpu, 1, net.delta_gpu, 1);
}
#endif
