#அந்தர்பாவ "dropout_layer.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <stdio.h>

dropout_layer make_dropout_layer(முழு batch, முழு inputs, தசம probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, அளவு(தசம));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    திரும்பு l;
} 

வெறுமை resize_dropout_layer(dropout_layer *l, முழு inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*அளவு(தசம));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

வெறுமை forward_dropout_layer(dropout_layer l, network net)
{
    முழு i;
    எனில் (!net.train) திரும்பு;
    க்கு(i = 0; i < l.batch * l.inputs; ++i){
        தசம r = rand_uniform(0, 1);
        l.rand[i] = r;
        எனில்(r < l.probability) net.உள்ளீடு[i] = 0;
        இல்லையெனில் net.உள்ளீடு[i] *= l.scale;
    }
}

வெறுமை backward_dropout_layer(dropout_layer l, network net)
{
    முழு i;
    எனில்(!net.delta) திரும்பு;
    க்கு(i = 0; i < l.batch * l.inputs; ++i){
        தசம r = l.rand[i];
        எனில்(r < l.probability) net.delta[i] = 0;
        இல்லையெனில் net.delta[i] *= l.scale;
    }
}

