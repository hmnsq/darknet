#ضمن "shortcut_layer.h"
#ضمن "cuda.h"
#ضمن "blas.h"
#ضمن "activations.h"

#ضمن <stdio.h>
#ضمن <assert.h>

layer make_shortcut_layer(صحيح batch, صحيح اشاريو, صحيح w, صحيح h, صحيح c, صحيح w2, صحيح h2, صحيح c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",اشاريو, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.ٻاهريون_w = w;
    l.ٻاهريون_h = h;
    l.ٻاهريون_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.اشاريو = اشاريو;

    l.delta =  صفر_يادگيري_ڏيو(l.outputs*batch, حجم(عائم));
    l.output = صفر_يادگيري_ڏيو(l.outputs*batch, حجم(عائم));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #جيڪڏهن_وصف_ٿيل GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #جيڪڏهن_پورو
    أرجع l;
}

فراغ resize_shortcut_layer(layer *l, صحيح w, صحيح h)
{
    أكد(l->w == l->ٻاهريون_w);
    أكد(l->h == l->ٻاهريون_h);
    l->w = l->ٻاهريون_w = w;
    l->h = l->ٻاهريون_h = h;
    l->outputs = w*h*l->ٻاهريون_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*حجم(عائم));
    l->output = realloc(l->output, l->outputs*l->batch*حجم(عائم));

#جيڪڏهن_وصف_ٿيل GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#جيڪڏهن_پورو
    
}


فراغ forward_shortcut_layer(ثابت layer l, ڄار net)
{
    copy_cpu(l.outputs*l.batch, net.داخلا, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.اشاريو].output, l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

فراغ backward_shortcut_layer(ثابت layer l, ڄار net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.اشاريو].delta);
}

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_shortcut_layer_gpu(ثابت layer l, ڄار net)
{
    copy_gpu(l.outputs*l.batch, net.داخلا_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.اشاريو].output_gpu, l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

فراغ backward_shortcut_layer_gpu(ثابت layer l, ڄار net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.ٻاهريون_w, l.ٻاهريون_h, l.ٻاهريون_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.اشاريو].delta_gpu);
}
#جيڪڏهن_پورو
