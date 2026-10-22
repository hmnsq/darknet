
#جے_بنا_تعریف CRNN_LAYER_H
#تعریف_کرو CRNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_crnn_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح hidden_filters, صحيح output_filters, صحيح قدماں, ACTIVATION activation, صحيح batch_normalize);

فراغ forward_crnn_layer(layer l, جال net);
فراغ backward_crnn_layer(layer l, جال net);
فراغ update_crnn_layer(layer l, update_دلیلاں a);

#جے_تعریف_شدہ GPU
فراغ forward_crnn_layer_gpu(layer l, جال net);
فراغ backward_crnn_layer_gpu(layer l, جال net);
فراغ update_crnn_layer_gpu(layer l, update_دلیلاں a);
فراغ ڈالو_crnn_layer(layer l);
فراغ pull_crnn_layer(layer l);
#جے_انت

#جے_انت

