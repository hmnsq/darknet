#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারনির্দেশ layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c);
শূন্য resize_avgpool_layer(avgpool_layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
শূন্য forward_avgpool_layer(ধ্রুব avgpool_layer l, network net);
শূন্য backward_avgpool_layer(ধ্রুব avgpool_layer l, network net);

#ifdef GPU
শূন্য forward_avgpool_layer_gpu(avgpool_layer l, network net);
শূন্য backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

