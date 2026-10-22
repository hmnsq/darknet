#અન્તર્ભાવ "dropout_layer.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <stdio.h>

dropout_layer make_dropout_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, દશાંશ probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, કદ(દશાંશ));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    પાછા l;
} 

શૂન્ય resize_dropout_layer(dropout_layer *l, પૂર્ણાંક inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*કદ(દશાંશ));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

શૂન્ય forward_dropout_layer(dropout_layer l, network net)
{
    પૂર્ણાંક i;
    જો (!net.train) પાછા;
    માટે(i = 0; i < l.batch * l.inputs; ++i){
        દશાંશ r = rand_uniform(0, 1);
        l.rand[i] = r;
        જો(r < l.probability) net.આગત[i] = 0;
        નહીંતર net.આગત[i] *= l.scale;
    }
}

શૂન્ય backward_dropout_layer(dropout_layer l, network net)
{
    પૂર્ણાંક i;
    જો(!net.delta) પાછા;
    માટે(i = 0; i < l.batch * l.inputs; ++i){
        દશાંશ r = l.rand[i];
        જો(r < l.probability) net.delta[i] = 0;
        નહીંતર net.delta[i] *= l.scale;
    }
}

