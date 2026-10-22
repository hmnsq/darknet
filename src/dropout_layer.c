#ଅନ୍ତର୍ଭାଵ "dropout_layer.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>

dropout_layer make_dropout_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ଦଶମିକ probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, ଆକାର(ଦଶମିକ));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    ଫେରନ୍ତୁ l;
} 

ଶୂନ୍ୟ resize_dropout_layer(dropout_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*ଆକାର(ଦଶମିକ));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

ଶୂନ୍ୟ forward_dropout_layer(dropout_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି (!net.train) ଫେରନ୍ତୁ;
    ପାଇଁ(i = 0; i < l.batch * l.inputs; ++i){
        ଦଶମିକ r = rand_uniform(0, 1);
        l.rand[i] = r;
        ଯଦି(r < l.probability) net.ନିବେଶ[i] = 0;
        ଅନ୍ୟଥା net.ନିବେଶ[i] *= l.scale;
    }
}

ଶୂନ୍ୟ backward_dropout_layer(dropout_layer l, network net)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି(!net.delta) ଫେରନ୍ତୁ;
    ପାଇଁ(i = 0; i < l.batch * l.inputs; ++i){
        ଦଶମିକ r = l.rand[i];
        ଯଦି(r < l.probability) net.delta[i] = 0;
        ଅନ୍ୟଥା net.delta[i] *= l.scale;
    }
}

