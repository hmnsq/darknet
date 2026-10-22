#அந்தர்பாவ "shortcut_layer.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "activations.h"

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>

layer make_shortcut_layer(முழு batch, முழு index, முழு w, முழு h, முழு c, முழு w2, முழு h2, முழு c2)
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

    l.delta =  calloc(l.outputs*batch, அளவு(தசம));
    l.output = calloc(l.outputs*batch, அளவு(தசம));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #ifdef GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    திரும்பு l;
}

வெறுமை resize_shortcut_layer(layer *l, முழு w, முழு h)
{
    ப்ரதிஜ்ஞா(l->w == l->out_w);
    ப்ரதிஜ்ஞா(l->h == l->out_h);
    l->w = l->out_w = w;
    l->h = l->out_h = h;
    l->outputs = w*h*l->out_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*அளவு(தசம));
    l->output = realloc(l->output, l->outputs*l->batch*அளவு(தசம));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}


வெறுமை forward_shortcut_layer(மாறிலி layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.உள்ளீடு, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.index].output, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

வெறுமை backward_shortcut_layer(மாறிலி layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.index].delta);
}

#ifdef GPU
வெறுமை forward_shortcut_layer_gpu(மாறிலி layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.உள்ளீடு_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.index].output_gpu, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

வெறுமை backward_shortcut_layer_gpu(மாறிலி layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.index].delta_gpu);
}
#endif
