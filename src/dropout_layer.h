#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer dropout_layer;

dropout_layer make_dropout_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, দশমিক probability);

শূন্য forward_dropout_layer(dropout_layer l, network net);
শূন্য backward_dropout_layer(dropout_layer l, network net);
শূন্য resize_dropout_layer(dropout_layer *l, পূর্ণসংখ্যা inputs);

#ifdef GPU
শূন্য forward_dropout_layer_gpu(dropout_layer l, network net);
শূন্য backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif
