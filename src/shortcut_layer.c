#شامل "shortcut_layer.h"
#شامل "cuda.h"
#شامل "blas.h"
#شامل "activations.h"

#شامل <stdio.h>
#شامل <assert.h>

layer make_shortcut_layer(صحیح batch, صحیح اشاریہ, صحیح w, صحیح h, صحیح c, صحیح w2, صحیح h2, صحیح c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",اشاریہ, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.برآمد_w = w;
    l.برآمد_h = h;
    l.برآمد_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.اشاریہ = اشاریہ;

    l.delta =  صفر_حافظہ_دو(l.outputs*batch, حجم(اعشاری));
    l.output = صفر_حافظہ_دو(l.outputs*batch, حجم(اعشاری));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #اگر_ختم
    واپس l;
}

خالی resize_shortcut_layer(layer *l, صحیح w, صحیح h)
{
    أكد(l->w == l->برآمد_w);
    أكد(l->h == l->برآمد_h);
    l->w = l->برآمد_w = w;
    l->h = l->برآمد_h = h;
    l->outputs = w*h*l->برآمد_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(اعشاری));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(اعشاری));

#اگر_تعریف_شدہ GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#اگر_ختم
    
}


خالی forward_shortcut_layer(ثابت layer l, جال net)
{
    copy_cpu(l.outputs*l.batch, net.داخلہ, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.اشاریہ].output, l.برآمد_w, l.برآمد_h, l.برآمد_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

خالی backward_shortcut_layer(ثابت layer l, جال net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.برآمد_w, l.برآمد_h, l.برآمد_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.اشاریہ].delta);
}

#اگر_تعریف_شدہ GPU
خالی forward_shortcut_layer_gpu(ثابت layer l, جال net)
{
    copy_gpu(l.outputs*l.batch, net.داخلہ_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.اشاریہ].output_gpu, l.برآمد_w, l.برآمد_h, l.برآمد_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

خالی backward_shortcut_layer_gpu(ثابت layer l, جال net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.برآمد_w, l.برآمد_h, l.برآمد_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.اشاریہ].delta_gpu);
}
#اگر_ختم
