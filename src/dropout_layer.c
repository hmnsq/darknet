#include "dropout_layer.h"
#include "utils.h"
#include "cuda.h"
#include <stdlib.h>
#include <stdio.h>

dropout_layer make_dropout_layer(ကိန်းပြည့် batch, ကိန်းပြည့် inputs, float probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, parimaa_na(float));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    ပြန်ပေး l;
} 

void resize_dropout_layer(dropout_layer *l, ကိန်းပြည့် inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*parimaa_na(float));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

void forward_dropout_layer(dropout_layer l, network net)
{
    ကိန်းပြည့် i;
    အကယ်၍ (!net.train) ပြန်ပေး;
    အတွက်(i = 0; i < l.batch * l.inputs; ++i){
        float r = rand_uniform(0, 1);
        l.rand[i] = r;
        အကယ်၍(r < l.probability) net.input[i] = 0;
        မဟုတ်ပါက net.input[i] *= l.scale;
    }
}

void backward_dropout_layer(dropout_layer l, network net)
{
    ကိန်းပြည့် i;
    အကယ်၍(!net.delta) ပြန်ပေး;
    အတွက်(i = 0; i < l.batch * l.inputs; ++i){
        float r = l.rand[i];
        အကယ်၍(r < l.probability) net.delta[i] = 0;
        မဟုတ်ပါက net.delta[i] *= l.scale;
    }
}

