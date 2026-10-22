#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_batchnorm_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c);
শূন্য forward_batchnorm_layer(layer l, network net);
শূন্য backward_batchnorm_layer(layer l, network net);

#ifdef GPU
শূন্য forward_batchnorm_layer_gpu(layer l, network net);
শূন্য backward_batchnorm_layer_gpu(layer l, network net);
শূন্য pull_batchnorm_layer(layer l);
শূন্য ঠেলো_batchnorm_layer(layer l);
#endif

#endif
