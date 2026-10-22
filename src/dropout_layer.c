#समावेश "dropout_layer.h"
#समावेश "utils.h"
#समावेश "cuda.h"
#समावेश <stdlib.h>
#समावेश <stdio.h>

dropout_layer make_dropout_layer(पूर्णांक batch, पूर्णांक inputs, भग्न probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.अक्रम = सुस्मृति(inputs*batch, माप(भग्न));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #अगर_घोषित GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.अक्रम, inputs*batch);
    #पूर्ण_अगर
    ख_लिखो(मानक_त्रुटि, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    वापस l;
} 

व्योम resize_dropout_layer(dropout_layer *l, पूर्णांक inputs)
{
    l->अक्रम = पुनः_स्मृति(l->अक्रम, l->inputs*l->batch*माप(भग्न));
    #अगर_घोषित GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->अक्रम, inputs*l->batch);
    #पूर्ण_अगर
}

व्योम forward_dropout_layer(dropout_layer l, network net)
{
    पूर्णांक i;
    अगर (!net.train) वापस;
    क्रम(i = 0; i < l.batch * l.inputs; ++i){
        भग्न r = rand_uniform(0, 1);
        l.अक्रम[i] = r;
        अगर(r < l.probability) net.निवेश[i] = 0;
        अन्यथा net.निवेश[i] *= l.scale;
    }
}

व्योम backward_dropout_layer(dropout_layer l, network net)
{
    पूर्णांक i;
    अगर(!net.delta) वापस;
    क्रम(i = 0; i < l.batch * l.inputs; ++i){
        भग्न r = l.अक्रम[i];
        अगर(r < l.probability) net.delta[i] = 0;
        अन्यथा net.delta[i] *= l.scale;
    }
}

