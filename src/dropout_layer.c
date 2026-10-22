#شامل "dropout_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل <stdlib.h>
#شامل <stdio.h>

dropout_layer make_dropout_layer(صحیح batch, صحیح inputs, اعشاری probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, اندازه(اعشاری));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    بازگشت l;
} 

تهی resize_dropout_layer(dropout_layer *l, صحیح inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*اندازه(اعشاری));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

تهی forward_dropout_layer(dropout_layer l, network net)
{
    صحیح i;
    اگر (!net.train) بازگشت;
    برای(i = 0; i < l.batch * l.inputs; ++i){
        اعشاری r = rand_uniform(0, 1);
        l.rand[i] = r;
        اگر(r < l.probability) net.درون_داد[i] = 0;
        وگرنه net.درون_داد[i] *= l.scale;
    }
}

تهی backward_dropout_layer(dropout_layer l, network net)
{
    صحیح i;
    اگر(!net.delta) بازگشت;
    برای(i = 0; i < l.batch * l.inputs; ++i){
        اعشاری r = l.rand[i];
        اگر(r < l.probability) net.delta[i] = 0;
        وگرنه net.delta[i] *= l.scale;
    }
}

