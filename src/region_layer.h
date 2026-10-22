#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_region_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح classes, صحيح coords);
فراغ forward_region_layer(ثابت layer l, network net);
فراغ backward_region_layer(ثابت layer l, network net);
فراغ resize_region_layer(layer *l, صحيح w, صحيح h);

#ifdef GPU
فراغ forward_region_layer_gpu(ثابت layer l, network net);
فراغ backward_region_layer_gpu(layer l, network net);
#endif

#endif
