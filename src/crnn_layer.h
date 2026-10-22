
#جيڪڏهن_وصف_نه_ٿيل CRNN_LAYER_H
#وصف_ڪريو CRNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_crnn_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح hidden_filters, صحيح output_filters, صحيح قدمن, ACTIVATION activation, صحيح batch_normalize);

فراغ forward_crnn_layer(layer l, ڄار net);
فراغ backward_crnn_layer(layer l, ڄار net);
فراغ update_crnn_layer(layer l, update_دليلون a);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_crnn_layer_gpu(layer l, ڄار net);
فراغ backward_crnn_layer_gpu(layer l, ڄار net);
فراغ update_crnn_layer_gpu(layer l, update_دليلون a);
فراغ ڌڪو_crnn_layer(layer l);
فراغ pull_crnn_layer(layer l);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

