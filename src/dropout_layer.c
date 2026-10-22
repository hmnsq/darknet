#شامل "dropout_layer.h"
#شامل "utils.h"
#شامل "cuda.h"
#شامل <stdlib.h>
#شامل <stdio.h>

dropout_layer make_dropout_layer(صحیح batch, صحیح inputs, اعشاری probability)
{
    dropout_layer l = {0};
    l.type = DROPOUT;
    l.probability = probability;
    l.inputs = inputs;
    l.outputs = inputs;
    l.batch = batch;
    l.rand = صفر_حافظہ_دو(inputs*batch, حجم(اعشاری));
    l.scale = 1./(1.-probability);
    l.forward = forward_dropout_layer;
    l.backward = backward_dropout_layer;
    #اگر_تعریف_شدہ GPU
    l.forward_gpu = forward_dropout_layer_gpu;
    l.backward_gpu = backward_dropout_layer_gpu;
    l.rand_gpu = cuda_make_array(l.rand, inputs*batch);
    #اگر_ختم
    fprintf(stderr, "dropout       p = %.2f               %4d  ->  %4d\n", probability, inputs, inputs);
    واپس l;
} 

خالی resize_dropout_layer(dropout_layer *l, صحیح inputs)
{
    l->rand = realloc(l->rand, l->inputs*l->batch*حجم(اعشاری));
    #اگر_تعریف_شدہ GPU
    cuda_free(l->rand_gpu);

    l->rand_gpu = cuda_make_array(l->rand, inputs*l->batch);
    #اگر_ختم
}

خالی forward_dropout_layer(dropout_layer l, جال net)
{
    صحیح i;
    اگر (!net.train) واپس;
    برائے(i = 0; i < l.batch * l.inputs; ++i){
        اعشاری r = rand_یکساں(0, 1);
        l.rand[i] = r;
        اگر(r < l.probability) net.داخلہ[i] = 0;
        ورنہ net.داخلہ[i] *= l.scale;
    }
}

خالی backward_dropout_layer(dropout_layer l, جال net)
{
    صحیح i;
    اگر(!net.delta) واپس;
    برائے(i = 0; i < l.batch * l.inputs; ++i){
        اعشاری r = l.rand[i];
        اگر(r < l.probability) net.delta[i] = 0;
        ورنہ net.delta[i] *= l.scale;
    }
}

