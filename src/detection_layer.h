#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer detection_layer;

detection_layer make_detection_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, পূর্ণসংখ্যা n, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা coords, পূর্ণসংখ্যা rescore);
শূন্য forward_detection_layer(ধ্রুবক detection_layer l, network net);
শূন্য backward_detection_layer(ধ্রুবক detection_layer l, network net);

#ifdef GPU
শূন্য forward_detection_layer_gpu(ধ্রুবক detection_layer l, network net);
শূন্য backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
