#അന്തര്ഭാവ "dropout_layer.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>

dropout_layer make_dropout_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, ദശാംശം probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, വലുപ്പം(ദശാംശം));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    തിരികെ l;
} 

ശൂന്യം resize_dropout_layer(dropout_layer *l, പൂർണ്ണസംഖ്യ inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*വലുപ്പം(ദശാംശം));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

ശൂന്യം forward_dropout_layer(dropout_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ (!net.train) തിരികെ;
    വേണ്ടി(i = 0; i < l.batch * l.inputs; ++i){
        ദശാംശം r = rand_uniform(0, 1);
        l.rand[i] = r;
        എങ്കിൽ(r < l.probability) net.നിവേശം[i] = 0;
        അല്ലെങ്കിൽ net.നിവേശം[i] *= l.scale;
    }
}

ശൂന്യം backward_dropout_layer(dropout_layer l, network net)
{
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ(!net.delta) തിരികെ;
    വേണ്ടി(i = 0; i < l.batch * l.inputs; ++i){
        ദശാംശം r = l.rand[i];
        എങ്കിൽ(r < l.probability) net.delta[i] = 0;
        അല്ലെങ്കിൽ net.delta[i] *= l.scale;
    }
}

