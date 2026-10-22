#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

layer make_batchnorm_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c);
শূন্য forward_batchnorm_layer(layer l, network net);
শূন্য backward_batchnorm_layer(layer l, network net);

#ifdef GPU
শূন্য forward_batchnorm_layer_gpu(layer l, network net);
শূন্য backward_batchnorm_layer_gpu(layer l, network net);
শূন্য pull_batchnorm_layer(layer l);
শূন্য ঠেলো_batchnorm_layer(layer l);
#endif

#endif
