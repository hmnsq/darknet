#അന്തര്ഭാവ "softmax_layer.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "cuda.h"

#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <assert.h>

softmax_layer make_softmax_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ groups)
{
    പ്രതിജ്ഞാ(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.output = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.delta = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.cost = calloc(1, വലുപ്പം(ദശാംശം));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ifdef GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    തിരികെ l;
}

ശൂന്യം forward_softmax_layer(സ്ഥിരം softmax_layer l, network net)
{
    എങ്കിൽ(l.softmax_വൃക്ഷം){
        പൂർണ്ണസംഖ്യ i;
        പൂർണ്ണസംഖ്യ എണ്ണം = 0;
        വേണ്ടി (i = 0; i < l.softmax_വൃക്ഷം->groups; ++i) {
            പൂർണ്ണസംഖ്യ group_size = l.softmax_വൃക്ഷം->group_size[i];
            softmax_cpu(net.നിവേശം + എണ്ണം, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + എണ്ണം);
            എണ്ണം += group_size;
        }
    } അല്ലെങ്കിൽ {
        softmax_cpu(net.നിവേശം, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    എങ്കിൽ(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

ശൂന്യം backward_softmax_layer(സ്ഥിരം softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ശൂന്യം pull_softmax_layer_output(സ്ഥിരം softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

ശൂന്യം forward_softmax_layer_gpu(സ്ഥിരം softmax_layer l, network net)
{
    എങ്കിൽ(l.softmax_വൃക്ഷം){
        softmax_വൃക്ഷം(net.നിവേശം_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_വൃക്ഷം);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } അല്ലെങ്കിൽ {
        എങ്കിൽ(l.spatial){
            softmax_gpu(net.നിവേശം_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }അല്ലെങ്കിൽ{
            softmax_gpu(net.നിവേശം_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    എങ്കിൽ(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        എങ്കിൽ(l.softmax_വൃക്ഷം){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_അക്കം, net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_അക്കം, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

ശൂന്യം backward_softmax_layer_gpu(സ്ഥിരം softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
