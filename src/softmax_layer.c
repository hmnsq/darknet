#ضمن "softmax_layer.h"
#ضمن "blas.h"
#ضمن "cuda.h"

#ضمن <float.h>
#ضمن <math.h>
#ضمن <stdlib.h>
#ضمن <stdio.h>
#ضمن <assert.h>

softmax_layer make_softmax_layer(صحيح batch, صحيح inputs, صحيح groups)
{
    أكد(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, حجم(عائم));
    l.output = calloc(inputs*batch, حجم(عائم));
    l.delta = calloc(inputs*batch, حجم(عائم));
    l.cost = calloc(1, حجم(عائم));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ifdef GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    ܦܢܐ l;
}

فراغ forward_softmax_layer(ثابت softmax_layer l, network net)
{
    ܐܢ(l.softmax_tree){
        صحيح i;
        صحيح count = 0;
        لكل (i = 0; i < l.softmax_tree->groups; ++i) {
            صحيح group_size = l.softmax_tree->group_size[i];
            softmax_cpu(net.input + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + count);
            count += group_size;
        }
    } ܐܚܪܢܐ {
        softmax_cpu(net.input, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    ܐܢ(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

فراغ backward_softmax_layer(ثابت softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

فراغ pull_softmax_layer_output(ثابت softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

فراغ forward_softmax_layer_gpu(ثابت softmax_layer l, network net)
{
    ܐܢ(l.softmax_tree){
        softmax_tree(net.input_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_tree);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } ܐܚܪܢܐ {
        ܐܢ(l.spatial){
            softmax_gpu(net.input_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }ܐܚܪܢܐ{
            softmax_gpu(net.input_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    ܐܢ(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        ܐܢ(l.softmax_tree){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_NUM, net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_NUM, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

فراغ backward_softmax_layer_gpu(ثابت softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
