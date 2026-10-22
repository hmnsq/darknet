#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#অন্তর্ভাব "darknet.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_iseg_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা ids);
শূন্য forward_iseg_layer(ধ্রুবক layer l, network net);
শূন্য backward_iseg_layer(ধ্রুবক layer l, network net);
শূন্য resize_iseg_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
পূর্ণসংখ্যা iseg_অঙ্ক_detections(layer l, দশমিক thresh);

#ifdef GPU
শূন্য forward_iseg_layer_gpu(ধ্রুবক layer l, network net);
শূন্য backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
