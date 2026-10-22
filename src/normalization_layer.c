#అన్తర్భావ "normalization_layer.h"
#అన్తర్భావ "blas.h"

#అన్తర్భావ <stdio.h>

layer make_normalization_layer(పూర్ణాంకం batch, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, పూర్ణాంకం కొలత, దశాంశం alpha, దశాంశం beta, దశాంశం kappa)
{
    fprintf(stderr, "Local Response Normalization Layer: %d x %d x %d image, %d size\n", w,h,c,కొలత);
    layer layer = {0};
    layer.type = NORMALIZATION;
    layer.batch = batch;
    layer.h = layer.బహిర్గతం_h = h;
    layer.w = layer.బహిర్గతం_w = w;
    layer.c = layer.బహిర్గతం_c = c;
    layer.kappa = kappa;
    layer.కొలత = కొలత;
    layer.alpha = alpha;
    layer.beta = beta;
    layer.output = సున్నా_జ్ఞాపకం_ఇవ్వు(h * w * c * batch, పరిమాణం(దశాంశం));
    layer.delta = సున్నా_జ్ఞాపకం_ఇవ్వు(h * w * c * batch, పరిమాణం(దశాంశం));
    layer.squared = సున్నా_జ్ఞాపకం_ఇవ్వు(h * w * c * batch, పరిమాణం(దశాంశం));
    layer.norms = సున్నా_జ్ఞాపకం_ఇవ్వు(h * w * c * batch, పరిమాణం(దశాంశం));
    layer.inputs = w*h*c;
    layer.outputs = layer.inputs;

    layer.forward = forward_normalization_layer;
    layer.backward = backward_normalization_layer;
    #నిర్వచితమైతే GPU
    layer.forward_gpu = forward_normalization_layer_gpu;
    layer.backward_gpu = backward_normalization_layer_gpu;

    layer.output_gpu =  cuda_make_array(layer.output, h * w * c * batch);
    layer.delta_gpu =   cuda_make_array(layer.delta, h * w * c * batch);
    layer.squared_gpu = cuda_make_array(layer.squared, h * w * c * batch);
    layer.norms_gpu =   cuda_make_array(layer.norms, h * w * c * batch);
    #అయితే_ముగింపు
    తిరిగి layer;
}

శూన్య resize_normalization_layer(layer *layer, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం c = layer->c;
    పూర్ణాంకం batch = layer->batch;
    layer->h = h;
    layer->w = w;
    layer->బహిర్గతం_h = h;
    layer->బహిర్గతం_w = w;
    layer->inputs = w*h*c;
    layer->outputs = layer->inputs;
    layer->output = realloc(layer->output, h * w * c * batch * పరిమాణం(దశాంశం));
    layer->delta = realloc(layer->delta, h * w * c * batch * పరిమాణం(దశాంశం));
    layer->squared = realloc(layer->squared, h * w * c * batch * పరిమాణం(దశాంశం));
    layer->norms = realloc(layer->norms, h * w * c * batch * పరిమాణం(దశాంశం));
#నిర్వచితమైతే GPU
    cuda_free(layer->output_gpu);
    cuda_free(layer->delta_gpu); 
    cuda_free(layer->squared_gpu); 
    cuda_free(layer->norms_gpu);   
    layer->output_gpu =  cuda_make_array(layer->output, h * w * c * batch);
    layer->delta_gpu =   cuda_make_array(layer->delta, h * w * c * batch);
    layer->squared_gpu = cuda_make_array(layer->squared, h * w * c * batch);
    layer->norms_gpu =   cuda_make_array(layer->norms, h * w * c * batch);
#అయితే_ముగింపు
}

