#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#ଅନ୍ତର୍ଭାଵ "darknet.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_iseg_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ids);
ଶୂନ୍ୟ forward_iseg_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_iseg_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ resize_iseg_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା iseg_ଅଙ୍କ_detections(layer l, ଦଶମିକ thresh);

#ifdef GPU
ଶୂନ୍ୟ forward_iseg_layer_gpu(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
