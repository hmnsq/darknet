
#اگر_تعریف_نشده CRNN_LAYER_H
#تعریف_کن CRNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_crnn_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح hidden_filters, صحيح output_filters, صحيح گامها, ACTIVATION activation, صحيح batch_normalize);

فراغ forward_crnn_layer(layer l, شبکه net);
فراغ backward_crnn_layer(layer l, شبکه net);
فراغ update_crnn_layer(layer l, update_آرگومانها a);

#اگر_تعریف_شده GPU
فراغ forward_crnn_layer_gpu(layer l, شبکه net);
فراغ backward_crnn_layer_gpu(layer l, شبکه net);
فراغ update_crnn_layer_gpu(layer l, update_آرگومانها a);
فراغ افزودن_crnn_layer(layer l);
فراغ pull_crnn_layer(layer l);
#پایان_اگر

#پایان_اگر

