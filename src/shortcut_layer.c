#ਅਨ੍ਤਰ੍ਭਾਵ "shortcut_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

layer make_shortcut_layer(ਪੂਰਨ batch, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ w2, ਪੂਰਨ h2, ਪੂਰਨ c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",ਸੂਚਕਾਂਕ, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.ਬਾਹਰਲਾ_w = w;
    l.ਬਾਹਰਲਾ_h = h;
    l.ਬਾਹਰਲਾ_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.ਸੂਚਕਾਂਕ = ਸੂਚਕਾਂਕ;

    l.delta =  ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.output = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(l.outputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #ਜੇ_ਅੰਤ
    ਵਾਪਸ l;
}

ਖਾਲੀ resize_shortcut_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(l->w == l->ਬਾਹਰਲਾ_w);
    ਪ੍ਰਤਿਜ੍ਞਾ(l->h == l->ਬਾਹਰਲਾ_h);
    l->w = l->ਬਾਹਰਲਾ_w = w;
    l->h = l->ਬਾਹਰਲਾ_h = h;
    l->outputs = w*h*l->ਬਾਹਰਲਾ_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l->output = realloc(l->output, l->outputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#ਜੇ_ਅੰਤ
    
}


ਖਾਲੀ forward_shortcut_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_cpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.ਸੂਚਕਾਂਕ].output, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

ਖਾਲੀ backward_shortcut_layer(ਅਚਲ layer l, ਜਾਲ net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.ਸੂਚਕਾਂਕ].delta);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_shortcut_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    copy_gpu(l.outputs*l.batch, net.ਨਿਵੇਸ਼_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.ਸੂਚਕਾਂਕ].output_gpu, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

ਖਾਲੀ backward_shortcut_layer_gpu(ਅਚਲ layer l, ਜਾਲ net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.ਬਾਹਰਲਾ_w, l.ਬਾਹਰਲਾ_h, l.ਬਾਹਰਲਾ_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.ਸੂਚਕਾਂਕ].delta_gpu);
}
#ਜੇ_ਅੰਤ
