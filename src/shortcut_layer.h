#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_shortcut_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক index, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক w2, পূর্ণাঙ্ক h2, পূর্ণাঙ্ক c2);
শূন্য forward_shortcut_layer(ধ্রুব layer l, network net);
শূন্য backward_shortcut_layer(ধ্রুব layer l, network net);
শূন্য resize_shortcut_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);

#ifdef GPU
শূন্য forward_shortcut_layer_gpu(ধ্রুব layer l, network net);
শূন্য backward_shortcut_layer_gpu(ধ্রুব layer l, network net);
#endif

#endif
