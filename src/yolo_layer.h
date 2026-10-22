#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#অন্তর্ভাব "darknet.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_yolo_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা n, পূর্ণসংখ্যা যোগ_2, পূর্ণসংখ্যা *mask, পূর্ণসংখ্যা classes);
শূন্য forward_yolo_layer(ধ্রুবক layer l, network net);
শূন্য backward_yolo_layer(ধ্রুবক layer l, network net);
শূন্য resize_yolo_layer(layer *l, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
পূর্ণসংখ্যা yolo_অঙ্ক_detections(layer l, দশমিক thresh);

#ifdef GPU
শূন্য forward_yolo_layer_gpu(ধ্রুবক layer l, network net);
শূন্য backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
