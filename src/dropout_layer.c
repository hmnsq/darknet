#অন্তর্ভাব "dropout_layer.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <stdio.h>

dropout_layer make_dropout_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, দশমিক probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, আকার(দশমিক));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    ফেরত l;
} 

শূন্য resize_dropout_layer(dropout_layer *l, পূর্ণসংখ্যা inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*আকার(দশমিক));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

শূন্য forward_dropout_layer(dropout_layer l, network net)
{
    পূর্ণসংখ্যা i;
    যদি (!net.train) ফেরত;
    জন্য(i = 0; i < l.batch * l.inputs; ++i){
        দশমিক r = rand_uniform(0, 1);
        l.rand[i] = r;
        যদি(r < l.probability) net.প্রবেশ[i] = 0;
        অন্যথা net.প্রবেশ[i] *= l.scale;
    }
}

শূন্য backward_dropout_layer(dropout_layer l, network net)
{
    পূর্ণসংখ্যা i;
    যদি(!net.delta) ফেরত;
    জন্য(i = 0; i < l.batch * l.inputs; ++i){
        দশমিক r = l.rand[i];
        যদি(r < l.probability) net.delta[i] = 0;
        অন্যথা net.delta[i] *= l.scale;
    }
}

