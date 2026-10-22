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
    l.rand = حافظه_صفر_بده(inputs*batch, حجم(عائم));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #اگر_تعریف_شده GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #پایان_اگر
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    أرجع l;
} 

فراغ resize_dropout_layer(dropout_layer *l, صحيح inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*حجم(عائم));
    #اگر_تعریف_شده GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #پایان_اگر
}

فراغ forward_dropout_layer(dropout_layer l, شبکه net)
{
    صحيح i;
    إذا (!net.train) أرجع;
    لكل(i = 0; i < l.batch * l.inputs; ++i){
        عائم r = rand_یکنواخت(0, 1);
        l.rand[i] = r;
        إذا(r < l.probability) net.درون_داد[i] = 0;
        وإلا net.درون_داد[i] *= l.scale;
    }
}

فراغ backward_dropout_layer(dropout_layer l, شبکه net)
{
    صحيح i;
    إذا(!net.delta) أرجع;
    لكل(i = 0; i < l.batch * l.inputs; ++i){
        عائم r = l.rand[i];
        إذا(r < l.probability) net.delta[i] = 0;
        وإلا net.delta[i] *= l.scale;
    }
}

