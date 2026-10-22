#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

layer make_shortcut_layer(صحيح batch, صحيح index, صحيح w, صحيح h, صحيح c, صحيح w2, صحيح h2, صحيح c2);
فراغ forward_shortcut_layer(ثابت layer l, network net);
فراغ backward_shortcut_layer(ثابت layer l, network net);
فراغ resize_shortcut_layer(layer *l, صحيح w, صحيح h);

#ifdef GPU
فراغ forward_shortcut_layer_gpu(ثابت layer l, network net);
فراغ backward_shortcut_layer_gpu(ثابت layer l, network net);
#endif

#endif
