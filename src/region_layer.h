#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#অন্তর্ভাৱ "darknet.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

layer make_region_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক n, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক coords);
শূন্য forward_region_layer(ধ্রুৱ layer l, network net);
শূন্য backward_region_layer(ধ্রুৱ layer l, network net);
শূন্য resize_region_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);

#ifdef GPU
শূন্য forward_region_layer_gpu(ধ্রুৱ layer l, network net);
শূন্য backward_region_layer_gpu(layer l, network net);
#endif

#endif
