#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#شامل "darknet.h"
#شامل "layer.h"
#شامل "network.h"

layer make_iseg_layer(صحیح batch, صحیح w, صحیح h, صحیح classes, صحیح ids);
تهی forward_iseg_layer(ثابت layer l, network net);
تهی backward_iseg_layer(ثابت layer l, network net);
تهی resize_iseg_layer(layer *l, صحیح w, صحیح h);
صحیح iseg_رقم_detections(layer l, اعشاری thresh);

#ifdef GPU
تهی forward_iseg_layer_gpu(ثابت layer l, network net);
تهی backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
