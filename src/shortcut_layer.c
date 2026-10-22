#അന്തര്ഭാവ "shortcut_layer.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "activations.h"

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>

layer make_shortcut_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ w2, പൂർണ്ണസംഖ്യ h2, പൂർണ്ണസംഖ്യ c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",index, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.out_w = w;
    l.out_h = h;
    l.out_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.index = index;

    l.delta =  calloc(l.outputs*batch, വലുപ്പം(ദശാംശം));
    l.output = calloc(l.outputs*batch, വലുപ്പം(ദശാംശം));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #ifdef GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    തിരികെ l;
}

ശൂന്യം resize_shortcut_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പ്രതിജ്ഞാ(l->w == l->out_w);
    പ്രതിജ്ഞാ(l->h == l->out_h);
    l->w = l->out_w = w;
    l->h = l->out_h = h;
    l->outputs = w*h*l->out_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*വലുപ്പം(ദശാംശം));
    l->output = realloc(l->output, l->outputs*l->batch*വലുപ്പം(ദശാംശം));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}


ശൂന്യം forward_shortcut_layer(സ്ഥിരം layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.നിവേശം, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.index].output, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ശൂന്യം backward_shortcut_layer(സ്ഥിരം layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.index].delta);
}

#ifdef GPU
ശൂന്യം forward_shortcut_layer_gpu(സ്ഥിരം layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.നിവേശം_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.index].output_gpu, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ശൂന്യം backward_shortcut_layer_gpu(സ്ഥിരം layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.index].delta_gpu);
}
#endif
