#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_iseg_layer(صحيح batch, صحيح w, صحيح h, صحيح classes, صحيح ids);
فراغ forward_iseg_layer(ثابت layer l, network net);
فراغ backward_iseg_layer(ثابت layer l, network net);
فراغ resize_iseg_layer(layer *l, صحيح w, صحيح h);
صحيح iseg_عدد_detections(layer l, عائم thresh);

#ifdef GPU
فراغ forward_iseg_layer_gpu(ثابت layer l, network net);
فراغ backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
