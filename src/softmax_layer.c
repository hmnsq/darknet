#ଅନ୍ତର୍ଭାଵ "softmax_layer.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"

#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>

softmax_layer make_softmax_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା groups)
{
    ପ୍ରତିଜ୍ଞା(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, ଆକାର(ଦଶମିକ));
    l.output = calloc(inputs*batch, ଆକାର(ଦଶମିକ));
    l.delta = calloc(inputs*batch, ଆକାର(ଦଶମିକ));
    l.cost = calloc(1, ଆକାର(ଦଶମିକ));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ifdef GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    ଫେରନ୍ତୁ l;
}

ଶୂନ୍ୟ forward_softmax_layer(ଅଚଳ softmax_layer l, network net)
{
    ଯଦି(l.softmax_ବୃକ୍ଷ){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
        ପାଇଁ (i = 0; i < l.softmax_ବୃକ୍ଷ->groups; ++i) {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_size = l.softmax_ବୃକ୍ଷ->group_size[i];
            softmax_cpu(net.ନିବେଶ + ଗଣନା_2, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + ଗଣନା_2);
            ଗଣନା_2 += group_size;
        }
    } ଅନ୍ୟଥା {
        softmax_cpu(net.ନିବେଶ, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    ଯଦି(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

ଶୂନ୍ୟ backward_softmax_layer(ଅଚଳ softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

ଶୂନ୍ୟ pull_softmax_layer_output(ଅଚଳ softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

ଶୂନ୍ୟ forward_softmax_layer_gpu(ଅଚଳ softmax_layer l, network net)
{
    ଯଦି(l.softmax_ବୃକ୍ଷ){
        softmax_ବୃକ୍ଷ(net.ନିବେଶ_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_ବୃକ୍ଷ);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } ଅନ୍ୟଥା {
        ଯଦି(l.spatial){
            softmax_gpu(net.ନିବେଶ_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }ଅନ୍ୟଥା{
            softmax_gpu(net.ନିବେଶ_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    ଯଦି(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        ଯଦି(l.softmax_ବୃକ୍ଷ){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_ଅଙ୍କ, net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_ଅଙ୍କ, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

ଶୂନ୍ୟ backward_softmax_layer_gpu(ଅଚଳ softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
