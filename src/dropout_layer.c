#include "dropout_layer.h"
#include "utils.h"
#include "cuda.h"
#include <stdlib.h>
#include <stdio.h>

dropout_layer make_dropout_layer(정수 batch, 정수 inputs, float probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, sizeof(float));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    반환 l;
} 

void resize_dropout_layer(dropout_layer *l, 정수 inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*sizeof(float));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

void forward_dropout_layer(dropout_layer l, network net)
{
    정수 i;
    만약 (!net.train) 반환;
    위해(i = 0; i < l.batch * l.inputs; ++i){
        float r = rand_uniform(0, 1);
        l.rand[i] = r;
        만약(r < l.probability) net.input[i] = 0;
        아니면 net.input[i] *= l.scale;
    }
}

void backward_dropout_layer(dropout_layer l, network net)
{
    정수 i;
    만약(!net.delta) 반환;
    위해(i = 0; i < l.batch * l.inputs; ++i){
        float r = l.rand[i];
        만약(r < l.probability) net.delta[i] = 0;
        아니면 net.delta[i] *= l.scale;
    }
}

