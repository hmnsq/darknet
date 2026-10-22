#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_logistic_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs);
শূন্য forward_logistic_layer(ধ্রুব layer l, network net);
শূন্য backward_logistic_layer(ধ্রুব layer l, network net);

#ifdef GPU
শূন্য forward_logistic_layer_gpu(ধ্রুব layer l, network net);
শূন্য backward_logistic_layer_gpu(ধ্রুব layer l, network net);
#endif

#endif
