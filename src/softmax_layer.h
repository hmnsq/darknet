#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারনির্দেশ layer softmax_layer;

শূন্য softmax_array(ভিন্ন *প্রবেশ, পূর্ণাঙ্ক n, ভিন্ন temp, ভিন্ন *output);
softmax_layer make_softmax_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক groups);
শূন্য forward_softmax_layer(ধ্রুব softmax_layer l, network net);
শূন্য backward_softmax_layer(ধ্রুব softmax_layer l, network net);

#ifdef GPU
শূন্য pull_softmax_layer_output(ধ্রুব softmax_layer l);
শূন্য forward_softmax_layer_gpu(ধ্রুব softmax_layer l, network net);
শূন্য backward_softmax_layer_gpu(ধ্রুব softmax_layer l, network net);
#endif

#endif
