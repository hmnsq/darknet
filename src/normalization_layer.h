#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_normalization_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, পূর্ণসংখ্যা আকার_2, দশমিক alpha, দশমিক beta, দশমিক kappa);
শূন্য resize_normalization_layer(layer *layer, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w);
শূন্য forward_normalization_layer(ধ্রুবক layer layer, network net);
শূন্য backward_normalization_layer(ধ্রুবক layer layer, network net);
শূন্য visualize_normalization_layer(layer layer, অক্ষর *window);

#ifdef GPU
শূন্য forward_normalization_layer_gpu(ধ্রুবক layer layer, network net);
শূন্য backward_normalization_layer_gpu(ধ্রুবক layer layer, network net);
#endif

#endif
