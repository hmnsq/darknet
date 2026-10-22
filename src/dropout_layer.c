#ಅನ್ತರ್ಭಾವ "dropout_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

dropout_layer make_dropout_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಭಿನ್ನ probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = calloc(inputs*batch, ಪರಿಮಾಣ(ಭಿನ್ನ));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #endif
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    ಪ್ರತ್ಯಾಗಮ l;
} 

ಶೂನ್ಯ resize_dropout_layer(dropout_layer *l, ಪೂರ್ಣಾಙ್ಕ inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*ಪರಿಮಾಣ(ಭಿನ್ನ));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #endif
}

ಶೂನ್ಯ forward_dropout_layer(dropout_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ (!net.train) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ(i = 0; i < l.batch * l.inputs; ++i){
        ಭಿನ್ನ r = rand_uniform(0, 1);
        l.rand[i] = r;
        ಯದಿ(r < l.probability) net.ಒಳಹರಿವು[i] = 0;
        ಅನ್ಯಥಾ net.ಒಳಹರಿವು[i] *= l.scale;
    }
}

ಶೂನ್ಯ backward_dropout_layer(dropout_layer l, network net)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ(!net.delta) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿ(i = 0; i < l.batch * l.inputs; ++i){
        ಭಿನ್ನ r = l.rand[i];
        ಯದಿ(r < l.probability) net.delta[i] = 0;
        ಅನ್ಯಥಾ net.delta[i] *= l.scale;
    }
}

