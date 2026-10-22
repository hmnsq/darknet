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

layer make_deconvolutional_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক padding, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize, পূর্ণাঙ্ক adam);
শূন্য resize_deconvolutional_layer(layer *l, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w);
শূন্য forward_deconvolutional_layer(ধ্রুব layer l, network net);
শূন্য update_deconvolutional_layer(layer l, update_args a);
শূন্য backward_deconvolutional_layer(layer l, network net);

#endif

