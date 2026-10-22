#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c);
শূন্য resize_avgpool_layer(avgpool_layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
শূন্য forward_avgpool_layer(ধ্রুবক avgpool_layer l, network net);
শূন্য backward_avgpool_layer(ধ্রুবক avgpool_layer l, network net);

#ifdef GPU
শূন্য forward_avgpool_layer_gpu(avgpool_layer l, network net);
শূন্য backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

