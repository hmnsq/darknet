#ضمن "shortcut_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "activations.h"

#ضمن <stdio.h>
#ضمن <assert.h>

layer make_shortcut_layer(صحيح batch, صحيح شاخص, صحيح w, صحيح h, صحيح c, صحيح w2, صحيح h2, صحيح c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",شاخص, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.بیرون_w = w;
    l.بیرون_h = h;
    l.بیرون_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.شاخص = شاخص;

    l.delta =  حافظه_صفر_بده(l.outputs*batch, حجم(عائم));
    l.output = حافظه_صفر_بده(l.outputs*batch, حجم(عائم));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #پایان_اگر
    أرجع l;
}

فراغ resize_shortcut_layer(layer *l, صحيح w, صحيح h)
{
    أكد(l->w == l->بیرون_w);
    أكد(l->h == l->بیرون_h);
    l->w = l->بیرون_w = w;
    l->h = l->بیرون_h = h;
    l->outputs = w*h*l->بیرون_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#اگر_تعریف_شده GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#پایان_اگر
    
}


فراغ forward_shortcut_layer(ثابت layer l, شبکه net)
{
    copy_cpu(l.outputs*l.batch, net.درون_داد, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.شاخص].output, l.بیرون_w, l.بیرون_h, l.بیرون_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

فراغ backward_shortcut_layer(ثابت layer l, شبکه net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.بیرون_w, l.بیرون_h, l.بیرون_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.شاخص].delta);
}

#اگر_تعریف_شده GPU
فراغ forward_shortcut_layer_gpu(ثابت layer l, شبکه net)
{
    copy_gpu(l.outputs*l.batch, net.درون_داد_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.شاخص].output_gpu, l.بیرون_w, l.بیرون_h, l.بیرون_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

فراغ backward_shortcut_layer_gpu(ثابت layer l, شبکه net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.بیرون_w, l.بیرون_h, l.بیرون_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.شاخص].delta_gpu);
}
#پایان_اگر
