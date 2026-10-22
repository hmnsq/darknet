#અન્તર્ભાવ "shortcut_layer.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "activations.h"

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>

layer make_shortcut_layer(પૂર્ણાંક batch, પૂર્ણાંક index, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, પૂર્ણાંક w2, પૂર્ણાંક h2, પૂર્ણાંક c2)
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

    l.delta =  calloc(l.outputs*batch, કદ(દશાંશ));
    l.output = calloc(l.outputs*batch, કદ(દશાંશ));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #ifdef GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    પાછા l;
}

શૂન્ય resize_shortcut_layer(layer *l, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પ્રતિજ્ઞા(l->w == l->out_w);
    પ્રતિજ્ઞા(l->h == l->out_h);
    l->w = l->out_w = w;
    l->h = l->out_h = h;
    l->outputs = w*h*l->out_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*કદ(દશાંશ));
    l->output = realloc(l->output, l->outputs*l->batch*કદ(દશાંશ));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}


શૂન્ય forward_shortcut_layer(અચલ layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.આગત, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.index].output, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

શૂન્ય backward_shortcut_layer(અચલ layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.index].delta);
}

#ifdef GPU
શૂન્ય forward_shortcut_layer_gpu(અચલ layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.આગત_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.index].output_gpu, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

શૂન્ય backward_shortcut_layer_gpu(અચલ layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.index].delta_gpu);
}
#endif
