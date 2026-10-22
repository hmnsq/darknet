#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#অন্তর্ভাৱ "cuda.h"
#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "activations.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

প্রকারনির্দেশ layer local_layer;

#ifdef GPU
শূন্য forward_local_layer_gpu(local_layer layer, network net);
শূন্য backward_local_layer_gpu(local_layer layer, network net);
শূন্য update_local_layer_gpu(local_layer layer, update_args a);

শূন্য ঠেলো_local_layer(local_layer layer);
শূন্য pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক pad, ACTIVATION activation);

শূন্য forward_local_layer(ধ্রুৱ local_layer layer, network net);
শূন্য backward_local_layer(local_layer layer, network net);
শূন্য update_local_layer(local_layer layer, update_args a);

শূন্য bias_output(ভিন্ন *output, ভিন্ন *biases, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);
শূন্য backward_bias(ভিন্ন *bias_updates, ভিন্ন *delta, পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size);

#endif

