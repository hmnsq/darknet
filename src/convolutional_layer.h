#اگر_غیر_تعریف_شدہ CONVOLUTIONAL_LAYER_H
#تعریف_کرو CONVOLUTIONAL_LAYER_H

#شامل "cuda.h"
#شامل "image.h"
#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer convolutional_layer;

#اگر_تعریف_شدہ GPU
خالی forward_convolutional_layer_gpu(convolutional_layer layer, جال net);
خالی backward_convolutional_layer_gpu(convolutional_layer layer, جال net);
خالی update_convolutional_layer_gpu(convolutional_layer layer, update_دلائل a);

خالی ڈالو_convolutional_layer(convolutional_layer layer);
خالی pull_convolutional_layer(convolutional_layer layer);

خالی جوڑو_bias_gpu(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح سائز);
خالی backward_bias_gpu(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح سائز);
خالی adam_update_gpu(اعشاری *w, اعشاری *d, اعشاری *m, اعشاری *v, اعشاری B1, اعشاری B2, اعشاری eps, اعشاری decay, اعشاری شرح, صحیح n, صحیح batch, صحیح t);
#اگر_تعریف_شدہ CUDNN
خالی cudnn_convolutional_تیاری(layer *l);
#اگر_ختم
#اگر_ختم

convolutional_layer make_convolutional_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح groups, صحیح سائز, صحیح stride, صحیح padding, ACTIVATION activation, صحیح batch_normalize, صحیح ثنائی, صحیح xnor, صحیح adam);
خالی resize_convolutional_layer(convolutional_layer *layer, صحیح w, صحیح h);
خالی forward_convolutional_layer(ثابت convolutional_layer layer, جال net);
خالی update_convolutional_layer(convolutional_layer layer, update_دلائل a);
image *visualize_convolutional_layer(convolutional_layer layer, حرف *کھڑکی, image *پچھلا_weights);
خالی binarize_weights(اعشاری *weights, صحیح n, صحیح سائز, اعشاری *ثنائی);
خالی تبادلہ_ثنائی(convolutional_layer *l);
خالی binarize_weights2(اعشاری *weights, صحیح n, صحیح سائز, حرف *ثنائی, اعشاری *scales);

خالی backward_convolutional_layer(convolutional_layer layer, جال net);

خالی جوڑو_bias(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح سائز);
خالی backward_bias(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح سائز);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_وزن(convolutional_layer layer, صحیح i);

صحیح convolutional_برآمد_اونچائی(convolutional_layer layer);
صحیح convolutional_برآمد_چوڑائی(convolutional_layer layer);

#اگر_ختم

