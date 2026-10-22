#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "image.h"
#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer local_layer;

#ifdef GPU
শূন্য forward_local_layer_gpu(local_layer layer, network net);
শূন্য backward_local_layer_gpu(local_layer layer, network net);
শূন্য update_local_layer_gpu(local_layer layer, update_args a);

শূন্য ঠেলো_local_layer(local_layer layer);
শূন্য pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা pad, ACTIVATION activation);

শূন্য forward_local_layer(ধ্রুবক local_layer layer, network net);
শূন্য backward_local_layer(local_layer layer, network net);
শূন্য update_local_layer(local_layer layer, update_args a);

শূন্য bias_output(দশমিক *output, দশমিক *biases, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);
শূন্য backward_bias(দশমিক *bias_updates, দশমিক *delta, পূর্ণসংখ্যা batch, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2);

#endif

