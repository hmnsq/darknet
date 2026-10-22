#അന്തര്ഭാവ "upsample_layer.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"

#അന്തര്ഭാവ <stdio.h>

layer make_upsample_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ stride)
{
    layer l = {0};
    l.type = UPSAMPLE;
    l.batch = batch;
    l.w = w;
    l.h = h;
    l.c = c;
    l.out_w = w*stride;
    l.out_h = h*stride;
    l.out_c = c;
    എങ്കിൽ(stride < 0){
        stride = -stride;
        l.reverse=1;
        l.out_w = w/stride;
        l.out_h = h/stride;
    }
    l.stride = stride;
    l.outputs = l.out_w*l.out_h*l.out_c;
    l.inputs = l.w*l.h*l.c;
    l.delta =  calloc(l.outputs*batch, വലുപ്പം(ദശാംശം));
    l.output = calloc(l.outputs*batch, വലുപ്പം(ദശാംശം));;

    l.forward = forward_upsample_layer;
    l.backward = backward_upsample_layer;
    #ifdef GPU
    l.forward_gpu = forward_upsample_layer_gpu;
    l.backward_gpu = backward_upsample_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    എങ്കിൽ(l.reverse) fprintf(stderr, "downsample         %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    അല്ലെങ്കിൽ fprintf(stderr, "upsample           %2dx  %4d x%4d x%4d   ->  %4d x%4d x%4d\n", stride, w, h, c, l.out_w, l.out_h, l.out_c);
    തിരികെ l;
}

ശൂന്യം resize_upsample_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    l->w = w;
    l->h = h;
    l->out_w = w*l->stride;
    l->out_h = h*l->stride;
    എങ്കിൽ(l->reverse){
        l->out_w = w/l->stride;
        l->out_h = h/l->stride;
    }
    l->outputs = l->out_w*l->out_h*l->out_c;
    l->inputs = l->h*l->w*l->c;
    l->delta =  realloc(l->delta, l->outputs*l->batch*വലുപ്പം(ദശാംശം));
    l->output = realloc(l->output, l->outputs*l->batch*വലുപ്പം(ദശാംശം));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}

ശൂന്യം forward_upsample_layer(സ്ഥിരം layer l, network net)
{
    fill_cpu(l.outputs*l.batch, 0, l.output, 1);
    എങ്കിൽ(l.reverse){
        upsample_cpu(l.output, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.നിവേശം);
    }അല്ലെങ്കിൽ{
        upsample_cpu(net.നിവേശം, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output);
    }
}

ശൂന്യം backward_upsample_layer(സ്ഥിരം layer l, network net)
{
    എങ്കിൽ(l.reverse){
        upsample_cpu(l.delta, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta);
    }അല്ലെങ്കിൽ{
        upsample_cpu(net.delta, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta);
    }
}

#ifdef GPU
ശൂന്യം forward_upsample_layer_gpu(സ്ഥിരം layer l, network net)
{
    fill_gpu(l.outputs*l.batch, 0, l.output_gpu, 1);
    എങ്കിൽ(l.reverse){
        upsample_gpu(l.output_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 0, l.scale, net.നിവേശം_gpu);
    }അല്ലെങ്കിൽ{
        upsample_gpu(net.നിവേശം_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.scale, l.output_gpu);
    }
}

ശൂന്യം backward_upsample_layer_gpu(സ്ഥിരം layer l, network net)
{
    എങ്കിൽ(l.reverse){
        upsample_gpu(l.delta_gpu, l.out_w, l.out_h, l.c, l.batch, l.stride, 1, l.scale, net.delta_gpu);
    }അല്ലെങ്കിൽ{
        upsample_gpu(net.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.scale, l.delta_gpu);
    }
}
#endif
