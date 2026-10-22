#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_reorg_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা reverse, পূর্ণসংখ্যা flatten, পূর্ণসংখ্যা extra);
শূন্য resize_reorg_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
শূন্য forward_reorg_layer(ধ্রুবক layer l, network net);
শূন্য backward_reorg_layer(ধ্রুবক layer l, network net);

#ifdef GPU
শূন্য forward_reorg_layer_gpu(layer l, network net);
শূন্য backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

