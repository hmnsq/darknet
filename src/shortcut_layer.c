#অন্তর্ভাব "shortcut_layer.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "activations.h"

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>

layer make_shortcut_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা w2, পূর্ণসংখ্যা h2, পূর্ণসংখ্যা c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",সূচক, w2,h2,c2, w,h,c);
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

    l.সূচক = সূচক;

    l.delta =  calloc(l.outputs*batch, আকার(দশমিক));
    l.output = calloc(l.outputs*batch, আকার(দশমিক));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #ifdef GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #endif
    ফেরত l;
}

শূন্য resize_shortcut_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    প্রতিজ্ঞা(l->w == l->out_w);
    প্রতিজ্ঞা(l->h == l->out_h);
    l->w = l->out_w = w;
    l->h = l->out_h = h;
    l->outputs = w*h*l->out_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*আকার(দশমিক));
    l->output = realloc(l->output, l->outputs*l->batch*আকার(দশমিক));

#ifdef GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#endif
    
}


শূন্য forward_shortcut_layer(ধ্রুবক layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.প্রবেশ, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.সূচক].output, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

শূন্য backward_shortcut_layer(ধ্রুবক layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.সূচক].delta);
}

#ifdef GPU
শূন্য forward_shortcut_layer_gpu(ধ্রুবক layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.প্রবেশ_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.সূচক].output_gpu, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

শূন্য backward_shortcut_layer_gpu(ধ্রুবক layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.সূচক].delta_gpu);
}
#endif
