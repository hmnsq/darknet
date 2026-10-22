#ضمن "shortcut_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "activations.h"

#ضمن <stdio.h>
#ضمن <assert.h>

layer make_shortcut_layer(صحيح batch, صحيح اشاریہ, صحيح w, صحيح h, صحيح c, صحيح w2, صحيح h2, صحيح c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",اشاریہ, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.باہرلا_w = w;
    l.باہرلا_h = h;
    l.باہرلا_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.اشاریہ = اشاریہ;

    l.delta =  صفر_حافظہ_دیو(l.outputs*batch, حجم(عائم));
    l.output = صفر_حافظہ_دیو(l.outputs*batch, حجم(عائم));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #جے_تعریف_شدہ GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #جے_انت
    أرجع l;
}

فراغ resize_shortcut_layer(layer *l, صحيح w, صحيح h)
{
    أكد(l->w == l->باہرلا_w);
    أكد(l->h == l->باہرلا_h);
    l->w = l->باہرلا_w = w;
    l->h = l->باہرلا_h = h;
    l->outputs = w*h*l->باہرلا_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#جے_تعریف_شدہ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#جے_انت
    
}


فراغ forward_shortcut_layer(ثابت layer l, جال net)
{
    copy_cpu(l.outputs*l.batch, net.داخلہ, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.اشاریہ].output, l.باہرلا_w, l.باہرلا_h, l.باہرلا_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

فراغ backward_shortcut_layer(ثابت layer l, جال net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.باہرلا_w, l.باہرلا_h, l.باہرلا_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.اشاریہ].delta);
}

#جے_تعریف_شدہ GPU
فراغ forward_shortcut_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.outputs*l.batch, net.داخلہ_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.اشاریہ].output_gpu, l.باہرلا_w, l.باہرلا_h, l.باہرلا_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

فراغ backward_shortcut_layer_gpu(ثابت layer l, جال net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.باہرلا_w, l.باہرلا_h, l.باہرلا_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.اشاریہ].delta_gpu);
}
#جے_انت
