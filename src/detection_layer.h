#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারনির্দেশ layer detection_layer;

detection_layer make_detection_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক n, পূর্ণাঙ্ক size, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক coords, পূর্ণাঙ্ক rescore);
শূন্য forward_detection_layer(ধ্রুব detection_layer l, network net);
শূন্য backward_detection_layer(ধ্রুব detection_layer l, network net);

#ifdef GPU
শূন্য forward_detection_layer_gpu(ধ্রুব detection_layer l, network net);
শূন্য backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
