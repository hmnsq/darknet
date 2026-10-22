#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

layer make_normalization_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, পূর্ণাঙ্ক size, ভিন্ন alpha, ভিন্ন beta, ভিন্ন kappa);
শূন্য resize_normalization_layer(layer *layer, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w);
শূন্য forward_normalization_layer(ধ্রুৱ layer layer, network net);
শূন্য backward_normalization_layer(ধ্রুৱ layer layer, network net);
শূন্য visualize_normalization_layer(layer layer, ৱর্ণ *window);

#ifdef GPU
শূন্য forward_normalization_layer_gpu(ধ্রুৱ layer layer, network net);
শূন্য backward_normalization_layer_gpu(ধ্রুৱ layer layer, network net);
#endif

#endif
