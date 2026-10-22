#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer softmax_layer;

শূন্য softmax_array(দশমিক *প্রবেশ, পূর্ণসংখ্যা n, দশমিক অস্থায়ী, দশমিক *output);
softmax_layer make_softmax_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, পূর্ণসংখ্যা groups);
শূন্য forward_softmax_layer(ধ্রুবক softmax_layer l, network net);
শূন্য backward_softmax_layer(ধ্রুবক softmax_layer l, network net);

#ifdef GPU
শূন্য pull_softmax_layer_output(ধ্রুবক softmax_layer l);
শূন্য forward_softmax_layer_gpu(ধ্রুবক softmax_layer l, network net);
শূন্য backward_softmax_layer_gpu(ধ্রুবক softmax_layer l, network net);
#endif

#endif
