#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "image.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

#ifdef GPU
শূন্য forward_deconvolutional_layer_gpu(layer l, network net);
শূন্য backward_deconvolutional_layer_gpu(layer l, network net);
শূন্য update_deconvolutional_layer_gpu(layer l, update_args a);
শূন্য ঠেলো_deconvolutional_layer(layer l);
শূন্য pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা padding, ACTIVATION activation, পূর্ণসংখ্যা batch_normalize, পূর্ণসংখ্যা adam);
শূন্য resize_deconvolutional_layer(layer *l, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w);
শূন্য forward_deconvolutional_layer(ধ্রুবক layer l, network net);
শূন্য update_deconvolutional_layer(layer l, update_args a);
শূন্য backward_deconvolutional_layer(layer l, network net);

#endif

