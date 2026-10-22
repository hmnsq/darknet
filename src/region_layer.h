#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#অন্তর্ভাব "darknet.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_region_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা n, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা coords);
শূন্য forward_region_layer(ধ্রুবক layer l, network net);
শূন্য backward_region_layer(ধ্রুবক layer l, network net);
শূন্য resize_region_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);

#ifdef GPU
শূন্য forward_region_layer_gpu(ধ্রুবক layer l, network net);
শূন্য backward_region_layer_gpu(layer l, network net);
#endif

#endif
