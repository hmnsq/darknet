#ifndef L2NORM_LAYER_H
#define L2NORM_LAYER_H
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_l2norm_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs);
শূন্য forward_l2norm_layer(ধ্রুব layer l, network net);
শূন্য backward_l2norm_layer(ধ্রুব layer l, network net);

#ifdef GPU
শূন্য forward_l2norm_layer_gpu(ধ্রুব layer l, network net);
শূন্য backward_l2norm_layer_gpu(ধ্রুব layer l, network net);
#endif

#endif
