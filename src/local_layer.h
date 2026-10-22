#اگر_غیر_تعریف_شدہ LOCAL_LAYER_H
#تعریف_کرو LOCAL_LAYER_H

#شامل "cuda.h"
#شامل "image.h"
#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer local_layer;

#اگر_تعریف_شدہ GPU
خالی forward_local_layer_gpu(local_layer layer, جال net);
خالی backward_local_layer_gpu(local_layer layer, جال net);
خالی update_local_layer_gpu(local_layer layer, update_دلائل a);

خالی ڈالو_local_layer(local_layer layer);
خالی pull_local_layer(local_layer layer);
#اگر_ختم

local_layer make_local_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح سائز, صحیح stride, صحیح pad, ACTIVATION activation);

خالی forward_local_layer(ثابت local_layer layer, جال net);
خالی backward_local_layer(local_layer layer, جال net);
خالی update_local_layer(local_layer layer, update_دلائل a);

خالی bias_output(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح سائز);
خالی backward_bias(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح سائز);

#اگر_ختم

