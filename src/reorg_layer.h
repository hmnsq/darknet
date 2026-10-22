#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#ضمن "image.h"
#ضمن "cuda.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_reorg_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride, صحيح reverse, صحيح flatten, صحيح extra);
فراغ resize_reorg_layer(layer *l, صحيح w, صحيح h);
فراغ forward_reorg_layer(ثابت layer l, network net);
فراغ backward_reorg_layer(ثابت layer l, network net);

#ifdef GPU
فراغ forward_reorg_layer_gpu(layer l, network net);
فراغ backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

