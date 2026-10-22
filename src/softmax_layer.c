#அந்தர்பாவ "softmax_layer.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "cuda.h"

#அந்தர்பாவ <float.h>
#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <assert.h>

softmax_layer make_softmax_layer(முழு batch, முழு inputs, முழு groups)
{
    ப்ரதிஜ்ஞா(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, அளவு(தசம));
    l.output = calloc(inputs*batch, அளவு(தசம));
    l.delta = calloc(inputs*batch, அளவு(தசம));
    l.cost = calloc(1, அளவு(தசம));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ifdef GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    திரும்பு l;
}

வெறுமை forward_softmax_layer(மாறிலி softmax_layer l, network net)
{
    எனில்(l.softmax_மரம்){
        முழு i;
        முழு மொத்தம் = 0;
        க்கு (i = 0; i < l.softmax_மரம்->groups; ++i) {
            முழு group_size = l.softmax_மரம்->group_size[i];
            softmax_cpu(net.உள்ளீடு + மொத்தம், group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + மொத்தம்);
            மொத்தம் += group_size;
        }
    } இல்லையெனில் {
        softmax_cpu(net.உள்ளீடு, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    எனில்(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

வெறுமை backward_softmax_layer(மாறிலி softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

வெறுமை pull_softmax_layer_output(மாறிலி softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

வெறுமை forward_softmax_layer_gpu(மாறிலி softmax_layer l, network net)
{
    எனில்(l.softmax_மரம்){
        softmax_மரம்(net.உள்ளீடு_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_மரம்);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } இல்லையெனில் {
        எனில்(l.spatial){
            softmax_gpu(net.உள்ளீடு_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }இல்லையெனில்{
            softmax_gpu(net.உள்ளீடு_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    எனில்(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        எனில்(l.softmax_மரம்){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_இலக்கம், net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_இலக்கம், net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

வெறுமை backward_softmax_layer_gpu(மாறிலி softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
