
#اگر_تعریف_نشده GRU_LAYER_H
#تعریف_کن GRU_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_gru_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح گامها, صحيح batch_normalize, صحيح adam);

فراغ forward_gru_layer(layer l, شبکه state);
فراغ backward_gru_layer(layer l, شبکه state);
فراغ update_gru_layer(layer l, update_آرگومانها a);

#اگر_تعریف_شده GPU
فراغ forward_gru_layer_gpu(layer l, شبکه state);
فراغ backward_gru_layer_gpu(layer l, شبکه state);
فراغ update_gru_layer_gpu(layer l, update_آرگومانها a);
فراغ افزودن_gru_layer(layer l);
فراغ pull_gru_layer(layer l);
#پایان_اگر

#پایان_اگر

