#అన్తర్భావ "dropout_layer.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>

dropout_layer make_dropout_layer(పూర్ణాంకం batch, పూర్ణాంకం inputs, దశాంశం probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = సున్నా_జ్ఞాపకం_ఇవ్వు(inputs*batch, పరిమాణం(దశాంశం));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #నిర్వచితమైతే GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #అయితే_ముగింపు
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    తిరిగి l;
} 

శూన్య resize_dropout_layer(dropout_layer *l, పూర్ణాంకం inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*పరిమాణం(దశాంశం));
    #నిర్వచితమైతే GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #అయితే_ముగింపు
}

శూన్య forward_dropout_layer(dropout_layer l, జాలం net)
{
    పూర్ణాంకం i;
    ఒకవేళ (!net.train) తిరిగి;
    కొరకు(i = 0; i < l.batch * l.inputs; ++i){
        దశాంశం r = rand_ఏకరీతి(0, 1);
        l.rand[i] = r;
        ఒకవేళ(r < l.probability) net.ఆదానం[i] = 0;
        లేకపోతే net.ఆదానం[i] *= l.scale;
    }
}

శూన్య backward_dropout_layer(dropout_layer l, జాలం net)
{
    పూర్ణాంకం i;
    ఒకవేళ(!net.delta) తిరిగి;
    కొరకు(i = 0; i < l.batch * l.inputs; ++i){
        దశాంశం r = l.rand[i];
        ఒకవేళ(r < l.probability) net.delta[i] = 0;
        లేకపోతే net.delta[i] *= l.scale;
    }
}

