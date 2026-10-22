#समावेश "reorg_layer.h"
#समावेश "cuda.h"
#समावेश "blas.h"

#समावेश <stdio.h>


layer make_reorg_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक stride, पूर्णांक reverse, पूर्णांक flatten, पूर्णांक extra)
{
    layer l = {0};
    l.type = REORG;
    l.batch = batch;
    l.stride = stride;
    l.extra = extra;
    l.h = h;
    l.w = w;
    l.c = c;
    l.flatten = flatten;
    अगर(reverse){
        l.out_w = w*stride;
        l.out_h = h*stride;
        l.out_c = c/(stride*stride);
    }अन्यथा{
        l.out_w = w/stride;
        l.out_h = h/stride;
        l.out_c = c*(stride*stride);
    }
    l.reverse = reverse;

    l.outputs = l.out_h * l.out_w * l.out_c;
    l.inputs = h*w*c;
    अगर(l.extra){
        l.out_w = l.out_h = l.out_c = 0;
        l.outputs = l.inputs + l.extra;
    }

    अगर(extra){
        ख_लिखो(मानक_त्रुटि, "reorg              %4d   ->  %4d\n",  l.inputs, l.outputs);
    } अन्यथा {
        ख_लिखो(मानक_त्रुटि, "reorg              /%2d  %4d x%4d x%4d   ->  %4d x%4d x%4d\n",  stride, w, h, c, l.out_w, l.out_h, l.out_c);
    }
    पूर्णांक output_आकार = l.outputs * batch;
    l.output =  सुस्मृति(output_आकार, माप(भग्न));
    l.delta =   सुस्मृति(output_आकार, माप(भग्न));

    l.forward = forward_reorg_layer;
    l.backward = backward_reorg_layer;
#अगर_घोषित GPU
    l.forward_gpu = forward_reorg_layer_gpu;
    l.backward_gpu = backward_reorg_layer_gpu;

    l.output_gpu  = cuda_make_array(l.output, output_आकार);
    l.delta_gpu   = cuda_make_array(l.delta, output_आकार);
#पूर्ण_अगर
    वापस l;
}

व्योम resize_reorg_layer(layer *l, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक stride = l->stride;
    पूर्णांक c = l->c;

    l->h = h;
    l->w = w;

    अगर(l->reverse){
        l->out_w = w*stride;
        l->out_h = h*stride;
        l->out_c = c/(stride*stride);
    }अन्यथा{
        l->out_w = w/stride;
        l->out_h = h/stride;
        l->out_c = c*(stride*stride);
    }

    l->outputs = l->out_h * l->out_w * l->out_c;
    l->inputs = l->outputs;
    पूर्णांक output_आकार = l->outputs * l->batch;

    l->output = पुनः_स्मृति(l->output, output_आकार * माप(भग्न));
    l->delta = पुनः_स्मृति(l->delta, output_आकार * माप(भग्न));

#अगर_घोषित GPU
    cuda_free(l->output_gpu);
    cuda_free(l->delta_gpu);
    l->output_gpu  = cuda_make_array(l->output, output_आकार);
    l->delta_gpu   = cuda_make_array(l->delta,  output_आकार);
#पूर्ण_अगर
}

व्योम forward_reorg_layer(स्थिर layer l, network net)
{
    पूर्णांक i;
    अगर(l.flatten){
        स_नकल(l.output, net.निवेश, l.outputs*l.batch*माप(भग्न));
        अगर(l.reverse){
            flatten(l.output, l.w*l.h, l.c, l.batch, 0);
        }अन्यथा{
            flatten(l.output, l.w*l.h, l.c, l.batch, 1);
        }
    } अन्यथा अगर (l.extra) {
        क्रम(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, net.निवेश + i*l.inputs, 1, l.output + i*l.outputs, 1);
        }
    } अन्यथा अगर (l.reverse){
        reorg_cpu(net.निवेश, l.w, l.h, l.c, l.batch, l.stride, 1, l.output);
    } अन्यथा {
        reorg_cpu(net.निवेश, l.w, l.h, l.c, l.batch, l.stride, 0, l.output);
    }
}

व्योम backward_reorg_layer(स्थिर layer l, network net)
{
    पूर्णांक i;
    अगर(l.flatten){
        स_नकल(net.delta, l.delta, l.outputs*l.batch*माप(भग्न));
        अगर(l.reverse){
            flatten(net.delta, l.w*l.h, l.c, l.batch, 1);
        }अन्यथा{
            flatten(net.delta, l.w*l.h, l.c, l.batch, 0);
        }
    } अन्यथा अगर(l.reverse){
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta);
    } अन्यथा अगर (l.extra) {
        क्रम(i = 0; i < l.batch; ++i){
            copy_cpu(l.inputs, l.delta + i*l.outputs, 1, net.delta + i*l.inputs, 1);
        }
    }अन्यथा{
        reorg_cpu(l.delta, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta);
    }
}

#अगर_घोषित GPU
व्योम forward_reorg_layer_gpu(layer l, network net)
{
    पूर्णांक i;
    अगर(l.flatten){
        अगर(l.reverse){
            flatten_gpu(net.निवेश_gpu, l.w*l.h, l.c, l.batch, 0, l.output_gpu);
        }अन्यथा{
            flatten_gpu(net.निवेश_gpu, l.w*l.h, l.c, l.batch, 1, l.output_gpu);
        }
    } अन्यथा अगर (l.extra) {
        क्रम(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, net.निवेश_gpu + i*l.inputs, 1, l.output_gpu + i*l.outputs, 1);
        }
    } अन्यथा अगर (l.reverse) {
        reorg_gpu(net.निवेश_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, l.output_gpu);
    }अन्यथा {
        reorg_gpu(net.निवेश_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, l.output_gpu);
    }
}

व्योम backward_reorg_layer_gpu(layer l, network net)
{
    अगर(l.flatten){
        अगर(l.reverse){
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 1, net.delta_gpu);
        }अन्यथा{
            flatten_gpu(l.delta_gpu, l.w*l.h, l.c, l.batch, 0, net.delta_gpu);
        }
    } अन्यथा अगर (l.extra) {
        पूर्णांक i;
        क्रम(i = 0; i < l.batch; ++i){
            copy_gpu(l.inputs, l.delta_gpu + i*l.outputs, 1, net.delta_gpu + i*l.inputs, 1);
        }
    } अन्यथा अगर(l.reverse){
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 0, net.delta_gpu);
    } अन्यथा {
        reorg_gpu(l.delta_gpu, l.w, l.h, l.c, l.batch, l.stride, 1, net.delta_gpu);
    }
}
#पूर्ण_अगर
