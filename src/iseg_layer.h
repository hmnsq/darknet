#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#অন্তর্ভাৱ "darknet.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

layer make_iseg_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক ids);
শূন্য forward_iseg_layer(ধ্রুৱ layer l, network net);
শূন্য backward_iseg_layer(ধ্রুৱ layer l, network net);
শূন্য resize_iseg_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
পূর্ণাঙ্ক iseg_অঙ্ক_detections(layer l, ভিন্ন thresh);

#ifdef GPU
শূন্য forward_iseg_layer_gpu(ধ্রুৱ layer l, network net);
শূন্য backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
