#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "cuda.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

layer make_reorg_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক reverse, পূর্ণাঙ্ক flatten, পূর্ণাঙ্ক extra);
শূন্য resize_reorg_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
শূন্য forward_reorg_layer(ধ্রুৱ layer l, network net);
শূন্য backward_reorg_layer(ধ্রুৱ layer l, network net);

#ifdef GPU
শূন্য forward_reorg_layer_gpu(layer l, network net);
শূন্য backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

