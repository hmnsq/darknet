#ضمن "dropout_layer.h"
#ضمن "utils.h"
#ضمن "cuda.h"
#ضمن <stdlib.h>
#ضمن <stdio.h>

dropout_layer make_dropout_layer(صحيح batch, صحيح inputs, عائم probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, حجم(عائم));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    החזר l;
} 

فراغ resize_dropout_layer(dropout_layer *l, صحيح inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*حجم(عائم));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

فراغ forward_dropout_layer(dropout_layer l, network net)
{
    صحيح i;
    אם (!net.train) החזר;
    עבור(i = 0; i < l.batch * l.inputs; ++i){
        عائم r = rand_uniform(0, 1);
        l.rand[i] = r;
        אם(r < l.probability) net.input[i] = 0;
        אחרת net.input[i] *= l.scale;
    }
}

فراغ backward_dropout_layer(dropout_layer l, network net)
{
    صحيح i;
    אם(!net.delta) החזר;
    עבור(i = 0; i < l.batch * l.inputs; ++i){
        عائم r = l.rand[i];
        אם(r < l.probability) net.delta[i] = 0;
        אחרת net.delta[i] *= l.scale;
    }
}

