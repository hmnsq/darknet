#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#ଅନ୍ତର୍ଭାଵ "darknet.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_yolo_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *mask, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes);
ଶୂନ୍ୟ forward_yolo_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_yolo_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ resize_yolo_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା yolo_ଅଙ୍କ_detections(layer l, ଦଶମିକ thresh);

#ifdef GPU
ଶୂନ୍ୟ forward_yolo_layer_gpu(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
