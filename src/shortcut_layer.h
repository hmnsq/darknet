#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_shortcut_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা সূচক, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা w2, পূর্ণসংখ্যা h2, পূর্ণসংখ্যা c2);
শূন্য forward_shortcut_layer(ধ্রুবক layer l, network net);
শূন্য backward_shortcut_layer(ধ্রুবক layer l, network net);
শূন্য resize_shortcut_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);

#ifdef GPU
শূন্য forward_shortcut_layer_gpu(ধ্রুবক layer l, network net);
শূন্য backward_shortcut_layer_gpu(ধ্রুবক layer l, network net);
#endif

#endif
