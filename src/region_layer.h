#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#ଅନ୍ତର୍ଭାଵ "darknet.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

layer make_region_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords);
ଶୂନ୍ୟ forward_region_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_region_layer(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ resize_region_layer(layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);

#ifdef GPU
ଶୂନ୍ୟ forward_region_layer_gpu(ଅଚଳ layer l, network net);
ଶୂନ୍ୟ backward_region_layer_gpu(layer l, network net);
#endif

#endif
