#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#অন্তর্ভাব "darknet.h"

layer make_upsample_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা stride);
শূন্য forward_upsample_layer(ধ্রুবক layer l, network net);
শূন্য backward_upsample_layer(ধ্রুবক layer l, network net);
শূন্য resize_upsample_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);

#ifdef GPU
শূন্য forward_upsample_layer_gpu(ধ্রুবক layer l, network net);
শূন্য backward_upsample_layer_gpu(ধ্রুবক layer l, network net);
#endif

#endif
