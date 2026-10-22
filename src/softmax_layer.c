#অন্তর্ভাব "softmax_layer.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "cuda.h"

#অন্তর্ভাব <float.h>
#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <assert.h>

softmax_layer make_softmax_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক groups)
{
    প্রতিজ্ঞা(inputs%groups == 0);
    fprintf(stderr, "softmax                                        %4d\n",  inputs);
    softmax_layer l = {0};
    l.type = SOFTMAX;
    l.batch = batch;
    l.groups = groups;
    l.inputs = inputs;
    l.outputs = inputs;
    l.loss = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.output = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.delta = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.cost = calloc(1, পরিমাণ(ভিন্ন));

    l.forward = forward_softmax_layer;
    l.backward = backward_softmax_layer;
    #ifdef GPU
    l.forward_gpu = forward_softmax_layer_gpu;
    l.backward_gpu = backward_softmax_layer_gpu;

    l.output_gpu = cuda_make_array(l.output, inputs*batch); 
    l.loss_gpu = cuda_make_array(l.loss, inputs*batch); 
    l.delta_gpu = cuda_make_array(l.delta, inputs*batch); 
    #endif
    প্রত্যাগম l;
}

শূন্য forward_softmax_layer(ধ্রুব softmax_layer l, network net)
{
    যদি(l.softmax_বৃক্ষ){
        পূর্ণাঙ্ক i;
        পূর্ণাঙ্ক গুনতি = 0;
        প্রতি (i = 0; i < l.softmax_বৃক্ষ->groups; ++i) {
            পূর্ণাঙ্ক group_size = l.softmax_বৃক্ষ->group_size[i];
            softmax_cpu(net.প্রবেশ + গুনতি, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output + গুনতি);
            গুনতি += group_size;
        }
    } অন্যথা {
        softmax_cpu(net.প্রবেশ, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output);
    }

    যদি(net.truth && !l.noloss){
        softmax_x_ent_cpu(l.batch*l.inputs, l.output, net.truth, l.delta, l.loss);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

শূন্য backward_softmax_layer(ধ্রুব softmax_layer l, network net)
{
    axpy_cpu(l.inputs*l.batch, 1, l.delta, 1, net.delta, 1);
}

#ifdef GPU

শূন্য pull_softmax_layer_output(ধ্রুব softmax_layer layer)
{
    cuda_pull_array(layer.output_gpu, layer.output, layer.inputs*layer.batch);
}

শূন্য forward_softmax_layer_gpu(ধ্রুব softmax_layer l, network net)
{
    যদি(l.softmax_বৃক্ষ){
        softmax_বৃক্ষ(net.প্রবেশ_gpu, 1, l.batch, l.inputs, l.temperature, l.output_gpu, *l.softmax_বৃক্ষ);
        /*
        int i;
        int count = 0;
        for (i = 0; i < l.softmax_tree->groups; ++i) {
            int group_size = l.softmax_tree->group_size[i];
            softmax_gpu(net.input_gpu + count, group_size, l.batch, l.inputs, 1, 0, 1, l.temperature, l.output_gpu + count);
            count += group_size;
        }
        */
    } অন্যথা {
        যদি(l.spatial){
            softmax_gpu(net.প্রবেশ_gpu, l.c, l.batch*l.c, l.inputs/l.c, l.w*l.h, 1, l.w*l.h, 1, l.output_gpu);
        }অন্যথা{
            softmax_gpu(net.প্রবেশ_gpu, l.inputs/l.groups, l.batch, l.inputs, l.groups, l.inputs/l.groups, 1, l.temperature, l.output_gpu);
        }
    }
    যদি(net.truth && !l.noloss){
        softmax_x_ent_gpu(l.batch*l.inputs, l.output_gpu, net.truth_gpu, l.delta_gpu, l.loss_gpu);
        যদি(l.softmax_বৃক্ষ){
            mask_gpu(l.batch*l.inputs, l.delta_gpu, SECRET_অঙ্ক, net.truth_gpu, 0);
            mask_gpu(l.batch*l.inputs, l.loss_gpu, SECRET_অঙ্ক, net.truth_gpu, 0);
        }
        cuda_pull_array(l.loss_gpu, l.loss, l.batch*l.inputs);
        l.cost[0] = sum_array(l.loss, l.batch*l.inputs);
    }
}

শূন্য backward_softmax_layer_gpu(ধ্রুব softmax_layer layer, network net)
{
    axpy_gpu(layer.batch*layer.inputs, 1, layer.delta_gpu, 1, net.delta_gpu, 1);
}

#endif
