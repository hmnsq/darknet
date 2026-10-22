#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c);
ଶୂନ୍ୟ resize_avgpool_layer(avgpool_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
ଶୂନ୍ୟ forward_avgpool_layer(ଅଚଳ avgpool_layer l, network net);
ଶୂନ୍ୟ backward_avgpool_layer(ଅଚଳ avgpool_layer l, network net);

#ifdef GPU
ଶୂନ୍ୟ forward_avgpool_layer_gpu(avgpool_layer l, network net);
ଶୂନ୍ୟ backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

