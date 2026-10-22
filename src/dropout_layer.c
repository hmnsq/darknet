#ಅನ್ತರ್ಭಾವ "dropout_layer.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

dropout_layer make_dropout_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ದಶಮಾಂಶ probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(inputs*batch, ಗಾತ್ರ(ದಶಮಾಂಶ));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #ಆದರೆ_ಅಂತ್ಯ
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    ಹಿಂತಿರುಗಿ l;
} 

ಶೂನ್ಯ resize_dropout_layer(dropout_layer *l, ಪೂರ್ಣಾಂಕ inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*ಗಾತ್ರ(ದಶಮಾಂಶ));
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ forward_dropout_layer(dropout_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ (!net.train) ಹಿಂತಿರುಗಿ;
    ಗಾಗಿ(i = 0; i < l.batch * l.inputs; ++i){
        ದಶಮಾಂಶ r = rand_ಏಕರೂಪ(0, 1);
        l.rand[i] = r;
        ಆದರೆ(r < l.probability) net.ಒಳಹರಿವು[i] = 0;
        ಇಲ್ಲದಿದ್ದರೆ net.ಒಳಹರಿವು[i] *= l.scale;
    }
}

ಶೂನ್ಯ backward_dropout_layer(dropout_layer l, ಜಾಲ net)
{
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ(!net.delta) ಹಿಂತಿರುಗಿ;
    ಗಾಗಿ(i = 0; i < l.batch * l.inputs; ++i){
        ದಶಮಾಂಶ r = l.rand[i];
        ಆದರೆ(r < l.probability) net.delta[i] = 0;
        ಇಲ್ಲದಿದ್ದರೆ net.delta[i] *= l.scale;
    }
}

