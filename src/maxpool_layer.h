#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারনির্দেশ layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক size, পূর্ণাঙ্ক stride, পূর্ণাঙ্ক padding);
শূন্য resize_maxpool_layer(maxpool_layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
শূন্য forward_maxpool_layer(ধ্রুব maxpool_layer l, network net);
শূন্য backward_maxpool_layer(ধ্রুব maxpool_layer l, network net);

#ifdef GPU
শূন্য forward_maxpool_layer_gpu(maxpool_layer l, network net);
শূন্য backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

