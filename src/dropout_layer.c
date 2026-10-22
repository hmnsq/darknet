#अन्तर्भाव "dropout_layer.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>

dropout_layer make_dropout_layer(पूर्णांक batch, पूर्णांक inputs, दशांश probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.अक्रम = सुस्मृति(inputs*batch, आकार(दशांश));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #ifdef GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.अक्रम, inputs*batch);
    #endif
    ख_लिखो(मानक_त्रुटि, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    परत l;
} 

शून्य resize_dropout_layer(dropout_layer *l, पूर्णांक inputs)
{
    l->अक्रम = पुनः_स्मृति(l->अक्रम, l->inputs*l->batch*आकार(दशांश));
    #ifdef GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->अक्रम, inputs*l->batch);
    #endif
}

शून्य forward_dropout_layer(dropout_layer l, network net)
{
    पूर्णांक i;
    जर (!net.train) परत;
    साठी(i = 0; i < l.batch * l.inputs; ++i){
        दशांश r = rand_uniform(0, 1);
        l.अक्रम[i] = r;
        जर(r < l.probability) net.आदान[i] = 0;
        अन्यथा net.आदान[i] *= l.scale;
    }
}

शून्य backward_dropout_layer(dropout_layer l, network net)
{
    पूर्णांक i;
    जर(!net.delta) परत;
    साठी(i = 0; i < l.batch * l.inputs; ++i){
        दशांश r = l.अक्रम[i];
        जर(r < l.probability) net.delta[i] = 0;
        अन्यथा net.delta[i] *= l.scale;
    }
}

