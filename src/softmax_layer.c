#અન્તર્ભાવ "softmax_layer.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "cuda.h"

#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <assert.h>

softmax_layer make_softmax_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક groups)
{
    પ્રતિજ્ઞા(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, કદ(દશાંશ));
    l.output = calloc(inputs*batch, કદ(દશાંશ));
    l.delta = calloc(inputs*batch, કદ(દશાંશ));
    l.cost = calloc(1, કદ(દશાંશ));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ifdef GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    પાછા l;
}

શૂન્ય forward_softmax_layer(અચલ softmax_layer l, network net)
{
    જો(l.softmax_વૃક્ષ){
        પૂર્ણાંક i;
        પૂર્ણાંક ગણતરી = 0;
        માટે (i = 0; i < l.softmax_વૃક્ષ->groups; ++i) {
            પૂર્ણાંક group_size = l.softmax_વૃક્ષ->group_size[i];
            softmax_cpu(net.આગત + ગણતરી, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + ગણતરી);
            ગણતરી += group_size;
        }
    } નહીંતર {
        softmax_cpu(net.આગત, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    જો(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

શૂન્ય backward_softmax_layer(અચલ softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

શૂન્ય pull_softmax_layer_output(અચલ softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

શૂન્ય forward_softmax_layer_gpu(અચલ softmax_layer l, network net)
{
    જો(l.softmax_વૃક્ષ){
        softmax_વૃક્ષ(net.આગત_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_વૃક્ષ);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } નહીંતર {
        જો(l.spatial){
            softmax_gpu(net.આગત_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }નહીંતર{
            softmax_gpu(net.આગત_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    જો(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        જો(l.softmax_વૃક્ષ){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_અંક, net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_અંક, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

શૂન્ય backward_softmax_layer_gpu(અચલ softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
