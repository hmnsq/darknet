#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#অন্তর্ভাব "darknet.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_yolo_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক n, পূর্ণাঙ্ক total, পূর্ণাঙ্ক *mask, পূর্ণাঙ্ক classes);
শূন্য forward_yolo_layer(ধ্রুব layer l, network net);
শূন্য backward_yolo_layer(ধ্রুব layer l, network net);
শূন্য resize_yolo_layer(layer *l, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
পূর্ণাঙ্ক yolo_অঙ্ক_detections(layer l, ভিন্ন thresh);

#ifdef GPU
শূন্য forward_yolo_layer_gpu(ধ্রুব layer l, network net);
শূন্য backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
