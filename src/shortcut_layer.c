#अन्तर्भाव "shortcut_layer.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "activations.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>

layer make_shortcut_layer(पूर्णाङ्क batch, पूर्णाङ्क सूचकांक, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क w2, पूर्णाङ्क h2, पूर्णाङ्क c2)
{
    fprintf(stderr, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",सूचकांक, w2,h2,c2, w,h,c);
    layer l = {0};
    l.type = SHORTCUT;
    l.batch = batch;
    l.w = w2;
    l.h = h2;
    l.c = c2;
    l.बहिर्गमनम्_w = w;
    l.बहिर्गमनम्_h = h;
    l.बहिर्गमनम्_c = c;
    l.outputs = w*h*c;
    l.inputs = l.outputs;

    l.सूचकांक = सूचकांक;

    l.delta =  शून्यस्मृतिं_देहि(l.outputs*batch, परिमाण(भिन्न));
    l.output = शून्यस्मृतिं_देहि(l.outputs*batch, परिमाण(भिन्न));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #यदि_परिभाषितम् GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #यदि_अन्तः
    प्रत्यागम l;
}

शून्य resize_shortcut_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h)
{
    प्रतिज्ञा(l->w == l->बहिर्गमनम्_w);
    प्रतिज्ञा(l->h == l->बहिर्गमनम्_h);
    l->w = l->बहिर्गमनम्_w = w;
    l->h = l->बहिर्गमनम्_h = h;
    l->outputs = w*h*l->बहिर्गमनम्_c;
    l->inputs = l->outputs;
    l->delta =  realloc(l->delta, l->outputs*l->batch*परिमाण(भिन्न));
    l->output = realloc(l->output, l->outputs*l->batch*परिमाण(भिन्न));

#यदि_परिभाषितम् GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#यदि_अन्तः
    
}


शून्य forward_shortcut_layer(ध्रुव layer l, जालम् net)
{
    copy_cpu(l.outputs*l.batch, net.निवेशः, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.सूचकांक].output, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

शून्य backward_shortcut_layer(ध्रुव layer l, जालम् net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.सूचकांक].delta);
}

#यदि_परिभाषितम् GPU
शून्य forward_shortcut_layer_gpu(ध्रुव layer l, जालम् net)
{
    copy_gpu(l.outputs*l.batch, net.निवेशः_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.सूचकांक].output_gpu, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

शून्य backward_shortcut_layer_gpu(ध्रुव layer l, जालम् net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.बहिर्गमनम्_w, l.बहिर्गमनम्_h, l.बहिर्गमनम्_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.सूचकांक].delta_gpu);
}
#यदि_अन्तः
