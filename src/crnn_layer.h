
#که_نه_تعریف_شوی CRNN_LAYER_H
#تعریف_کړه CRNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_crnn_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح hidden_filters, صحيح output_filters, صحيح ګامونه, ACTIVATION activation, صحيح batch_normalize);

فراغ forward_crnn_layer(layer l, شبکه net);
فراغ backward_crnn_layer(layer l, شبکه net);
فراغ update_crnn_layer(layer l, update_دلیلونه a);

#که_تعریف_شوی GPU
فراغ forward_crnn_layer_gpu(layer l, شبکه net);
فراغ backward_crnn_layer_gpu(layer l, شبکه net);
فراغ update_crnn_layer_gpu(layer l, update_دلیلونه a);
فراغ ټېل_crnn_layer(layer l);
فراغ pull_crnn_layer(layer l);
#د_که_پای

#د_که_پای

