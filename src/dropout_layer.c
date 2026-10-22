#অন্তর্ভাব "dropout_layer.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>

dropout_layer make_dropout_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, ভিন্ন probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, পরিমাণ(ভিন্ন));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    প্রত্যাগম l;
} 

শূন্য resize_dropout_layer(dropout_layer *l, পূর্ণাঙ্ক inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*পরিমাণ(ভিন্ন));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

শূন্য forward_dropout_layer(dropout_layer l, network net)
{
    পূর্ণাঙ্ক i;
    যদি (!net.train) প্রত্যাগম;
    প্রতি(i = 0; i < l.batch * l.inputs; ++i){
        ভিন্ন r = rand_uniform(0, 1);
        l.rand[i] = r;
        যদি(r < l.probability) net.প্রবেশ[i] = 0;
        অন্যথা net.প্রবেশ[i] *= l.scale;
    }
}

শূন্য backward_dropout_layer(dropout_layer l, network net)
{
    পূর্ণাঙ্ক i;
    যদি(!net.delta) প্রত্যাগম;
    প্রতি(i = 0; i < l.batch * l.inputs; ++i){
        ভিন্ন r = l.rand[i];
        যদি(r < l.probability) net.delta[i] = 0;
        অন্যথা net.delta[i] *= l.scale;
    }
}

