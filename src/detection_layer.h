#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer detection_layer;

detection_layer make_detection_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rescore);
ଶୂନ୍ୟ forward_detection_layer(ଅଚଳ detection_layer l, network net);
ଶୂନ୍ୟ backward_detection_layer(ଅଚଳ detection_layer l, network net);

#ifdef GPU
ଶୂନ୍ୟ forward_detection_layer_gpu(ଅଚଳ detection_layer l, network net);
ଶୂନ୍ୟ backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
