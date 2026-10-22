
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_crnn_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح hidden_filters, صحيح output_filters, صحيح steps, ACTIVATION activation, صحيح batch_normalize);

فراغ forward_crnn_layer(layer l, network net);
فراغ backward_crnn_layer(layer l, network net);
فراغ update_crnn_layer(layer l, update_args a);

#ifdef GPU
فراغ forward_crnn_layer_gpu(layer l, network net);
فراغ backward_crnn_layer_gpu(layer l, network net);
فراغ update_crnn_layer_gpu(layer l, update_args a);
فراغ تولق_crnn_layer(layer l);
فراغ pull_crnn_layer(layer l);
#endif

#endif

