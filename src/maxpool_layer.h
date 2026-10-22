#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা stride, পূর্ণসংখ্যা padding);
শূন্য resize_maxpool_layer(maxpool_layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
শূন্য forward_maxpool_layer(ধ্রুবক maxpool_layer l, network net);
শূন্য backward_maxpool_layer(ধ্রুবক maxpool_layer l, network net);

#ifdef GPU
শূন্য forward_maxpool_layer_gpu(maxpool_layer l, network net);
শূন্য backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

