#ਅਨ੍ਤਰ੍ਭਾਵ "dropout_layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

dropout_layer make_dropout_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਦਸ਼ਮਲਵ probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(inputs*batch, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #ਜੇ_ਅੰਤ
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    ਵਾਪਸ l;
} 

ਖਾਲੀ resize_dropout_layer(dropout_layer *l, ਪੂਰਨ inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #ਜੇ_ਅੰਤ
}

ਖਾਲੀ forward_dropout_layer(dropout_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i;
    ਜੇ (!net.train) ਵਾਪਸ;
    ਲਈ(i = 0; i < l.batch * l.inputs; ++i){
        ਦਸ਼ਮਲਵ r = rand_ਇਕਸਾਰ(0, 1);
        l.rand[i] = r;
        ਜੇ(r < l.probability) net.ਨਿਵੇਸ਼[i] = 0;
        ਨਹੀਂਤਾਂ net.ਨਿਵੇਸ਼[i] *= l.scale;
    }
}

ਖਾਲੀ backward_dropout_layer(dropout_layer l, ਜਾਲ net)
{
    ਪੂਰਨ i;
    ਜੇ(!net.delta) ਵਾਪਸ;
    ਲਈ(i = 0; i < l.batch * l.inputs; ++i){
        ਦਸ਼ਮਲਵ r = l.rand[i];
        ਜੇ(r < l.probability) net.delta[i] = 0;
        ਨਹੀਂਤਾਂ net.delta[i] *= l.scale;
    }
}

