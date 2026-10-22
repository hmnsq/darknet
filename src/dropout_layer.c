#अन्तर्भाव "dropout_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>

dropout_layer make_dropout_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, भिन्न probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, परिमाण(भिन्न));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    पटिनिवत्त l;
} 

शून्य resize_dropout_layer(dropout_layer *l, पूर्णाङ्क inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*परिमाण(भिन्न));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

शून्य forward_dropout_layer(dropout_layer l, network net)
{
    पूर्णाङ्क i;
    सचे (!net.train) पटिनिवत्त;
    अत्थाय(i = 0; i < l.batch * l.inputs; ++i){
        भिन्न r = rand_uniform(0, 1);
        l.rand[i] = r;
        सचे(r < l.probability) net.निवेशः[i] = 0;
        अञ्ञथा net.निवेशः[i] *= l.scale;
    }
}

शून्य backward_dropout_layer(dropout_layer l, network net)
{
    पूर्णाङ्क i;
    सचे(!net.delta) पटिनिवत्त;
    अत्थाय(i = 0; i < l.batch * l.inputs; ++i){
        भिन्न r = l.rand[i];
        सचे(r < l.probability) net.delta[i] = 0;
        अञ्ञथा net.delta[i] *= l.scale;
    }
}

