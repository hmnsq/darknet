#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#অন্তর্ভাব "darknet.h"

layer make_upsample_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক stride);
শূন্য forward_upsample_layer(ধ্রুব layer l, network net);
শূন্য backward_upsample_layer(ধ্রুব layer l, network net);
শূন্য resize_upsample_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);

#ifdef GPU
শূন্য forward_upsample_layer_gpu(ধ্রুব layer l, network net);
শূন্য backward_upsample_layer_gpu(ধ্রুব layer l, network net);
#endif

#endif
