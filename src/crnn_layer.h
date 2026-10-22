
#اگر_غیر_تعریف_شدہ CRNN_LAYER_H
#تعریف_کرو CRNN_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

layer make_crnn_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح hidden_filters, صحیح output_filters, صحیح اقدام, ACTIVATION activation, صحیح batch_normalize);

خالی forward_crnn_layer(layer l, جال net);
خالی backward_crnn_layer(layer l, جال net);
خالی update_crnn_layer(layer l, update_دلائل a);

#اگر_تعریف_شدہ GPU
خالی forward_crnn_layer_gpu(layer l, جال net);
خالی backward_crnn_layer_gpu(layer l, جال net);
خالی update_crnn_layer_gpu(layer l, update_دلائل a);
خالی ڈالو_crnn_layer(layer l);
خالی pull_crnn_layer(layer l);
#اگر_ختم

#اگر_ختم

