#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#ضمن "darknet.h"

layer make_upsample_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride);
فراغ forward_upsample_layer(ثابت layer l, network net);
فراغ backward_upsample_layer(ثابت layer l, network net);
فراغ resize_upsample_layer(layer *l, صحيح w, صحيح h);

#ifdef GPU
فراغ forward_upsample_layer_gpu(ثابت layer l, network net);
فراغ backward_upsample_layer_gpu(ثابت layer l, network net);
#endif

#endif
