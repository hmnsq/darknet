#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#شامل "layer.h"
#شامل "network.h"

layer make_shortcut_layer(صحیح batch, صحیح index, صحیح w, صحیح h, صحیح c, صحیح w2, صحیح h2, صحیح c2);
تهی forward_shortcut_layer(ثابت layer l, network net);
تهی backward_shortcut_layer(ثابت layer l, network net);
تهی resize_shortcut_layer(layer *l, صحیح w, صحیح h);

#ifdef GPU
تهی forward_shortcut_layer_gpu(ثابت layer l, network net);
تهی backward_shortcut_layer_gpu(ثابت layer l, network net);
#endif

#endif
