#समावेश "shortcut_layer.h"
#समावेश "cuda.h"
#समावेश "blas.h"
#समावेश "activations.h"

#समावेश <stdio.h>
#समावेश <assert.h>

layer make_shortcut_layer(पूर्णांक batch, पूर्णांक सूचकांक, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक w2, पूर्णांक h2, पूर्णांक c2)
{
    ख_लिखो(मानक_त्रुटि, "res  %3d                %4d x%4d x%4d   ->  %4d x%4d x%4d\n",सूचकांक, w2,h2,c2, w,h,c);
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

    l.सूचकांक = सूचकांक;

    l.delta =  सुस्मृति(l.outputs*batch, माप(भग्न));
    l.output = सुस्मृति(l.outputs*batch, माप(भग्न));;

    l.forward = forward_shortcut_layer;
    l.backward = backward_shortcut_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_shortcut_layer_gpu;
    l.backward_gpu = backward_shortcut_layer_gpu;

    l.delta_gpu =  cuda_make_array(l.delta, l.outputs*batch);
    l.output_gpu = cuda_make_array(l.output, l.outputs*batch);
    #पूर्ण_अगर
    वापस l;
}

व्योम resize_shortcut_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    निश्चित(l->w == l->out_w);
    निश्चित(l->h == l->out_h);
    l->w = l->out_w = w;
    l->h = l->out_h = h;
    l->outputs = w*h*l->out_c;
    l->inputs = l->outputs;
    l->delta =  पुनः_स्मृति(l->delta, l->outputs*l->batch*माप(भग्न));
    l->output = पुनः_स्मृति(l->output, l->outputs*l->batch*माप(भग्न));

#अगर_घोषित GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, l->outputs*l->batch);
    l->delta_gpu   = cuda_make_array(l->delta,  l->outputs*l->batch);
#पूर्ण_अगर
    
}


व्योम forward_shortcut_layer(स्थिर layer l, network net)
{
    copy_cpu(l.outputs*l.batch, net.निवेश, 1, l.output, 1);
    shortcut_cpu(l.batch, l.w, l.h, l.c, net.layers[l.सूचकांक].output, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output);
    activate_array(l.output, l.outputs*l.batch, l.activation);
}

व्योम backward_shortcut_layer(स्थिर layer l, network net)
{
    gradient_array(l.output, l.outputs*l.batch, l.activation, l.delta);
    axpy_cpu(l.outputs*l.batch, l.alpha, l.delta, 1, net.delta, 1);
    shortcut_cpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta, l.w, l.h, l.c, 1, l.beta, net.layers[l.सूचकांक].delta);
}

#अगर_घोषित GPU
व्योम forward_shortcut_layer_gpu(स्थिर layer l, network net)
{
    copy_gpu(l.outputs*l.batch, net.निवेश_gpu, 1, l.output_gpu, 1);
    shortcut_gpu(l.batch, l.w, l.h, l.c, net.layers[l.सूचकांक].output_gpu, l.out_w, l.out_h, l.out_c, l.alpha, l.beta, l.output_gpu);
    activate_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation);
}

व्योम backward_shortcut_layer_gpu(स्थिर layer l, network net)
{
    gradient_array_gpu(l.output_gpu, l.outputs*l.batch, l.activation, l.delta_gpu);
    axpy_gpu(l.outputs*l.batch, l.alpha, l.delta_gpu, 1, net.delta_gpu, 1);
    shortcut_gpu(l.batch, l.out_w, l.out_h, l.out_c, l.delta_gpu, l.w, l.h, l.c, 1, l.beta, net.layers[l.सूचकांक].delta_gpu);
}
#पूर्ण_अगर