శూన్య forward_normalization_layer(స్థిరరాశి layer layer, జాలం net)
{
    పూర్ణాంకం k,b;
    పూర్ణాంకం w = layer.w;
    పూర్ణాంకం h = layer.h;
    పూర్ణాంకం c = layer.c;
    scal_cpu(w*h*c*layer.batch, 0, layer.squared, 1);

    కొరకు(b = 0; b < layer.batch; ++b){
        దశాంశం *squared = layer.squared + w*h*c*b;
        దశాంశం *norms   = layer.norms + w*h*c*b;
        దశాంశం *ఆదానం   = net.ఆదానం + w*h*c*b;
        pow_cpu(w*h*c, 2, ఆదానం, 1, squared, 1);

        const_cpu(w*h, layer.kappa, norms, 1);
        కొరకు(k = 0; k < layer.కొలత/2; ++k){
            axpy_cpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        కొరకు(k = 1; k < layer.c; ++k){
            copy_cpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            పూర్ణాంకం prev = k - ((layer.కొలత-1)/2) - 1;
            పూర్ణాంకం తదుపరి = k + (layer.కొలత/2);
            ఒకవేళ(prev >= 0)      axpy_cpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            ఒకవేళ(తదుపరి < layer.c) axpy_cpu(w*h,  layer.alpha, squared + w*h*తదుపరి, 1, norms + w*h*k, 1);
        }
    }
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, layer.output, 1);
    mul_cpu(w*h*c*layer.batch, net.ఆదానం, 1, layer.output, 1);
}

శూన్య backward_normalization_layer(స్థిరరాశి layer layer, జాలం net)
{
    // TODO This is approximate ;-)
    // Also this should add in to delta instead of overwritting.

    పూర్ణాంకం w = layer.w;
    పూర్ణాంకం h = layer.h;
    పూర్ణాంకం c = layer.c;
    pow_cpu(w*h*c*layer.batch, -layer.beta, layer.norms, 1, net.delta, 1);
    mul_cpu(w*h*c*layer.batch, layer.delta, 1, net.delta, 1);
}

#నిర్వచితమైతే GPU
శూన్య forward_normalization_layer_gpu(స్థిరరాశి layer layer, జాలం net)
{
    పూర్ణాంకం k,b;
    పూర్ణాంకం w = layer.w;
    పూర్ణాంకం h = layer.h;
    పూర్ణాంకం c = layer.c;
    scal_gpu(w*h*c*layer.batch, 0, layer.squared_gpu, 1);

    కొరకు(b = 0; b < layer.batch; ++b){
        దశాంశం *squared = layer.squared_gpu + w*h*c*b;
        దశాంశం *norms   = layer.norms_gpu + w*h*c*b;
        దశాంశం *ఆదానం   = net.ఆదానం_gpu + w*h*c*b;
        pow_gpu(w*h*c, 2, ఆదానం, 1, squared, 1);

        const_gpu(w*h, layer.kappa, norms, 1);
        కొరకు(k = 0; k < layer.కొలత/2; ++k){
            axpy_gpu(w*h, layer.alpha, squared + w*h*k, 1, norms, 1);
        }

        కొరకు(k = 1; k < layer.c; ++k){
            copy_gpu(w*h, norms + w*h*(k-1), 1, norms + w*h*k, 1);
            పూర్ణాంకం prev = k - ((layer.కొలత-1)/2) - 1;
            పూర్ణాంకం తదుపరి = k + (layer.కొలత/2);
            ఒకవేళ(prev >= 0)      axpy_gpu(w*h, -layer.alpha, squared + w*h*prev, 1, norms + w*h*k, 1);
            ఒకవేళ(తదుపరి < layer.c) axpy_gpu(w*h,  layer.alpha, squared + w*h*తదుపరి, 1, norms + w*h*k, 1);
        }
    }
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, layer.output_gpu, 1);
    mul_gpu(w*h*c*layer.batch, net.ఆదానం_gpu, 1, layer.output_gpu, 1);
}

శూన్య backward_normalization_layer_gpu(స్థిరరాశి layer layer, జాలం net)
{
    // TODO This is approximate ;-)

    పూర్ణాంకం w = layer.w;
    పూర్ణాంకం h = layer.h;
    పూర్ణాంకం c = layer.c;
    pow_gpu(w*h*c*layer.batch, -layer.beta, layer.norms_gpu, 1, net.delta_gpu, 1);
    mul_gpu(w*h*c*layer.batch, layer.delta_gpu, 1, net.delta_gpu, 1);
}
#అయితే_ముగింపు
